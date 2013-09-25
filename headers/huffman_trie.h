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
	Trie_node* insert_node(Trie_node* Root, char character, unsigned long int val);
	unsigned long int char_count;
	unsigned long int node_count;
	void count_traverse(Trie_node* Root);
	void delete_trie(Trie_node* Root);
	void enc_traverse(Trie_node* Root, stack<int> huffman);
	void node_traverse(Trie_node* Root);
	unsigned long int sum_nodes(Trie_node* Root, unsigned long int sum);

	public:
	
	Trie();
	unsigned long int character_count();
	unsigned long int size_of_trie();
	void populate_trie();
	void get_encoding();
	void print_encoding_table();
	void print_sort_table();
	~Trie();
};

#endif
