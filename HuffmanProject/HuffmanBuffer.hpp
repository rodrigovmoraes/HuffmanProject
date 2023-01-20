#ifndef _HUFFMAN_BUFFER_
#define _HUFFMAN_BUFFER_

#include <vector>
#include <cstdint>
#include <stdexcept>

using namespace std;

class HuffmanBuffer {
	private:
		vector<uint8_t> buffer;
		bool _hasBits = false;
	public:
		bool isFull();
		uint8_t getContent();
		void addBit(uint8_t bit);
		bool hasBits();
};

#endif
