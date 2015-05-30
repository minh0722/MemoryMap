#include "MemoryMap.h"
#include <stdexcept>
#include <algorithm>


MemoryMap::MemoryMap(const char* file_path) {

	m_file = fopen(file_path, "rb+");

	if (!m_file)	{
		throw std::exception("Can't open file");
	}

	memset(m_map, 0, PAGE_COUNT * sizeof(Page));

	for (size_t i = 0; i < PAGE_COUNT; ++i) {
		usage[i] = i;
	}

}


MemoryMap::~MemoryMap() {
	for (size_t i = 0; i < PAGE_COUNT; ++i) {
		writePageToDisk(i);
	}

	deallocateAllPages();
	fclose(m_file);
}

char& MemoryMap::operator[](const size_t byte_index) {
	if (byte_index > fileSize()) {
		throw std::out_of_range("Index out of range");
	}
	
	int page_index = getPageOnIndex(byte_index);
	++m_map[page_index].usage;

	sortPagesByUsage();

	return m_map[page_index].buffer[byte_index - m_map[page_index].from];
}





int MemoryMap::getPageOnIndex(size_t byteIndex) {
	//existing page
	for (int i = 0; i < PAGE_COUNT; ++i) {
		if (m_map[i].from <= byteIndex && byteIndex <= m_map[i].to && m_map[i].buffer) {
			return i;
		}
	}

	//free page
	for (int i = 0; i < PAGE_COUNT; ++i) {
		if (m_map[i].from == 0 && m_map[i].to == 0 && !m_map[i].buffer)	{
			allocatePage(i, getPageRange(byteIndex));
			return i;
		}
	}

	writePageToDisk(0);
	deallocatePage(0);
	allocatePage(0, getPageRange(byteIndex));
	return 0;
}

std::pair<size_t, size_t> MemoryMap::getPageRange(size_t byteIndex) {
	size_t file_size = fileSize();

	size_t from = (byteIndex / PAGE_SIZE) * PAGE_SIZE;
	size_t to = file_size > from + PAGE_SIZE - 1 ? from + PAGE_SIZE - 1 : file_size  - 1;

	return std::make_pair(from, to);
}

void MemoryMap::writePageToDisk(size_t pageIndex) {
	if (m_map[pageIndex].buffer) {
		fseek(m_file, m_map[pageIndex].from, SEEK_SET);
		fwrite(m_map[pageIndex].buffer, sizeof(char), m_map[pageIndex].getBufferSize(), m_file);
	}
}


void MemoryMap::allocatePage(size_t page_index, std::pair<size_t, size_t>& pageRange) {
	m_map[page_index].from = pageRange.first;
	m_map[page_index].to = pageRange.second;

	m_map[page_index].buffer = new char[m_map[page_index].getBufferSize()];
	fseek(m_file, pageRange.first, SEEK_SET);
	fread(m_map[page_index].buffer, sizeof(char), m_map[page_index].getBufferSize(), m_file);
}

void MemoryMap::sortPagesByUsage() {
	std::sort(
		usage, 
		usage + PAGE_COUNT, 
		[this](const int& l, const int& r){
			return this->m_map[l].usage < this->m_map[r].usage;
		}
	);
}

size_t MemoryMap::fileSize() {
	size_t currPosition = ftell(m_file);
	fseek(m_file, 0, SEEK_END);
	size_t size = ftell(m_file);
	fseek(m_file, currPosition, SEEK_SET);
	return size;
}

void MemoryMap::deallocateAllPages() {
	for (size_t i = 0; i < PAGE_COUNT; ++i) {
		deallocatePage(i);
	}
}

void MemoryMap::deallocatePage(size_t pageIndex) {
	delete[] m_map[pageIndex].buffer;
	memset(&m_map[pageIndex], 0, sizeof(Page));
}
