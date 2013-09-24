/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#ifndef MCP_H
#define MCP_H

#include "headers.h"

struct CharBucket
{
	unsigned long int encoding;
	CharBucket* buckets;
};

struct CharNode //Structure used for CharTable will be shared with Trie class
{
	bool active;
	char character;	
	CharBucket* buckets;
	unsigned long int encodeLength;	
	unsigned long int encoding;
	unsigned long int occurrence;
};

#endif
