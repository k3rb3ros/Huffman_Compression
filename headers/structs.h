/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#ifndef STRUCTS_H
#define STRUCTS_H

struct CharBucket
{
	uint64_t encoding;
	CharBucket* buckets;
};

struct CharNode //Structure used for CharTable will be shared with Trie class
{
	bool active;
	uint8_t character;	
	CharBucket* buckets;
	short int encodeLength;	
	uint64_t encoding;
	uint64_t occurrence;
};

struct Trie_node //Trie node struct used to create Huffman encoding
{
	bool is_character;	
	uint8_t character;
	uint64_t val;
	Trie_node* left;
	Trie_node* right;
};

#endif
