// (C) Copyright by Stephan Theisgen
//
// The code in this file is based on the file "siphashxN.h" from John Tromp (2019)
// (see https://github.com/tromp/cuckoo/blob/master/src/crypto/siphashxN.h)
// If available, it uses SIMD instructions to calculate SipHash in parallel.
// Currently, it supports 2x (SSE2), 4x (SSE2/AVX2), 8x (AVX2)
// parallel SipHash calculations. To activate it use the NSIPHASH compiler option.
//
// The siphasher class is not thread-safe. So, each thread needs its own instance and counts
// the amount of total hashes thread-local.

#ifndef SIPHASH_HPP
#define SIPHASH_HPP

#include <cstdint>
#include <immintrin.h>

#ifdef __AVX2__

	#define ADD(a, b) _mm256_add_epi64(a, b)
	#define XOR(a, b) _mm256_xor_si256(a, b)
	#define AND(a, b) _mm256_and_si256(a, b)
	#define ROTATE16 _mm256_set_epi64x(0x0D0C0B0A09080F0EULL,0x0504030201000706ULL, 0x0D0C0B0A09080F0EULL, 0x0504030201000706ULL)
	#define ROT13(x) _mm256_or_si256(_mm256_slli_epi64(x,13),_mm256_srli_epi64(x,51))
	#define ROT16(x) _mm256_shuffle_epi8((x), ROTATE16)
	#define ROT17(x) _mm256_or_si256(_mm256_slli_epi64(x,17),_mm256_srli_epi64(x,47))
	#define ROT21(x) _mm256_or_si256(_mm256_slli_epi64(x,21),_mm256_srli_epi64(x,43))
	#define ROT23(x) _mm256_or_si256(_mm256_slli_epi64(x,23),_mm256_srli_epi64(x,41))
	#define ROT25(x) _mm256_or_si256(_mm256_slli_epi64(x,25),_mm256_srli_epi64(x,39))
	#define ROT32(x) _mm256_shuffle_epi32((x), _MM_SHUFFLE(2, 3, 0, 1))

#elif defined __SSE2__

	#define ADD(a, b) _mm_add_epi64(a, b)
	#define XOR(a, b) _mm_xor_si128(a, b)
	#define AND(a, b) _mm_and_si128(a, b)
	#define ROT13(x) _mm_or_si128(_mm_slli_epi64(x,13),_mm_srli_epi64(x,51))
	#define ROT16(x) _mm_shufflehi_epi16(_mm_shufflelo_epi16(x, _MM_SHUFFLE(2,1,0,3)), _MM_SHUFFLE(2,1,0,3))
	#define ROT17(x) _mm_or_si128(_mm_slli_epi64(x,17),_mm_srli_epi64(x,47))
	#define ROT21(x) _mm_or_si128(_mm_slli_epi64(x,21),_mm_srli_epi64(x,43))
	#define ROT23(x) _mm_or_si128(_mm_slli_epi64(x,23),_mm_srli_epi64(x,41))
	#define ROT25(x) _mm_or_si128(_mm_slli_epi64(x,25),_mm_srli_epi64(x,39))
	#define ROT32(x) _mm_shuffle_epi32  (x, _MM_SHUFFLE(2,3,0,1))

#endif

#define ROTL(x, b) ((x<<b)|(x>>(64-b)))

#ifndef ROT_E
	#define ROT_E ROT21
	#define ROTL_E(x) ROTL(x,21)
#endif

#define SIPROUND \
do { \
	v0 += v1; v2 += v3; v1 = ROTL(v1,13); \
	v3 = ROTL(v3,16); v1 ^= v0; v3 ^= v2; \
	v0 = ROTL(v0,32); v2 += v1; v0 += v3; \
	v1 = ROTL(v1,17); v3 = ROTL_E(v3); \
	v1 ^= v2; v3 ^= v0; v2 = ROTL(v2,32); \
} while(0)

#define SIPROUNDXN \
do { \
	v0 = ADD(v0,v1); v2 = ADD(v2,v3); v1 = ROT13(v1); \
	v3 = ROT16(v3);  v1 = XOR(v1,v0); v3 = XOR(v3,v2); \
	v0 = ROT32(v0);  v2 = ADD(v2,v1); v0 = ADD(v0,v3); \
	v1 = ROT17(v1);                   v3 = ROT_E(v3); \
	v1 = XOR(v1,v2); v3 = XOR(v3,v0); v2 = ROT32(v2); \
} while(0)

