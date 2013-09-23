/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#ifndef CHARLIST_H
#define CHARLIST_H

#include "headers.h"

struct CharBucket
{
	unsigned long int encoding;
	CharBucket* buckets;
};

struct CharNode //Structure used for CharTable will be shared with Trie class
{
	bool active;
	//char char_in_text;	
	CharBucket* buckets;
	unsigned long int encodeLength;	
	unsigned long int encoding;
	unsigned long int occurrence;
};

class CharList
{
	private:

	char* buffer;
	int charOccurrence;
	unsigned long int len;
	CharNode charTable[CHAR_MAX]; //Create a lookup table for every ascii character		
	char* charSort[CHAR_MAX];
	//vector<CharNode*> frequencyTable; //Added to class to be compliant with Object Oriented methodology
	void delBuckets(CharBucket* buckets);	
	void swap(char* &a, char* &b);
	public:

	CharList(); //default constructor
	void bufferFile(string fname); //Name changed so that it makes more sense
	bool isFound(char a);
	void populateTable();
	void showCharCount();
	void sortTable();
	~CharList(); //Destructor
};

#endif
