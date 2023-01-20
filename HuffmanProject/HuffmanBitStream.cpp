#include "HuffmanBitStream.hpp"
#include <vector>
#include <cstdint>

HuffmanBitStream::HuffmanBitStream(fstream *content) {
	this->content = content;
	uint8_t currentByte = 0;
	if (content->read(reinterpret_cast<char*>(&currentByte), sizeof(uint8_t))) {
		this->currentByte = currentByte;
		this->bitIndex = 0;
		this->_eof = false;
	} else {
		this->currentByte = 0;
		this->bitIndex = 0;
		this->_eof = true;
	}
}

uint8_t HuffmanBitStream::getNextBit() {
	uint8_t r = ( this->currentByte & (0b00000001 << this->bitIndex) ) >> this->bitIndex;
	this->bitIndex++;
	if (this->bitIndex >= 8) {//end of byte
		//get next byte
		uint8_t currentByte = 0;
		if (content->read(reinterpret_cast<char*>(&currentByte), sizeof(uint8_t))) {
			this->currentByte = currentByte;
			this->bitIndex = 0;
		} else {
			this->currentByte = 0;
			this->bitIndex = 0;
			this->_eof = true;
		}
	}
	return r;
}

bool HuffmanBitStream::eof() {
	return this->_eof;
}