#define SIPROUNDX2N \
do { \
	v0 = ADD(v0,v1); v4 = ADD(v4,v5); \
	v2 = ADD(v2,v3); v6 = ADD(v6,v7); \
	v1 = ROT13(v1);  v5 = ROT13(v5); \
	v3 = ROT16(v3);  v7 = ROT16(v7); \
	v1 = XOR(v1,v0); v5 = XOR(v5,v4); \
	v3 = XOR(v3,v2); v7 = XOR(v7,v6); \
	v0 = ROT32(v0);  v4 = ROT32(v4); \
	v2 = ADD(v2,v1); v6 = ADD(v6,v5); \
	v0 = ADD(v0,v3); v4 = ADD(v4,v7); \
	v1 = ROT17(v1);  v5 = ROT17(v5); \
	v3 = ROT_E(v3);  v7 = ROT_E(v7); \
	v1 = XOR(v1,v2); v5 = XOR(v5,v6); \
	v3 = XOR(v3,v0); v7 = XOR(v7,v4); \
	v2 = ROT32(v2);  v6 = ROT32(v6); \
} while(0)
 
#define SIPROUNDX4N \
do { \
	v0 = ADD(v0,v1); v4 = ADD(v4,v5);  v8 = ADD(v8,v9); vC = ADD(vC,vD); \
	v2 = ADD(v2,v3); v6 = ADD(v6,v7);  vA = ADD(vA,vB); vE = ADD(vE,vF); \
	v1 = ROT13(v1);  v5 = ROT13(v5);   v9 = ROT13(v9);  vD = ROT13(vD); \
	v3 = ROT16(v3);  v7 = ROT16(v7);   vB = ROT16(vB);  vF = ROT16(vF); \
	v1 = XOR(v1,v0); v5 = XOR(v5,v4);  v9 = XOR(v9,v8); vD = XOR(vD,vC); \
	v3 = XOR(v3,v2); v7 = XOR(v7,v6);  vB = XOR(vB,vA); vF = XOR(vF,vE); \
	v0 = ROT32(v0);  v4 = ROT32(v4);   v8 = ROT32(v8);  vC = ROT32(vC); \
	v2 = ADD(v2,v1); v6 = ADD(v6,v5);  vA = ADD(vA,v9); vE = ADD(vE,vD); \
	v0 = ADD(v0,v3); v4 = ADD(v4,v7);  v8 = ADD(v8,vB); vC = ADD(vC,vF); \
	v1 = ROT17(v1);  v5 = ROT17(v5);   v9 = ROT17(v9);  vD = ROT17(vD); \
	v3 = ROT_E(v3);  v7 = ROT_E(v7);   vB = ROT_E(vB);  vF = ROT_E(vF); \
	v1 = XOR(v1,v2); v5 = XOR(v5,v6);  v9 = XOR(v9,vA); vD = XOR(vD,vE); \
	v3 = XOR(v3,v0); v7 = XOR(v7,v4);  vB = XOR(vB,v8); vF = XOR(vF,vC); \
	v2 = ROT32(v2);  v6 = ROT32(v6);   vA = ROT32(vA);  vE = ROT32(vE); \
} while(0)

class siphasher {

	public:

		struct keys { 
			uint64_t v0, v1, v2, v3;
		};	

		siphasher() noexcept { reset(0, 0, 0, 0); };
		siphasher(const uint64_t v0, const uint64_t v1, const uint64_t v2, const uint64_t v3) noexcept { reset(v0, v1, v2, v3); };
		siphasher(const siphasher::keys& keys) noexcept { reset(keys.v0, keys.v1, keys.v2, keys.v3); };

		inline void reset(const uint64_t v0, const uint64_t v1, const uint64_t v2, const uint64_t v3) noexcept {
			hcnt = 0;
			_v0 = v0; _v1 = v1;	_v2 = v2; _v3 = v3;
		};
		
		inline uint64_t count() const noexcept { return hcnt; };

		inline uint64_t siphash24x1(const uint64_t& index) noexcept {
			uint64_t v0 = _v0, v1 = _v1, v2 = _v2, v3 = _v3;
			v3 ^= index;
			SIPROUND; SIPROUND;
			v0 ^= index;
			v2 ^= 0xff;
			SIPROUND; SIPROUND; SIPROUND; SIPROUND;
			hcnt++;
			return (v0 ^ v1 ^ v2  ^ v3);
		};

#ifdef __AVX2__

