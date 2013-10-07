/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../headers/charlist.h"
#include "../headers/headers.h"
#include "../headers/huffman.h"
#include "../headers/huffman_trie.h"


void compress(char* file_name)
{
	string mycppstr(file_name);
	Huffman huffman;
	huffman.bufferMessage();
	huffman.populateTable();
	huffman.sortTable();
	huffman.populate_trie();
	huffman.get_encoding();
	huffman.compress();
	huffman.writeHeader();
	//huffman.compression_percentage(percent_compressed,compressed_count,uncompressed_count);

}

void decompress(char* file_name)
{
	string mycppstr(file_name);	
	Huffman huffman;

	huffman.readHeader();
	huffman.populateTable();
	huffman.sortTable();
	huffman.populate_trie();
	//huffman.get_encoding(); // we may not need this
	huffman.decompress();
	huffman.bufferHuffman();
}
