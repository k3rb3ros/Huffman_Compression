/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#ifndef HEADER_H
#define HEADER_H

#include "charlist.h"
#include "headers.h"
#include "huffman_trie.h"
#include "structs.h"

class Huffman : public Trie //Huffman is a CharList and Trie
{
	private:
	//CharList* c_list;
	int magic_number;
	string file_to_compress;
	string header_table;
	string encoded_text;
	char end_of_text_marker;
	char end_of_file;
	void getFileName();
	
	public:
	Huffman();
	void populateHeader(string hfile,string fname);
	~Huffman();
};
#endif