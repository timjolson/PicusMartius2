#ifndef _UTIL_H
#define _UTIL_H

#include "./bitset.h"
#include "./queue.h"

namespace util {
	template<class T>
	void swap(T& a, T& b) {
		T c = a;
		a = b;
		b = c;
	}
}

#endif
