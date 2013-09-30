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

#define MAGIC_NUMBER 0x63130
#define DELIM "\1\27"
#define O_EOF '\26'


class Huffman : public Trie //Huffman is a CharList and Trie
{
	private:
	string file_to_compress;
	string header_table;
	string encoded_text;
	string getMcpName(string);

	void bit_write(char &src, char &dest, const short int bits);
	short int get_byte(int length);

	public:
	Huffman();
	void compress();
	int readHeader(string hfile);
	void populateHeader(string hfile,string fname);
	void test();
	~Huffman();
};
#endif
