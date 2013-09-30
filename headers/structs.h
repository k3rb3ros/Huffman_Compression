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
	unsigned long int encoding;
	CharBucket* buckets;
};

struct CharNode //Structure used for CharTable will be shared with Trie class
{
	bool active;
	unsigned char character;	
	CharBucket* buckets;
	int encodeLength;	
	unsigned long int encoding;
	unsigned long int occurrence;
};

struct Trie_node //Trie node struct used to create Huffman encoding
{
	bool is_character;	
	unsigned char character;
	unsigned long int val;
	Trie_node* left;
	Trie_node* right;
};

#endif
