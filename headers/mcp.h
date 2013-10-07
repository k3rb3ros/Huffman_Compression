/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#ifndef MCP_H
#define MCP_H

#include "headers.h"
#include "huffman.h"

class Mcp : public Huffman
{
	public:
	void compress(char* file_name);
	void decompress(char* file_name);
};
#endif
