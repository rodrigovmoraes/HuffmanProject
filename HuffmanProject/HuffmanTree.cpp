#include "HuffmanTree.hpp"
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <cstdint>
#include <map>
#include <iomanip>

using namespace std;

long HuffmanTreeNode::getFrequency() const {
	return this->frequency;
}

void HuffmanTreeNode::setFrequency(long frequency) {
	this->frequency = frequency;
}

void HuffmanTreeNode::setSymbol(uint8_t symbol) {
	this->symbol = symbol;
}

uint8_t HuffmanTreeNode::getSymbol() const {
	return this->symbol;
}

void HuffmanTreeNode::setZeroChild(HuffmanTreeNode *zeroChild) {
	this->zeroChild = zeroChild;
}

HuffmanTreeNode *HuffmanTreeNode::getZeroChild() const {
	return this->zeroChild;
}

void HuffmanTreeNode::setOneChild(HuffmanTreeNode *oneChild) {
	this->oneChild = oneChild;
}

HuffmanTreeNode *HuffmanTreeNode::getOneChild() const {
	return this->oneChild;
}

bool HuffmanTreeNodeCompare(HuffmanTreeNode *a, HuffmanTreeNode *b) {
	return a->getFrequency() > b->getFrequency();
}

void HuffmanTreeNode::print() const {
	this->print(0, *this);
}

void HuffmanTreeNode::print(int space, const HuffmanTreeNode &root) const {
	if (root.getZeroChild() != NULL) {
		root.print(space + 4,*root.getZeroChild());
	}
	for(int i = 0; i < space; i++) {
		cout << " ";
	}
	if (root.symbol == 0) {
		cout << "X" << endl;
	} else {
		cout << root.symbol << endl;
	}
	if (root.getOneChild() != NULL) {
		root.print(space + 4,*root.getOneChild());
	}
}

bool HuffmanTreeNode::isLeaf() const {
	return this->getZeroChild() == NULL && this->getOneChild() == NULL;
}

HuffmanTreeNode* HuffmanTreeNode::buildHuffmanTree(vector<HuffmanTreeNode*> &v) {
	make_heap(v.begin(), v.end(), HuffmanTreeNodeCompare);

	while(v.size() != 1) {
		pop_heap(v.begin(), v.end(), HuffmanTreeNodeCompare);
		HuffmanTreeNode* ci = v.back();
		v.pop_back();
		pop_heap(v.begin(), v.end(), HuffmanTreeNodeCompare);
		HuffmanTreeNode* cj = v.back();
		v.pop_back();
		//combine ci and cj
		HuffmanTreeNode *ciCj = new HuffmanTreeNode();
		ciCj->setFrequency(ci->getFrequency() + cj->getFrequency());
		ciCj->setOneChild(ci);
		ciCj->setZeroChild(cj);
		//add ciCj to the heap
		v.push_back(ciCj);
		push_heap(v.begin(), v.end(), HuffmanTreeNodeCompare);
	}

	HuffmanTreeNode* root = v.back();
	return root;
}

void HuffmanTreeNode::countNodes(HuffmanTreeNode* node, long &count) {
	count++;
	if (node->zeroChild != NULL) {
		node->countNodes(node->zeroChild, count);
	}
	if (node->oneChild != NULL) {
		node->countNodes(node->oneChild, count);
	}
}

long HuffmanTreeNode::countNodes() {
	long count = 0;
	this->countNodes(this, count);
	return count;
}

void HuffmanTreeNode::save(fstream &file) {
	//get the number of nodes
	long amountOfNodes = this->countNodes();
	long sizeOfHuffmanTreeInBytes = amountOfNodes * sizeof(nodeInFile);
	file.write(reinterpret_cast<char*>(&sizeOfHuffmanTreeInBytes), sizeof(long));
	this->saveNode(file, 0, 0, true);
}

