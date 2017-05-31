#include "Arduino.h"

#ifndef CALLABLE_H
#define CALLABLE_H


class Callable {
	private:
	public:
		virtual bool call(int methodNum, int val) = 0;
};

#endif