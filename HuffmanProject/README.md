## An implementation of a file compressor utilizing the Huffman algorithm.

### Compilation

Use Eclipse IDE. Checkout the project from Eclipse IDE and compile it. The project use C++20 and C++17 features, then you will have to have a C++ compiler with support for C++20 and C++17. For this purpose, follow the instructions in (https://gasparri.org/2020/07/30/installing-c17-and-c20-on-ubuntu-and-amazon-linux/). After installing C++ and C compilers, you will have to update the link of gcc and g++ to point to gcc-10 and g++-10 respectively in /usr/bin folder. For this purpose, execute the following commands where the gcc and g++ are located, usually in /usr/bin:

`rm gcc` <br>
`ln -s gcc-10 gcc`  <br>
`rm g++`  <br>
`ln -s g++-10 g++`  <br>

### Run

#### For compress a file use:

`./huffman -c <filename>`

The executable will create a compressed file with the name of the original file name concatenated with the suffix ".huff"

#### For decompress a file

`./huffman -d <filename>`

The executable will restore the compressed file to original content with the name of the file with the suffix ".huff" removed.
