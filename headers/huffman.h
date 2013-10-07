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
#define MAX_BIT_SIZE sizeof(unsigned long int)*8
#define U_LONG_INT_SIZE sizeof(unsigned long int)
#define DELIM "\1\27"
#define H_BUF len
#define O_EOF '\26'


class Huffman : public Trie //Huffman is a CharList and Trie
{
	private:
	
	string header_table;
	string encoded_text;
	vector<string> mylist;

	bool get_chbit(unsigned char* buffer, unsigned long int index, unsigned short int offet);
	bool get_ulbit(unsigned long int* buffer, unsigned short int offset);
	int search(unsigned long int* pattern, unsigned short int* length, unsigned char* huffman_character);
	string getMcpName(string fname);
	string setMcpName(string file_to_compress);
	void dump_buffer();
	void set_chbit(unsigned char* buffer, unsigned long int index, unsigned short int offset, bool value);
	void set_ulbit(unsigned long int* buffer, unsigned short int offset, bool value); 

	public:
	Huffman();
	void compress();
	void decompress();
	int readHeader();
	void writeHeader();
	void print_huffman();
	
	double compression_percentage(double &percent_compressed,int compressed_count,int uncompressed_count);
	void deleteVector(vector<string> mylist);
	void table_char_count(string table);
	void test();
	~Huffman();
};
#endif
