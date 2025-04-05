// (C) Copyright by Stephan Theisgen
//
// This is a quick and dirty implementation of a Golomb/Rice stream.
// It allows to store a stream of strictly increasing values efficiently,
// if the values follow a Geometric distribution (see https://en.wikipedia.org/wiki/Golomb_coding);
// which is the case for edges that need to be stored using the splean miner approach

#ifndef RICE_HPP
#define RICE_HPP

#include <cstdint>
#include <cstdio>
#include <cstring>

//define version of ctz (count trailing zeros)
#ifdef __GNUC__
	#define ctz(x) __builtin_ctzll(x)
#elif defined _MSC_VER
	#include <intrin.h>
	inline int ctz(uint64_t x) {
		unsigned long p = 0;
		_BitScanForward64(&p, x)
		return static_cast<int>(p);
	}
#endif

template<uint32_t SIZE, uint32_t B, typename T = uint64_t> //SIZE in bits (minimum is 32), B must not be >31
class rice_stream {

	public:

		rice_stream() { clear(); };

		void write(const uint32_t value) noexcept { //writes a new value to the stream, value needs to be strictly increasing with each call of this function
			//function assumes that full memory is cleared to 0 (call clear before reusing the stream for writing)
			if(pos == 0) { //handling start case
				bits[0] = value;
				pos += 32;
			} else {
				uint32_t x = value - last - 1; //get difference to previous value/item
				uint32_t q = x >> B; //calculate Rice values from Rice parameter b
				uint32_t r = x & R;
				if((pos + (q+1+B)) >= (WORDS*BW)) { //checks if there is still enough memory left
					printf("PANIC: Write overflow in Rice stream (%u vs. %u, with rice parameter: q=%u, b=%u) !\n", pos, WORDS*BW, q, B);
					return;
				}
				write_unary(q); //writes q times 0
				write_binary(r); //writes separator 1 bit, and r as B bit binary
			}
			cnt++; //update state
			len = pos;
			last = value;
		};

		bool read(uint32_t& value) noexcept { //reads next value in stream, returns if stream still has more values
			if(pos >= len) return false; //prevent reading after end of stream
			if(pos == 0) { //handling start case
				last = bits[0];
				pos += 32;
			} else {
				uint32_t q = read_unary(); //reads q as amount of 0 bits until separator 1 bit is found
				uint32_t r = read_binary(); //reads r as B bit binary
				uint32_t x = (q << B) | r; //calculate value
				last += x + 1;
			}
			value = last; //update state
			return true;
		}

		void clear() {
	    	memset(bits, 0, WORDS*sizeof(T));
			pos = 0; //reset state
			cnt = 0;
			len = 0;
		};

		void reset() noexcept { pos = 0; }; //resets stream position (start from beginning again)

		uint32_t count() const noexcept { return cnt; }; //items in stream
		double usage() const noexcept { return (double)len/(WORDS*BW); }; //how much memory is used

		static uint32_t size() noexcept { return WORDS*BW; }; //size in bits

	private:

		static const uint8_t BW = (sizeof(T)*8); //size of a word in bits
		static const uint32_t R = ((1UL<<B)-1);	//mask for B bits binary part
		static const uint32_t WORDS = (SIZE+(BW-1))/BW; //amount of words

		inline void write_unary(const uint32_t q) noexcept { pos += q; }; //write q times a 0 bit (just advance state, memory is assumed to be cleared to zero)
 
 		inline void write_binary(T r) noexcept { //function updates position in stream, type T is important here (because of shift later),
			uint32_t x = pos / BW;
			uint32_t y = pos % BW;
			r = (r << 1) | 1; //add separator 1 bit
			bits[x] |= (r << y); //write r value and...
			if((y+(B+1)) > BW) bits[x+1] |= (r >> (BW-y)); //handle case if we go over word boundary
			pos += (B+1); //advance position
		};

		inline uint32_t read_unary() noexcept { //function updates position in stream
			uint32_t q = 0;
			uint32_t x = pos / BW;
			uint32_t y = pos % BW;
			T tmp = bits[x]>>y; //ignore already read bits of this word
			if(!tmp) { //if the rest of this word is all zeros
				q += (BW-y); //increase q by this amount, and scan until separator 1 bit is found (so a non-zero word)...
				while(!bits[++x]) q += BW; //step over all words which are fully zero, increase q accordingly
				tmp = bits[x]; //now tmp contains current word (which has not all zeros, so must contain separator 1 bit)
			}
			q += ctz(tmp); //count trailing zeros (until separator 1 bit) and update q
			pos += (q+1); //step over separator 1 bit and update position
			return q;
		};

		inline uint32_t read_binary() noexcept { //function updates position in stream
			uint32_t x = pos / BW;
			uint32_t y = pos % BW;
			uint32_t r = ((bits[x] >> y) & R); //read r value and...
			if((y+B) > BW) r |= ((bits[x+1] & ((1ULL<<((y+B)-BW))-1)) << (BW-y)); //handle case if it is split over word boundary
			pos += B;
			return r;
		};

		T bits[WORDS]; //memory in words of type T
		uint32_t pos; //position in stream in bits
		uint32_t cnt; //number of items stored
		uint32_t last; //last value (to calculate delta)
		uint32_t len; //total length of stream in bits
};

#endif