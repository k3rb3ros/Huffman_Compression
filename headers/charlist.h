/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#ifndef CHARLIST_H
#define CHARLIST_H

#include "headers.h"
#include "structs.h"

class CharList
{
	private:	
	friend class Huffman;
	unsigned char* message_buffer;
	unsigned char* huffman_buffer;
	int charOccurrence;
	string file_to_compress;
	string file_to_decompress;
	unsigned long int len;
	unsigned long int h_len;
	void delBuckets(CharBucket* buckets);	
	void swap(unsigned char* &a, unsigned char* &b);
	
	protected:
	short int table_len;
	
	CharNode charTable[UCHAR_MAX]; //Create a lookup table for every ascii character		
	unsigned char* charSort[UCHAR_MAX]; //Create a sort table pointing to the lookup table so we can have access to sorted values

	public:

	CharList(); //default constructor
	void bufferMessage(); //Name changed so that it makes more sense
	void bufferHuffman();
	void populateTable();
	void printMessageBuffer();
	void printSorted();
	void showCharCount();
	void sortTable();
	~CharList(); //Destructor
};

#endif
