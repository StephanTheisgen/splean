// (C) Copyright by Stephan Theisgen
//
// This file contains some helper function to create SipHash keys and construct the header,
// as well as a precise timestamp function for time calculations later. Some functions to
// "siphash"-ing of edges in batches (consecutive and from an edge stream). It also contains
// a function to verify a Cuckatoo cycle, closely adapted from from John Tromp (2019)
// (see https://github.com/tromp/cuckoo/blob/master/src/crypto/siphashxN.h)
//
// The SIMD reinterpret_casts technically violate C++'s strict aliasing rules,
// so compile with -fno-strict-aliasing.

#ifndef HELPER_HPP
#define HELPER_HPP

#include <cstdint>
#include <chrono>
#include <ctime>

#include "portable_endian.h"
#include "blake2.h"

#include "splean.hpp"
#include "siphash.hpp"
#include "rice.hpp"

// DATA TYPE SPECIFICATIONS

typedef rice_stream<STREAM_SIZE, STREAM_PARAM> edge_strm;
typedef bloom_filter<WALKFLT_SIZE> walk_flt;
typedef bloom_filter<TRIMFLT_SIZE> trim_flt;

// HELPER FUNCTIONS

//calculate nodes using edges from a stream, uses SIMD and batch-processing if available
inline uint32_t calc_nodes(edge_strm& stream, const uint8_t uorv, uint64_t (&ns)[NSIPHASH], siphasher& hasher) noexcept {
	uint32_t es[NSIPHASH];
	uint32_t i = 0;
	for(; i < NSIPHASH; i++) {
		if(!stream.read(es[i])) break;
	}
#if (NSIPHASH == 1)
	ns[0] = (hasher.siphash24x1((2*es[0])+uorv) & NODE_MASK);
#elif (NSIPHASH == 2) && __SSE2__ && (!__AVX2__)
	static const __m128i mask = _mm_set1_epi64x(NODE_MASK);
	const __m128i uv = _mm_set1_epi64x(uorv);
	const __m128i indices = _mm_add_epi64(_mm_slli_epi64(_mm_set_epi64x(es[1], es[0]), 1), uv);
	__m128i hashes;
	hasher.siphash24x2(indices, hashes);
	reinterpret_cast<__m128i&>(ns) = _mm_and_si128(hashes, mask); //alignment has to guaranteed by the caller
	//for(int i = 0; i < 2; i++) ns[i] = _mm_extract_epi64(_mm_and_si128(hashes, mask), i);
#elif (NSIPHASH == 4) && __AVX2__
	static const __m256i mask = _mm256_set1_epi64x(NODE_MASK);
	const __m256i uv = _mm256_set1_epi64x(uorv);
	const __m256i indices = _mm256_add_epi64(_mm256_slli_epi64(_mm256_set_epi64x(es[3], es[2], es[1], es[0]), 1), uv);
	__m256i hashes;
	hasher.siphash24x4(indices, hashes);
	reinterpret_cast<__m256i&>(ns) = _mm256_and_si256(hashes, mask); //alignment has to guaranteed by the caller
	//for(int i = 0; i < 4; i++) ns[i] = _mm256_extract_epi64(_mm256_and_si256(hashes, mask), i);
#elif (NSIPHASH == 4) && __SSE2__
	static const __m128i mask = _mm_set1_epi64x(NODE_MASK);
	const __m128i uv = _mm_set1_epi64x(uorv);
	const __m128i indices0 = _mm_add_epi64(_mm_slli_epi64(_mm_set_epi64x(es[1], es[0]), 1), uv);
	const __m128i indices1 = _mm_add_epi64(_mm_slli_epi64(_mm_set_epi64x(es[3], es[2]), 1), uv);
	__m128i hashes0, hashes1;
	hasher.siphash24x4(indices0, indices1, hashes0, hashes1);
	reinterpret_cast<__m128i&>(ns[0]) = _mm_and_si128(hashes0, mask); //alignment has to guaranteed by the caller
	reinterpret_cast<__m128i&>(ns[2]) = _mm_and_si128(hashes1, mask); //alignment has to guaranteed by the caller
	//for(int i = 0; i < 2; i++) ns[0+i] = _mm_extract_epi64(_mm_and_si128(hashes, mask), i);
	//for(int i = 0; i < 2; i++) ns[2+i] = _mm_extract_epi64(_mm_and_si128(hashes, mask), i);
#elif (NSIPHASH == 8) && __AVX2__
	static const __m256i mask = _mm256_set1_epi64x(NODE_MASK);
	const __m256i uv = _mm256_set1_epi64x(uorv);
	const __m256i indices0 = _mm256_add_epi64(_mm256_slli_epi64(_mm256_set_epi64x(es[3], es[2], es[1], es[0]), 1), uv);
	const __m256i indices1 = _mm256_add_epi64(_mm256_slli_epi64(_mm256_set_epi64x(es[7], es[6], es[5], es[4]), 1), uv);
	__m256i hashes0, hashes1;
	hasher.siphash24x8(indices0, indices1, hashes0, hashes1);
	reinterpret_cast<__m256i&>(ns[0]) = _mm256_and_si256(hashes0, mask); //alignment has to guaranteed by the caller
	reinterpret_cast<__m256i&>(ns[4]) = _mm256_and_si256(hashes1, mask); //alignment has to guaranteed by the caller
	//for(int i = 0; i < 4; i++) ns[0+i] = _mm256_extract_epi64(_mm256_and_si256(hashes0, mask), i);
	//for(int i = 0; i < 4; i++) ns[4+i] = _mm256_extract_epi64(_mm256_and_si256(hashes1, mask), i);
#else
	#error combination of NSIPHASH and SIMD instruction set not implemented
#endif
	return i;
}

