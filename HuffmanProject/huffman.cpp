#include "HuffmanFacade.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <string.h>

using namespace std;

void printUse() {
	cout << "Use: huffman -c <filename> or huffman -d <filename>" << endl;
	exit(1);
}

int main(int argc, char **argv) {
	if (argc == 3) {
		if (strcmp(argv[1], "-c") == 0) {
			fstream fileIn;
			fstream fileOut;
			fileIn.open(argv[2], ios::binary | ios::in);
			//create a string with the extension .huff appended to
			//original file name
			string fileOutName(argv[2]);
			fileOutName.append(".huff");
			//open the file output
			fileOut.open(fileOutName, ios::binary | ios::out);
			HuffmanFacade::encodingToFile(fileIn, fileOut);
			fileIn.close();
			fileOut.close();
		} else if (strcmp(argv[1], "-d") == 0) {
			string fileInName(argv[2]);
			string fileOutName;
			if (fileInName.ends_with(".huff")) {
				fileOutName = fileInName.substr(0, fileInName.size() - 5);
				fstream fileIn;
				fstream fileOut;
				fileIn.open(fileInName, ios::binary | ios::in);
				fileOut.open(fileOutName, ios::binary | ios::out);
				HuffmanFacade::decodeFromFile(fileIn, fileOut);
				fileIn.close();
				fileOut.close();
			} else {
				cout << "The input file name must terminate with the extension .huff" << endl;
				exit(1);
			}
		} else {
			printUse();
		}
	} else {
		printUse();
	}
}
