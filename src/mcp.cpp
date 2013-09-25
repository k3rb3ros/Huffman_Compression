/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../headers/charlist.h"
#include "../headers/headers.h"
#include "../headers/huffman_trie.h"

int main(int argv, char **argc)
{	
	string fname = "../text/us_constitution.txt";
	Trie huffman;
	//example compress
	
	huffman.bufferFile(fname);
	huffman.populateTable();
	huffman.showCharCount();
	huffman.sortTable();
	huffman.populate_trie();
	cout << endl;
	//cout << "Nodes: " << huffman.size_of_trie() << endl;
	//cout << "Characters: " << huffman.character_count() << endl;
	huffman.get_encoding();
	huffman.print_encoding_table();
	while(getchar() != '\n');
	return 0;
}
