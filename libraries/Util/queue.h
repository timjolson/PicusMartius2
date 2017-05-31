#ifndef _UTIL_QUEUE_H
#define _UTIL_QUEUE_H

#include <cstddef>

namespace util {
	template<class Type, uint8_t Limit>
	class Queue {
		public:
			Queue() {}
			
			uint8_t push(const Type*, uint8_t count);
			bool push(const Type&);
			
			Type pop();
			const Type& peek() const;
			void remove();
			void clear();
			
			uint8_t size() const;
			uint8_t limit() const {
				return Limit;
			}
		
		private:
			Type _queue[Limit];
			uint8_t _size = 0;
			uint8_t _start = 0;
			
			inline uint8_t calc_index(uint16_t start, uint16_t offset) {
				return (uint8_t)((start + offset) % Limit);
			}
	};
	
	template<class Type, uint8_t Limit>
	uint8_t Queue<Type, Limit>::push(const Type* items, uint8_t count) {
		uint8_t i = 0;
		for (; i < count; i++) {
			if (!this->push(items[i])) {
				break;
			}
		}
		return i;
	}
	
	template<class Type, uint8_t Limit>
	bool Queue<Type, Limit>::push(const Type& item) {
		if (this->_size >= Limit) {
			return false;
		}
		
		this->_queue[this->calc_index(this->_start, this->_size)] = item;
		this->_size++;
		return true;
	}
	
	template<class Type, uint8_t Limit>
	Type Queue<Type, Limit>::pop() {
		const Type& item = this->peek();
		this->remove();
		return item;
	}
	
	template<class Type, uint8_t Limit>
	const Type& Queue<Type, Limit>::peek() const {
		return this->_queue[this->_start];
	}
	
	template<class Type, uint8_t Limit>
	void Queue<Type, Limit>::remove() {
		if (this->_size) {
			this->_start = this->calc_index(this->_start, 1);
			this->_size--;
		}
	}
	
	template<class Type, uint8_t Limit>
	void Queue<Type, Limit>::clear() {
		this->_size = 0;
	}
	
	template<class Type, uint8_t Limit>
	uint8_t Queue<Type, Limit>::size() const {
		return this->_size;
	}
}

#endif
