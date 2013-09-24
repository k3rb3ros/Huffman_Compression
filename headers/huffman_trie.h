/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#ifndef HUFFMAN_TRIE_H
#define HUFFMAN_TRIE_H

#include "charlist.h"
#include "headers.h"

/*struct Enc_node //Enc node used to create a lookup table of Huffman encoding for every character
{	
	bool active;
	unsigned long int encoding;
	int length;
	Enc_node* buckets;
};*/

struct Trie_node //Trie node struct used to create Huffman encoding
{
	bool is_character;	
	char character;
	unsigned long int val;
	Trie_node* left;
	Trie_node* right;
};

class Trie: public CharList //Contains all the necessary resources to produce a huffman tree and lookup table
{
	private:

	//Enc_node enc_table[CHAR_MAX];
	Trie_node* root;
	unsigned long int char_count;
	unsigned long int node_count;
	void count_traverse(Trie_node* root);
	//void delete_buckets(Enc_node* bkt);
	void delete_trie(Trie_node* root);
	void enc_traverse(Trie_node* Root, stack<int> huffman, int code_length);
	void node_traverse(Trie_node* root);
	
	public:
	
	Trie();
	unsigned long int character_count();
	void get_encoding();
	unsigned long int size_of_trie();
	void insert_trie(const char Character, int count);
	~Trie();
};

#endif
