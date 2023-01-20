## An implementation of a file compressor utilizing the Huffman algorithm.

### Compilation

Use Eclipse IDE. Checkout the project from Eclipse IDE and compile it.

### Run

#### For compress a file use:

`./huffman -c <filename>`

The executable will create a compressed file with the name of the original file name concatenated with the suffix ".huff"

#### For decompress a file

`./huffman -d <filename>`

The executable will restore the compressed file to original content with the name of the file with the suffix ".huff" removed.
