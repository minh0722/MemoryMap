#include <iostream>
#include "MemoryMap.h"

int main() {

	MemoryMap m("sample.txt");

	m[0] = 'q';

	return 0;
}
