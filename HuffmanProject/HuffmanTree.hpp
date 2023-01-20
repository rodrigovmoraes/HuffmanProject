#ifndef _HUFFMAN_TREE_
#define _HUFFMAN_TREE_

#include <cstdint>
#include <cstddef>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

	class HuffmanTreeNode {
		private:
			long frequency = 0;
			uint8_t symbol = 0;
			HuffmanTreeNode *zeroChild = NULL;
			HuffmanTreeNode *oneChild = NULL;
			void print(int space, const HuffmanTreeNode &root) const;
			void countNodes(HuffmanTreeNode *node, long &count);
			void saveNode(fstream &file, ostream::pos_type parentAddress, uint8_t zeroOrOne, bool isRoot);
			void getMapOfCodes(map<uint8_t, vector<uint8_t>> &map, vector<uint8_t> code);
		public:
			long getFrequency() const;
			void setFrequency(long frequency);
			uint8_t getSymbol() const;
			void setSymbol(uint8_t symbol);
			void setZeroChild(HuffmanTreeNode *zeroChild);
			HuffmanTreeNode *getZeroChild() const;
			void setOneChild(HuffmanTreeNode *oneChild);
			HuffmanTreeNode *getOneChild() const;
			bool isLeaf() const;
			void print() const;
			void save(fstream &file);
			long countNodes();
			map<uint8_t, vector<uint8_t>> &getMapOfCodes();
			static HuffmanTreeNode* read(fstream &file);
			static HuffmanTreeNode* buildHuffmanTree(vector<HuffmanTreeNode*> &v);
	};

	bool HuffmanTreeNodeCompare(HuffmanTreeNode *a, HuffmanTreeNode *b);

	typedef struct structNodeInFile {
		uint8_t symbol;
		size_t parent;
		uint8_t zeroOrOne;
	} nodeInFile;

#endif
