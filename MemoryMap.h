#pragma once

#include <cstdio>
#include <utility>

#define PAGE_SIZE (10 * 10)	//100 B
#define PAGE_COUNT 10

struct Page {
	size_t from, to;
	char* buffer;

	size_t getBufferSize() {
		return to - from + 1;
	}
};

class MemoryMap {
public:
	MemoryMap(const char* file_path);
	~MemoryMap();

	char& operator[](const size_t index);

private:
	void allocatePage(size_t page_index, std::pair<size_t,size_t>& pageRange);
	
	size_t fileSize();

	/* return page index
	**/
	int getPageOnIndex(size_t byteIndex);

	void writePageToDisk(size_t pageIndex);
	void deallocateAllPages();
	void deallocatePage(size_t pageIndex);

	/* return range of a page in the file for the byte index
	**/
	std::pair<size_t, size_t> getPageRange(size_t byteIndex);

	Page m_map[PAGE_COUNT];
	FILE* m_file;
};

