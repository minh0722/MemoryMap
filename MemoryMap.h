#pragma once

#include <cstdio>
#include <utility>

#define PAGE_SIZE (4096)	//100 B
#define PAGE_COUNT 8

struct Page {
	size_t from, to;
	char* buffer;
	size_t usage;

	size_t getBufferSize()
	{
		return to - from + 1;
	}
};

class MemoryMap {
public:
	MemoryMap(const char* file_path);
	~MemoryMap();

	char& operator[](const size_t index);

	size_t fileSize();
	size_t getFileSize();
private:
	/* allocate memory for page and get file content
	**/
	void allocatePage(size_t page_index, std::pair<size_t, size_t>& pageRange);


	/* get index of page that contain byte at index
	**/
	int getPageOnIndex(size_t byteIndex);

	void writePageToDisk(size_t pageIndex);
	void deallocatePage(size_t pageIndex);

	/* return range of a page in the file for the byte index
	**/
	std::pair<size_t, size_t> getPageRange(size_t byteIndex);

	void sortPagesByUsage();

	size_t usage[PAGE_COUNT];
	Page m_map[PAGE_COUNT];
	FILE* m_file;
	size_t file_size;
};
