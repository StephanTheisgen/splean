// (C) Copyright by Stephan Theisgen
//
// This is a quick and dirty implementation of a blocked Bloom filter.
// The code is based on code from Apache Impala (incubating) and its version from here:
// https://github.com/peterboncz/bloomfilter-repro/blob/master/src/simd-block.h 
//
// The SIMD reinterpret_casts technically violate C++'s strict aliasing rules,
// so compile with -fno-strict-aliasing.
//
// It needs to be thread-safe as all threads needs to be able to store and read from the same instance.
// This implementation allows this by controlling access on a per-bucket level. It allows only one thread to 
// update a bucket at a time. As storing and reading values are strictly separated phases using the
// splean miner approach, reading is not controlled. After storing all values, a barrier must be implemented
// to make sure the filter is fully up-to-date before threads begin to read from it in the next phase.
//
// This is a blocked Bloom filter (from Putze et al.'s "Cache-, Hash- and Space-Efficient
// Bloom Filters") with some twists:
// 1. Each block is a split Bloom filter - see Section 2.1 of Broder and Mitzenmacher's
// "Network Applications of Bloom Filters: A Survey".
// 2. The number of bits set per Add() is constant in order to take advantage of SIMD
// instructions.
//
// It also allows an amount of buckets that is not a power of two. This is crucial for space efficiency.
// For higher speed it avoids a modulo operation to randomly choose a bucket (in hash_index function),
// using the approach from here: https://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
// It uses efficient hashing (in hash function) from "High Speed Hashing for Integers and Strings" 
// (by Mikkel Thorup, see https://arxiv.org/pdf/1504.06804).
// Luckily the performance of this hash functions are not too crucial as all values to store come from a
// SipHash24 function, which has already good performance. This allows choosing the fastest hash functions,
// and greatly helps to reach top performance.

#ifndef BLOOM_HPP
#define BLOOM_HPP

#include <cstdint>
#include <cstring>

#include <atomic>

#include <immintrin.h>

template<uint32_t SIZE> //SIZE in bits
class bloom_filter {

	using buc_t = uint64_t[4];

	public:

		bloom_filter() : a(0xd3d4de51c6bddcab), b(0xf25b261118de7499) { clear(); };  //odd random seeds

		double usage() const noexcept { return (double)cnt/size(); }; //usage in items per bits
		uint32_t count() const noexcept { return cnt; }; //items stored

	private:

		inline uint32_t hash(const uint32_t x) const noexcept { //see "High Speed Hashing for Integers and Strings" by Mikkel Thorup
			return (a*x+b)>>32;
		};

		inline uint32_t hash_index(const uint32_t x) const noexcept { //see "A fast alternative to the modulo reduction" by Daniel Lemire
			return ((uint64_t)x*(uint64_t)(M))>>32;
		};

		static const uint32_t BWM = (sizeof(buc_t)*8); //size of a bucket in bits
		static const uint32_t M = (SIZE+(BWM-1))/BWM; //amount of buckets

		const uint64_t a, b;
		
#if (NTHREADS > 1) //concurrent version

  	public:

		inline void clear() noexcept { //only one thread should call clear
			//memset(mutex, 0, S*sizeof(uint64_t)); //using memset on std::atomic would give a warning
			for(uint32_t s = 0; s < S; s++) mutex[s].store(0, std::memory_order_relaxed);
			memset(buckets, 0, M*sizeof(buc_t));
			cnt = 0;
		};

		static inline uint64_t size() noexcept { //size in bits
			return M*BWM + S*BWS;
		};

  	private:

		static const uint8_t BWS = sizeof(uint64_t)*8; //bit-width of a word of mutexes
		static const uint32_t S = (M+(BWS-1))/BWS; //words needed to have a mutex for each bucket

		std::atomic<uint64_t> mutex[S];

		inline void lock(const uint32_t idx) noexcept {
			uint64_t msk = (1llu << (idx % BWS)); //mask to set the corresponding bit in mutex word
			while(uint64_t old = std::atomic_fetch_or_explicit(&mutex[idx/BWS], msk, std::memory_order_acquire) & msk) {}; //update mutex word
		};

