/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "charlist.h"
#include "headers.h"
#include "huffman_trie.h"
#include "structs.h"

#define MAGIC_NUMBER 0x63130
#define MAX_BIT_SIZE sizeof(uint64_t)*8
#define U_LONG_INT_SIZE sizeof(uint64_t)
#define DELIM "\1\27"
#define H_BUF len
#define O_EOF '\26'


class Huffman : public Trie //Huffman is a CharList and Trie
{
	private:
	
	string header_table;
	string encoded_text;

	bool delim_match(string &buffer, const string delim);
	bool get_chbit(unsigned char* buffer, uint64_t index, unsigned short int offet);
	bool get_ulbit(uint64_t* buffer, unsigned short int offset);
	string getMcpName(string fname);
	string setMcpName(string file_to_compress);
	void dump_buffer();
	uint64_t get_Line(ifstream& inf, string &buffer, const string &delim);
	void set_chbit(unsigned char* buffer, uint64_t index, unsigned short int offset, bool value);
	void set_ulbit(uint64_t* buffer, unsigned short int offset, bool value); 

	public:
	Huffman();
	double compression_percentage();
	int readHeader();
	void compress();
	void decompress();
	void print_orig();
	void print_huffman();
	void test();
	void writeHeader();
	void writeMessage();
	
	~Huffman();
};
#endif