void HuffmanTreeNode::saveNode(fstream &file, ostream::pos_type parentAddress, uint8_t zeroOrOne, bool isRoot) {
	ostream::pos_type nextParentAddress = file.tellp();
	if (isRoot) {
		nodeInFile *rootNode = (nodeInFile *) malloc(sizeof(nodeInFile));
		rootNode->parent = 0;
		rootNode->symbol = 0;
		rootNode->zeroOrOne = 0;
		file.write(reinterpret_cast<char*>(rootNode), sizeof(nodeInFile));
		if (this->getOneChild() != NULL) {
			this->getOneChild()->saveNode(file, nextParentAddress, 1, false);
		}
		if (this->getZeroChild()) {
			this->getZeroChild()->saveNode(file, nextParentAddress, 0, false);
		}
		free(rootNode);
	} else {
		nodeInFile *node = (nodeInFile *) malloc(sizeof(nodeInFile));
		node->parent = parentAddress;
		node->symbol = this->symbol;
		node->zeroOrOne = zeroOrOne;
		file.write(reinterpret_cast<char*>(node), sizeof(nodeInFile));
		if (this->getOneChild() != NULL) {
			this->getOneChild()->saveNode(file, nextParentAddress, 1, false);
		}
		if (this->getZeroChild() != NULL) {
			this->getZeroChild()->saveNode(file, nextParentAddress, 0, false);
		}
		free(node);
	}
}

HuffmanTreeNode* HuffmanTreeNode::read(fstream &file) {
	map<ostream::pos_type, HuffmanTreeNode*> huffmanNodes;
	long treeSizeInBytes;
	file.read(reinterpret_cast<char*>(&treeSizeInBytes), sizeof(long));
	long amountOfNodes = treeSizeInBytes / sizeof(nodeInFile);
	//construct huffman nodes map
	for (long i = 0; i < amountOfNodes; i++) {
		ostream::pos_type address = file.tellp();
		nodeInFile *node = (nodeInFile *) malloc(sizeof(nodeInFile));
		file.read(reinterpret_cast<char*>(node), sizeof(nodeInFile));
		HuffmanTreeNode* huffmanTreeNode = new HuffmanTreeNode();
		huffmanTreeNode->setSymbol(node->symbol);
		huffmanNodes.insert({ address, huffmanTreeNode });
		free(node);
	}
	file.seekp(sizeof(long));
	//construct the huffman tree
	HuffmanTreeNode* rootNode = NULL;
	for (long i = 0; i < amountOfNodes; i++) {
		nodeInFile *node = (nodeInFile *) malloc(sizeof(nodeInFile));
		ostream::pos_type address = file.tellp();
		file.read(reinterpret_cast<char*>(node), sizeof(nodeInFile));
		if (node->parent == 0) { //is root
			rootNode = huffmanNodes[sizeof(long)];
		} else {
			HuffmanTreeNode* huffmanTreeNode = huffmanNodes[address];
			HuffmanTreeNode* parentHuffmanTreeNode = huffmanNodes[node->parent];
			if (node->zeroOrOne == 0) {
				parentHuffmanTreeNode->setZeroChild(huffmanTreeNode);
			} else if (node->zeroOrOne == 1) {
				parentHuffmanTreeNode->setOneChild(huffmanTreeNode);
			}
		}
		free(node);
	}
	return rootNode;
}

void HuffmanTreeNode::getMapOfCodes(map<uint8_t, vector<uint8_t>> &map, vector<uint8_t> code) {
	if (this->getZeroChild() != NULL) {
		vector<uint8_t> copyCode = code;
		copyCode.push_back(0);
		this->getZeroChild()->getMapOfCodes(map, copyCode);
	}
	if (this->getOneChild() != NULL) {
		vector<uint8_t> copyCode = code;
		copyCode.push_back(1);
		this->getOneChild()->getMapOfCodes(map, copyCode);
	}
	if (this->isLeaf()) {
		map.insert({ this->symbol, code });
	}
}

typedef map<uint8_t, vector<uint8_t>> mapUint8_tVectorOfUint8_t;

map<uint8_t, vector<uint8_t>> &HuffmanTreeNode::getMapOfCodes() {
	map<uint8_t, vector<uint8_t>> *map = new mapUint8_tVectorOfUint8_t();
	vector<uint8_t> code;
	this->getMapOfCodes(*map, code);
	return *map;
}


