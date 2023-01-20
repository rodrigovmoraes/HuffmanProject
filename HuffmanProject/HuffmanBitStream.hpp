#ifndef _HUFFMAN_BIT_STREAM_
#define _HUFFMAN_BIT_STREAM_

#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class HuffmanBitStream {
	private:
		fstream *content;
		unsigned long bitIndex;
		bool _eof = false;
		uint8_t currentByte;
	public:
		HuffmanBitStream(fstream *content);
		uint8_t getNextBit();
		bool eof();
};

#endif
