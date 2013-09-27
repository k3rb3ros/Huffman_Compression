/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#ifndef HEADER_H
#define HEADER_H

#include "headers.h"

class HuffmanHeader
{
	private:
	int magic_number;
	string file_to_compress;
	string header_table;
	string encoded_text;
	char end_of_text_marker;
	char eof;
	void getFileName();
	
	public:
	HuffmanHeader();
	void populateHeader(string fname);
	~HuffmanHeader();
};
#endif