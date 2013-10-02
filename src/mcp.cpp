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
	//string hfile = "header_output_test.txt";
	Huffman huffman;
	//example compress
	huffman.bufferFile(fname);
	huffman.populateTable();
	//huffman.showCharCount();
	huffman.sortTable();
	//huffman.printSorted();
	huffman.populate_trie();
	cout << "Nodes: " << huffman.size_of_trie() << endl;
	cout << "Characters: " << huffman.character_count() << endl;
	huffman.get_encoding();
	//huffman.print_encoding_table();
	//huffman.test();
	huffman.compress();
	//huffman.print_huffman();
	//huffman.populateHeader(hfile,fname);
	//huffman.print_encoding_table();
	//huffman.printBinary();
	//huffman.populateHeader(hfile,fname);
	//huffman.readHeader(hfile);
	while(getchar() != '\n');
	return 0;
}