//calculate nodes from NSIPHASH consecutive edges (starting at e), uses SIMD and batch-processing if available
inline void calc_nodes(const uint32_t e, const uint8_t uorv, uint64_t (&ns)[NSIPHASH], siphasher& hasher) noexcept {
#if (NSIPHASH == 1)
	ns[0] = (hasher.siphash24x1((2*e)+uorv) & NODE_MASK);
#elif (NSIPHASH == 2) && __SSE2__ && (!__AVX2__)
	static const __m128i mask = _mm_set1_epi64x(NODE_MASK);
	static const __m128i run = _mm_set_epi64x(1, 0);
	const __m128i uv = _mm_set1_epi64x(uorv);
	const __m128i edges = _mm_add_epi64(_mm_set1_epi64x(e), run);
	const __m128i indices = _mm_add_epi64(_mm_slli_epi64(edges, 1), uv);
	__m128i hashes;
	hasher.siphash24x2(indices, hashes);
	reinterpret_cast<__m128i&>(ns) = _mm_and_si128(hashes, mask); //alignment has to guaranteed by the caller
	//for(int i = 0; i < 2; i++) ns[i] = _mm_extract_epi64(_mm_and_si128(hashes, mask), i);
#elif (NSIPHASH == 4) && __AVX2__
	static const __m256i mask = _mm256_set1_epi64x(NODE_MASK);
	static const __m256i run = _mm256_set_epi64x(3, 2, 1, 0);
	const __m256i uv = _mm256_set1_epi64x(uorv);
	const __m256i edges = _mm256_add_epi64(_mm256_set1_epi64x(e), run);
	const __m256i indices = _mm256_add_epi64(_mm256_slli_epi64(edges, 1), uv);
	__m256i hashes;
	hasher.siphash24x4(indices, hashes);
	reinterpret_cast<__m256i&>(ns) = _mm256_and_si256(hashes, mask); //alignment has to guaranteed by the caller
	//for(int i = 0; i < 4; i++) ns[i] = _mm256_extract_epi64(_mm256_and_si256(hashes, mask), i);
#elif (NSIPHASH == 4) && __SSE2__
	static const __m128i mask = _mm_set1_epi64x(NODE_MASK);
	static const __m128i run0 = _mm_set_epi64x(1, 0);
	static const __m128i run1 = _mm_set_epi64x(3, 2);
	const __m128i uv = _mm_set1_epi64x(uorv);
	const __m128i edges0 = _mm_add_epi64(_mm_set1_epi64x(e), run0);
	const __m128i edges1 = _mm_add_epi64(_mm_set1_epi64x(e), run1);
	const __m128i indices0 = _mm_add_epi64(_mm_slli_epi64(edges0, 1), uv);
	const __m128i indices1 = _mm_add_epi64(_mm_slli_epi64(edges1, 1), uv);
	__m128i hashes0, hashes1;
	hasher.siphash24x4(indices0, indices1, hashes0, hashes1);
	reinterpret_cast<__m128i&>(ns[0]) = _mm_and_si128(hashes0, mask); //alignment has to guaranteed by the caller
	reinterpret_cast<__m128i&>(ns[2]) = _mm_and_si128(hashes1, mask); //alignment has to guaranteed by the caller
	//for(int i = 0; i < 2; i++) ns[0+i] = _mm_extract_epi64(_mm_and_si128(hashes, mask), i);
	//for(int i = 0; i < 2; i++) ns[2+i] = _mm_extract_epi64(_mm_and_si128(hashes, mask), i);
#elif (NSIPHASH == 8) && __AVX2__
	static const __m256i mask = _mm256_set1_epi64x(NODE_MASK);
	static const __m256i run0 = _mm256_set_epi64x(3, 2, 1, 0);
	static const __m256i run1 = _mm256_set_epi64x(7, 6, 5, 4);
	const __m256i uv = _mm256_set1_epi64x(uorv);
	const __m256i edges0 = _mm256_add_epi64(_mm256_set1_epi64x(e), run0);
	const __m256i edges1 = _mm256_add_epi64(_mm256_set1_epi64x(e), run1);
	const __m256i indices0 = _mm256_add_epi64(_mm256_slli_epi64(edges0, 1), uv);
	const __m256i indices1 = _mm256_add_epi64(_mm256_slli_epi64(edges1, 1), uv);
	__m256i hashes0, hashes1;
	hasher.siphash24x8(indices0, indices1, hashes0, hashes1);
	reinterpret_cast<__m256i&>(ns[0]) = _mm256_and_si256(hashes0, mask); //alignment has to guaranteed by the caller
	reinterpret_cast<__m256i&>(ns[4]) = _mm256_and_si256(hashes1, mask); //alignment has to guaranteed by the caller
	//for(int i = 0; i < 4; i++) ns[0+i] = _mm256_extract_epi64(_mm256_and_si256(hashes0, mask), i);
	//for(int i = 0; i < 4; i++) ns[4+i] = _mm256_extract_epi64(_mm256_and_si256(hashes1, mask), i);
#else
	#error combination of NSIPHASH and SIMD instruction set not implemented
#endif
//	for(uint32_t i = 0; i < NSIPHASH; i++) hasher.siphash24x1((2*(e+i))+uorv, ns[i], NODE_MASK);
}

