/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../headers/charlist.h"
#include "../headers/headers.h"
#include "../headers/huffman_trie.h"

int main(int argv, char **argc){
	
	CharList c;
	string fname = "../text/us_constitution.txt";
	//Trie huffman;

	c.bufferFile(fname);	
    c.populateTable();
	c.showCharCount();
	//cout << "Constructed and inited\n";
	//cout << "Nodes: " << huffman.size_of_trie() << endl;
	//cout << "Characters: " << huffman.character_count() << endl;
	while(getchar() != '\n');
	return 0;
}