		inline void unlock(const uint32_t idx) noexcept {
			uint64_t msk = ~(1llu << (idx % BWS)); //mask to clear the corresponding bit in mutex word
			atomic_fetch_and_explicit(&mutex[idx/BWS], msk, std::memory_order_release);	//update mutex word
		};

  		std::atomic<uint32_t> cnt;

#else //non-concurrent version

	public:

		inline void clear() noexcept {
	    	memset(buckets, 0, M*sizeof(buc_t));
			cnt = 0;
		};

		static inline uint64_t size() noexcept { //size in bits
			return M*BWM;
		};

	private: 

		inline void lock(const uint32_t idx) noexcept {};
		inline void unlock(const uint32_t idx) noexcept {};

		uint32_t cnt;

#endif

#ifdef __AVX2__ //SIMD version to speed up insert and look-up

 	public:

 		inline void insert(const uint32_t item) noexcept {
			const uint32_t hsh = hash(item);
			const uint32_t idx = hash_index(hsh);
			const __m256i msk = make_mask(hsh);
			lock(idx);
				__m256i* const buc = &reinterpret_cast<__m256i*>(buckets)[idx];
				_mm256_store_si256(buc, _mm256_or_si256(*buc, msk));
			unlock(idx);
			cnt++;
		};

		inline bool contains(const uint32_t item) const noexcept {
			const uint32_t hsh = hash(item);
			const uint32_t idx = hash_index(hsh);
			const __m256i msk = make_mask(hsh);
			const __m256i buc = reinterpret_cast<const __m256i*>(buckets)[idx];
			return _mm256_testc_si256(buc, msk);
		};

	private:

		static inline __m256i make_mask(const uint32_t hash) {
			const __m256i ones = _mm256_set1_epi32(1);
			const __m256i rehash = _mm256_setr_epi32(0x47b6137bU, 0x44974d91U, 0x8824ad5bU, 0xa2b7289dU, 0x705495c7U, 0x2df1424bU, 0x9efc4947U, 0x5c6bfb31U);
			__m256i hash_data = _mm256_set1_epi32(hash);
			hash_data = _mm256_mullo_epi32(rehash, hash_data);
			hash_data = _mm256_srli_epi32(hash_data, 27);
			return _mm256_sllv_epi32(ones, hash_data);
		};

		alignas(32) buc_t buckets[M]; //alignment is important for AVX2 data

#else  //standard (non-SIMD) version

	public:

		inline void insert(const uint32_t item) noexcept {
			const uint32_t hsh = hash(item);
			const uint32_t idx = hash_index(hsh);
			const uint64_t msk[4] = { 
				make_mask(hsh, 0x47b6137bU, 0x44974d91U), 
				make_mask(hsh, 0x8824ad5bU, 0xa2b7289dU), 
				make_mask(hsh, 0x705495c7U, 0x2df1424bU), 
				make_mask(hsh, 0x9efc4947U, 0x5c6bfb31U)
			};
			lock(idx);
				buckets[idx][0] |= msk[0];
				buckets[idx][1] |= msk[1];
				buckets[idx][2] |= msk[2];
				buckets[idx][3] |= msk[3];
			unlock(idx);
			cnt++;		
		}

		inline bool contains(const uint32_t item) const noexcept {
			const uint32_t hsh = hash(item);
			const uint32_t idx = hash_index(hsh);
			const uint64_t msk[4] = { 
				make_mask(hsh, 0x47b6137bU, 0x44974d91U), 
				make_mask(hsh, 0x8824ad5bU, 0xa2b7289dU), 
				make_mask(hsh, 0x705495c7U, 0x2df1424bU), 
				make_mask(hsh, 0x9efc4947U, 0x5c6bfb31U)
			};
			if((buckets[idx][0] & msk[0]) != msk[0]) return false;
			if((buckets[idx][1] & msk[1]) != msk[1]) return false;
			if((buckets[idx][2] & msk[2]) != msk[2]) return false;
			if((buckets[idx][3] & msk[3]) != msk[3]) return false;
			return true;
		};

	private:

		static inline uint64_t make_mask(const uint32_t hash, const uint32_t c, const uint32_t d) noexcept {
			uint32_t s1 = (c*hash)>>(32-5);
			uint32_t s2 = (d*hash)>>(32-5);
			return (1ULL << (s1+0)) | (1ULL << (s2+32));
		}

		buc_t buckets[M];

#endif

};

#endif