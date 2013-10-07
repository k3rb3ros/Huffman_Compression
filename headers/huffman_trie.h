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
#include "structs.h"

class Trie: public CharList //Contains all the necessary resources to produce a huffman tree and lookup table
{
	private:

	friend class Huffman;
	Trie_node* root;
	Trie_node* insert_node(Trie_node* Root, unsigned char character, uint64_t val);
	Trie_node* insert_2nodes(Trie_node* Root, unsigned char char1, uint64_t val1, unsigned char char2, uint64_t val2);
	int enc_traverse(Trie_node* Root, short int bit, uint64_t bitcode, unsigned short int length);
	uint64_t char_count;
	uint64_t node_count;
	uint64_t sum_nodes(Trie_node* Root, uint64_t sum);
	void count_traverse(Trie_node* Root);
	void delete_trie(Trie_node* Root);
	void node_traverse(Trie_node* Root);
	void print_binary(CharNode* node);

	protected:
	bool is_a_leaf(Trie_node* Node);
			
	public:
	
	Trie();
	uint64_t character_count();
	uint64_t size_of_trie();
	void populate_trie();
	int get_encoding();
	void printBinary();
	void print_encoding_table();
	void print_sort_table();
	~Trie();
};

#endif
