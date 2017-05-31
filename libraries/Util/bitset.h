#ifndef _UTIL_BITSET_H
#define _UTIL_BITSET_H

#include <stdint.h>
#include <cstddef>

namespace util {
	template<size_t BitCount>
	class Bitset {
		public:
			static const size_t ByteCount = 1 + ((BitCount - 1) / 8);
			
			Bitset();
			Bitset(unsigned long long val);
			
			size_t count() const;
			size_t size() const { return BitCount; }
			bool test(size_t pos) const;
			bool any() const;
			bool none() const;
			bool all() const;
			
			Bitset<BitCount>& set();
			Bitset<BitCount>& set(size_t pos, bool val = true);
			Bitset<BitCount>& reset();
			Bitset<BitCount>& reset(size_t pos) { return this->set(pos, false); }
			Bitset<BitCount>& flip();
			Bitset<BitCount>& flip(size_t pos);
			
			
		
		private:
			uint8_t _bytes[ByteCount];
	};

	template<size_t BitCount>
	Bitset<BitCount>::Bitset() {
		this->reset();
	}

	template<size_t BitCount>
	Bitset<BitCount>::Bitset(unsigned long long val) {
		this->reset();
		for (size_t i = 0; i < BitCount && i < sizeof(val) && val;
				++i, val >>= 1) {
			this->set(i, val & 0x01);
		}
	}

	template<size_t BitCount>
	size_t Bitset<BitCount>::count() const {
		size_t count = 0;
		for (size_t i = 0; i < BitCount; ++i) {
			if (this->test(i)) {
				count++;
			}
		}
		return count;
	}

	template<size_t BitCount>
	bool Bitset<BitCount>::test(size_t pos) const {
		size_t byte_num = pos / 8;
		uint8_t bit_num = pos % 8;
		return (this->_bytes[byte_num] & (1 << bit_num)) > 0;
	}

	template<size_t BitCount>
	bool Bitset<BitCount>::any() const {
		for (size_t i = 0; i < BitCount; ++i) {
			if (this->test(i)) {
				return true;
			}
		}
		return false;
	}

	template<size_t BitCount>
	bool Bitset<BitCount>::none() const {
		for (size_t i = 0; i < BitCount; ++i) {
			if (this->test(i)) {
				return false;
			}
		}
		return true;
	}

	template<size_t BitCount>
	bool Bitset<BitCount>::all() const {
		for (size_t i = 0; i < BitCount; ++i) {
			if (!this->test(i)) {
				return false;
			}
		}
		return true;
	}

	template<size_t BitCount>
	Bitset<BitCount>& Bitset<BitCount>::set() {
		for (size_t i = 0; i < ByteCount; ++i) {
			this->_bytes[i] = 0xff;
		}
		return *this;
	}

	template<size_t BitCount>
	Bitset<BitCount>& Bitset<BitCount>::set(size_t pos, bool val) {
		size_t byte_num = pos / 8;
		uint8_t bit_num = pos % 8;
		if (val) {
			this->_bytes[byte_num] |= (1 << bit_num);
		} else {
			this->_bytes[byte_num] &= ~(1 << bit_num);
		}
		return *this;
	}

	template<size_t BitCount>
	Bitset<BitCount>& Bitset<BitCount>::reset() {
		for (size_t i = 0; i < ByteCount; ++i) {
			this->_bytes[i] = 0;
		}
		return *this;
	}

	template<size_t BitCount>
	Bitset<BitCount>& Bitset<BitCount>::flip() {
		for (size_t i = 0; i < ByteCount; ++i) {
			this->_bytes[i] ^= 0xff;
		}
		return *this;
	}

	template<size_t BitCount>
	Bitset<BitCount>& Bitset<BitCount>::flip(size_t pos) {
		this->set(pos, !this->test(pos));
		return *this;
	}
}
	
#endif