		inline void siphash24x4(const __m256i& indices, __m256i& hashes) noexcept {
			const __m256i packet = indices;
			__m256i v0 = _mm256_set1_epi64x(_v0);
			__m256i v1 = _mm256_set1_epi64x(_v1);
			__m256i v2 = _mm256_set1_epi64x(_v2);
			__m256i v3 = _mm256_set1_epi64x(_v3);
			v3 = XOR(v3,packet);
			SIPROUNDXN; SIPROUNDXN;
			v0 = XOR(v0,packet);
			v2 = XOR(v2,_mm256_set1_epi64x(0xffLL));
			SIPROUNDXN; SIPROUNDXN; SIPROUNDXN; SIPROUNDXN;
			hashes = XOR(XOR(v0,v1),XOR(v2,v3));
			hcnt++;
		};

		inline void siphash24x8(const __m256i& indices0, const __m256i& indices1, __m256i& hashes0, __m256i& hashes1) noexcept {
			const __m256i packet0 = indices0;
			const __m256i packet4 = indices1;
			__m256i v0, v1, v2, v3, v4, v5, v6, v7;
			v7 = v3 = _mm256_set1_epi64x(_v3);
			v4 = v0 = _mm256_set1_epi64x(_v0);
			v5 = v1 = _mm256_set1_epi64x(_v1);
			v6 = v2 = _mm256_set1_epi64x(_v2);
			v3 = XOR(v3,packet0); v7 = XOR(v7,packet4);
			SIPROUNDX2N; SIPROUNDX2N;
			v0 = XOR(v0,packet0); v4 = XOR(v4,packet4);
			v2 = XOR(v2,_mm256_set1_epi64x(0xffLL));
			v6 = XOR(v6,_mm256_set1_epi64x(0xffLL));
			SIPROUNDX2N; SIPROUNDX2N; SIPROUNDX2N; SIPROUNDX2N;
			hashes0 = XOR(XOR(v0,v1),XOR(v2,v3));
			hashes1 = XOR(XOR(v4,v5),XOR(v6,v7));
			hcnt++;
		};

#elif defined __SSE2__

		inline void siphash24x2(const __m128i& indices, __m128i& hashes) noexcept {
			const __m128i mi = indices;
			__m128i v0, v1, v2, v3;
			v0 = _mm_set1_epi64x(_v0);
			v1 = _mm_set1_epi64x(_v1);
			v2 = _mm_set1_epi64x(_v2);
			v3 = _mm_set1_epi64x(_v3);
			v3 = XOR (v3, mi);
			SIPROUNDXN; SIPROUNDXN;
			v0 = XOR (v0, mi);
			v2 = XOR (v2, _mm_set1_epi64x(0xffLL));
			SIPROUNDXN; SIPROUNDXN; SIPROUNDXN; SIPROUNDXN;
			hashes = XOR(XOR(v0,v1),XOR(v2,v3));
			hcnt++;
		};

		inline void siphash24x4(const __m128i& indices0, const __m128i& indices1, __m128i& hashes0, __m128i& hashes1) noexcept {
			const __m128i mi = indices0;
			const __m128i m2 = indices1;
			__m128i v0, v1, v2, v3, v4, v5, v6, v7;
			v4 = v0 = _mm_set1_epi64x(_v0);
			v5 = v1 = _mm_set1_epi64x(_v1);
			v6 = v2 = _mm_set1_epi64x(_v2);
			v7 = v3 = _mm_set1_epi64x(_v3);
			v3 = XOR (v3, mi);
			v7 = XOR (v7, m2);
			SIPROUNDX2N; SIPROUNDX2N;
			v0 = XOR (v0, mi);
			v4 = XOR (v4, m2);
			v2 = XOR (v2, _mm_set1_epi64x(0xffLL));
			v6 = XOR (v6, _mm_set1_epi64x(0xffLL));
			SIPROUNDX2N; SIPROUNDX2N; SIPROUNDX2N; SIPROUNDX2N;
			hashes0 = XOR(XOR(v0,v1),XOR(v2,v3));
			hashes1 = XOR(XOR(v4,v5),XOR(v6,v7));
			hcnt++;
		};

#endif

	private:

		uint64_t _v0, _v1, _v2, _v3;
		uint64_t hcnt;

};

#endif