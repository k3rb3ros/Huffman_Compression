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
#define H_BUF 2*len
#define O_EOF '\26'


class Huffman : public Trie //Huffman is a CharList and Trie
{
	private:
	string file_to_compress;
	string header_table;
	string encoded_text;
	string getMcpName(string);
	vector<string> mylist;

	void bit_write(unsigned char &src,unsigned char &dest, const short int bits);
	short int get_bit(int l_carry, short int length);
	short int get_byte(int length);
	short int get_offset(int length);
	void dump_buffer();
	bool getbit(unsigned long int* buffer, short int offset);
	void setbit(unsigned char* buffer,int index, short int offset, bool value);

	public:
	Huffman();
	void compress();
	int readHeader(string hfile);
	void populateHeader(string hfile,string fname);
	void print_huffman();
	
	double compression_percentage(double &percent_compressed,int compressed_count,int uncompressed_count);
	void deleteVector(vector<string> mylist);
	void table_char_count(string table);
	void test();
	~Huffman();
};
#endif
