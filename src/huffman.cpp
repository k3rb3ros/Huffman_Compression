#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "charlist.h"
#include "huffman_trie.cpp"
 
int main(int argv, char **argc){
	string fname= "constitution.txt";
	CharList c;
	Trie test;
	vector<CharNode*> frequencyTable;
	c.initBuffer(fname);	
    c.populateTable(frequencyTable);

	cout << "Constructed and inited\n";
	cout << "Nodes: " << test.size_of_trie() << endl;
	cout << "Characters: " << test.character_count() << endl;
    std::string temp;
	std::cin >> temp;
	return 0;
}