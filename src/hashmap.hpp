// (C) Copyright by Stephan Theisgen
//
// This is a quick and dirty implementation of a simple hash-based multimap
// with linear probing as open addressing method.
// This hashmap is not thread-safe by itself. Thread-safety is obtained by 
// allowing only one defined thread to insert items and all insertions are
// completed before the structure is read by multiple threads at once.
// Therefore, the constructor and the methods "insert" and "clear" should only be 
// called by a single defined thread.
//
// Items are 64 bit words and keys/values are 32 bit unsigned integers only.
// For efficiency reasons, they are packed into one 64 bit word to form an item.
// However, there is one caveat. A key of 0 that also has a value of 0 is recognized 
// as empty slot. This should be a pretty rare event and is therefore not problematic
// in this context here.
//
// It also allows an amount of slots that is not a power of two. This is crucial for space efficiency.
// For higher speed it avoids a modulo operation to randomly choose a slot, using the approach 
// from here: https://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
// It uses efficient hashing (in hash function) from "High Speed Hashing for Integers and Strings" 
// (by Mikkel Thorup, see https://arxiv.org/pdf/1504.06804).
// Luckily the performance of this hash functions are not too crucial as all values to store come from a
// SipHash24 function, which has already good performance. This allows choosing the fastest hash functions,
// and greatly helps to reach top performance.

#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <cstdint>
#include <cstdio>
#include <cstring>

template<uint32_t SIZE> //SIZE in amount of slots (max. items to store)
class hashmap {

	public:

		hashmap() : a(0xd3d4de51c6bddcab), b(0xf25b261118de7499) { clear(); };  //odd random seeds

		inline bool insert(const uint32_t key, const uint32_t value) noexcept {
			uint32_t i = hash(key);
			for(uint32_t c = 0; c < SIZE; c++) {
				if(!slots[i]) { //empty slot found
					slots[i] = make_item(key, value); //store item
					cnt++;
					return true;
				}
				i++;
				if(i >= SIZE) i = 0; //wrap around
			}
			printf("PANIC: Hashmap full!\n"); //we are full
			return false;
		};

		inline bool get(const uint32_t key, uint32_t& value, uint32_t& pos) const noexcept {
			uint32_t i = hash(key);
			i += pos; //start search relative to position
			for(uint32_t c = 1; c <= (SIZE-pos); c++) { //stop before we reach the first match again, to avoid cycling
				if(i >= SIZE) i -= SIZE; //wrap around
				if(!slots[i]) return false; //we reached an empty slot, so no further occurrences of the key are possible
				if(get_key(slots[i]) == key) { //we found another matching entry
					value = get_value(slots[i]);
					pos += c; //update position relative to start index
					return true;
				}
				i++;
			}
			return false; //the whole structure was searched, no further matches found
		};

		inline void clear() {
			cnt = 0;
	    	memset(slots, 0, SIZE*sizeof(uint64_t));
		};

		double usage() const noexcept { return (double)cnt/SIZE; };
		uint32_t count() const noexcept { return cnt; };
	
		static uint64_t size() noexcept { return sizeof(uint64_t)*8*SIZE; }; //size in bits

	private:

		inline uint32_t hash(const uint32_t x) const noexcept { 
			uint32_t i = (a*x+b)>>32; //see "High Speed Hashing for Integers and Strings" by Mikkel Thorup
			return ( (uint64_t)i * (uint64_t)SIZE ) >> 32; //see "A fast alternative to the modulo reduction" by Daniel Lemire 
		};

		inline static uint64_t make_item(const uint32_t key, const uint32_t value) noexcept { return (((uint64_t)key<<32)|value); }
		inline static uint32_t get_key(const uint64_t item) noexcept { return (item>>32); };
		inline static uint32_t get_value(const uint64_t item) noexcept { return (item & ((1llu<<32)-1)); };

		uint64_t slots[SIZE];
		uint32_t cnt;

		const uint64_t a, b;
};

#endif