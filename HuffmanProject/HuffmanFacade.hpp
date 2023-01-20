#ifndef __HUFFMAN_FACADE__
#define __HUFFMAN_FACADE__

#include "HuffmanTree.hpp"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class HuffmanFacade {
	private:
		static vector<HuffmanTreeNode*> &computeFrequencies(fstream &file, long &totalBytes);
	public:
		static void decodeFromFile(fstream &encodedContent, fstream &fileOut);
		static void encodingToFile(fstream &fileIn, fstream &fileOut);
};

#endif
