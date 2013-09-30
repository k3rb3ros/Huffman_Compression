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
#define H_BUF 3*len
#define O_EOF '\26'


class Huffman : public Trie //Huffman is a CharList and Trie
{
	private:
	string file_to_compress;
	string header_table;
	string encoded_text;
	string getMcpName(string);

	void bit_write(unsigned char &src,unsigned char &dest, const short int bits);
	void dump_buffer();
	short int get_bit(int l_carry, short int length);
	short int get_byte(int length);
	short int get_offset(int length);
	string getLine(ifstream &inf, string delim);

	public:
	Huffman();
	void compress();
	int readHeader(string hfile);
	void populateHeader(string hfile,string fname);
	void print_huffman();
	void test();
	~Huffman();
};
#endif
