/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../headers/charlist.h"
#include "../headers/headers.h"
#include "../headers/huffman.h"
#include "../headers/huffman_trie.h"

int main(int argv, char **argc)
{	
	string fname = "../text/us_constitution.txt";
	string hfile = "header_output_test.txt";
	Huffman huffman;
	//example compress
	huffman.bufferMessage(fname);
	huffman.populateTable();
	//huffman.showCharCount();
	huffman.sortTable();
	//huffman.printSorted();
	huffman.populate_trie();
	huffman.get_encoding();
	//huffman.test();
	//huffman.printBinary();
	//huffman.print_encoding_table();
	huffman.compress();
	//huffman.print_huffman();
	//huffman.populateHeader(fname);
	//huffman.readHeader(hfile);
	huffman.decompress();
	//huffman.printMessageBuffer();
	while(getchar() != '\n');
	return 0;
}