inline uint64_t calc_node(const uint32_t edge, const uint8_t uorv, siphasher& hasher) noexcept {
	return (hasher.siphash24x1((2*edge)+uorv) & NODE_MASK);
}

void get_siphash_keys(const char* header, siphasher::keys& keys) {
	uint64_t hdrkeys[4];
	blake2b((void*)hdrkeys, sizeof(hdrkeys), (const void *)header, HEADER_LEN, 0, 0);
	keys.v0 = htole64(hdrkeys[0]);
	keys.v1 = htole64(hdrkeys[1]);
	keys.v2 = htole64(hdrkeys[2]);
	keys.v3 = htole64(hdrkeys[3]);
}

void set_header_nonce(char *header, const uint32_t nonce) {
	((uint32_t*)header)[HEADER_LEN/sizeof(uint32_t)-1] = htole32(nonce); //place nonce at the end
}

uint64_t timestamp() {
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::chrono::duration<uint64_t, std::ratio<1, 1000000000>> d = now.time_since_epoch();
	return d.count();
}

bool verify(const uint32_t edges[PROOF_SIZE], siphasher& hasher) {
	uint32_t uvs[2*PROOF_SIZE];
	for (uint32_t n = 0; n < PROOF_SIZE; n++) {
		if (edges[n] > EDGE_MASK) return false;
		if (n && edges[n] <= edges[n-1]) return false;
		uvs[2*n  ] = calc_node(edges[n], 0, hasher);
		uvs[2*n+1] = calc_node(edges[n], 1, hasher);
	}
	uint32_t n = 0, i = 0, j;
	do {
		for (uint32_t k = j = i; (k = (k+2) % (2*PROOF_SIZE)) != i; ) {
			if (uvs[k] >> 1 == uvs[i] >> 1) {
				if (j != i) return false;
				j = k;
			}
		}
		if (j == i || uvs[j] == uvs[i]) return false;
		i = j^1;
		n++;
	} while (i != 0);
	return n == PROOF_SIZE ? true : false;
};

#endif