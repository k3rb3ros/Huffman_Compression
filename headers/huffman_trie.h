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

	Trie_node* root;
	Trie_node* insert_node(Trie_node* Root, unsigned char character, unsigned long int val);
	Trie_node* insert_2nodes(Trie_node* Root, unsigned char char1, unsigned long int val1, unsigned char char2, unsigned long int val2);
	unsigned long int char_count;
	unsigned long int node_count;
	void count_traverse(Trie_node* Root);
	void delete_trie(Trie_node* Root);
	void enc_traverse(Trie_node* Root, unsigned long int bit, unsigned long int bitcode, int length);
	void node_traverse(Trie_node* Root);
	void print_binary(CharNode* node);
	unsigned long int sum_nodes(Trie_node* Root, unsigned long int sum);

	public:
	
	Trie();
	unsigned long int character_count();
	unsigned long int size_of_trie();
	void populate_trie();
	void get_encoding();
	void printBinary();
	void print_encoding_table();
	void print_sort_table();
	~Trie();
};

#endif
