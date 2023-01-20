#include "HuffmanBuffer.hpp"

#include <vector>
#include <cstdint>
#include <stdexcept>

using namespace std;

uint8_t HuffmanBuffer::getContent() {
	uint8_t value = 0b00000000;
	int index = 0;
	for (const auto &bit : this->buffer) {
		if (bit == 0) {
			uint8_t mask = ~(0b00000001 << index);
			value &= mask;
		} else if (bit == 1) {
			uint8_t mask = 0b00000001 << index;
			value |= mask;
		}
		index++;
	}
	this->buffer.clear();
	this->_hasBits = false;
	return value;
}

bool HuffmanBuffer::isFull() {
	return this->buffer.size() == 8;
}

void HuffmanBuffer::addBit(uint8_t bit) {
	this->_hasBits = true;
	if (this->buffer.size() < 8) {
		this->buffer.push_back(bit);
	} else {
		 throw runtime_error("Huffman buffer is full\n");
	}
}

bool HuffmanBuffer::hasBits() {
	return this->_hasBits;
}
