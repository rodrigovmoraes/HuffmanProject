#include "HuffmanFacade.hpp"
#include "HuffmanBitStream.hpp"
#include "HuffmanTree.hpp"
#include "HuffmanBuffer.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <cstdint>
#include <istream>
#include <sstream>

void HuffmanFacade::decodeFromFile(fstream &encodedContent, fstream &fileOut) {
	HuffmanTreeNode* huffmanTree = HuffmanTreeNode::read(encodedContent);
	long size;
	encodedContent.read(reinterpret_cast<char*>(&size), sizeof(long));
	HuffmanBitStream huffmanBitStream(&encodedContent);
	for (long i = 0; i < size && !huffmanBitStream.eof(); i++) {
		HuffmanTreeNode *currentNode = huffmanTree;
		while(!huffmanBitStream.eof() && !currentNode->isLeaf()) {
			uint8_t nextBit = huffmanBitStream.getNextBit();
			if (nextBit == 0) {
				currentNode = currentNode->getZeroChild();
			} else if (nextBit == 1) {
				currentNode = currentNode->getOneChild();
			}
		}
		if (currentNode->isLeaf()) {
			uint8_t currentByte = currentNode->getSymbol();
			fileOut.write(reinterpret_cast<char*>(&currentByte), sizeof(uint8_t));
		}
	}
}

vector<HuffmanTreeNode*> &HuffmanFacade::computeFrequencies(fstream &file, long &totalBytes) {
	map<uint8_t, long> frequenciesMap;
	uint8_t currentByte = 0;
	while(file.read(reinterpret_cast<char*>(&currentByte), sizeof(uint8_t))) {
		totalBytes++;
		if (frequenciesMap.contains(currentByte)) {
			frequenciesMap[currentByte] = frequenciesMap[currentByte] + 1;
		} else {
			frequenciesMap.insert({currentByte, 1});
		}
	}
	vector<HuffmanTreeNode*> *r = new vector<HuffmanTreeNode*>();
	for (const auto& [byte, frequency] : frequenciesMap) {
		HuffmanTreeNode *huffmanTreeNode = new HuffmanTreeNode();
		huffmanTreeNode->setSymbol(byte);
		huffmanTreeNode->setFrequency(frequency);
		r->push_back(huffmanTreeNode);
	}
	return *r;
}

void HuffmanFacade::encodingToFile(fstream &fileIn, fstream &fileOut) {
	long totalBytes = 0;
	vector<HuffmanTreeNode*> initialHuffmanNodes = HuffmanFacade::computeFrequencies(fileIn, totalBytes);
	HuffmanTreeNode *huffmanTreeNode = HuffmanTreeNode::buildHuffmanTree(initialHuffmanNodes);
	huffmanTreeNode->save(fileOut);
	fileOut.write(reinterpret_cast<char*>(&totalBytes), sizeof(long));
	//encode the content
	map<uint8_t, vector<uint8_t>> mapCodes = huffmanTreeNode->getMapOfCodes();
	fileIn.clear();
	fileIn.seekg(0);
	HuffmanBuffer huffmanBuffer;
	uint8_t currentByte = 0;
	while(fileIn.read(reinterpret_cast<char*>(&currentByte), sizeof(uint8_t))) {
		if (mapCodes.contains(currentByte)) {
			vector<uint8_t> bits = mapCodes[currentByte];
			for (uint8_t bit : bits) {
				huffmanBuffer.addBit(bit);
				if (huffmanBuffer.isFull()) {
					uint8_t byteContent = huffmanBuffer.getContent();
					fileOut.write(reinterpret_cast<char*>(&byteContent), sizeof(uint8_t));
				}
			}
		} else {
			string runtimeErrorMessage = "It doesn't have a code for the character ";
			runtimeErrorMessage.push_back((const unsigned char)currentByte);
			runtimeErrorMessage.append("\n");
			throw runtime_error(runtimeErrorMessage.c_str());
		}
	}
	if (huffmanBuffer.hasBits()) {
		uint8_t byteContent = huffmanBuffer.getContent();
		fileOut.write(reinterpret_cast<char*>(&byteContent), sizeof(uint8_t));
	}
}
