#include <iostream>
#include "MemoryMap.h"

#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]) {

	MemoryMap m(argv[1]);

	auto t1 = high_resolution_clock::now();
	for (size_t i = 0; i < m.getFileSize(); ++i) {
		m[i] = *argv[2];
	}

	auto t2 = high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	cout << "miliseconds: " << duration << endl;

	//cin.get();
	//86,621,800
	return 0;
}
