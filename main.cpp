#include <iostream>
#include "MemoryMap.h"

int main() {

	MemoryMap m("sample.txt");

	m[0] = 'q';

	m[200] = 'g';
	m[233] = 'h';
	m[266] = 'r';
	m[212] = 'l';
	m[256] = 'e';

	m[100] = 't';
	m[133] = 'e';
	m[122] = 'g';

	m[343] = 'b';
	m[342] = 'm';
	m[355] = 'y';
	m[347] = 'g';




	return 0;
}
