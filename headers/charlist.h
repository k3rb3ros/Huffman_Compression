/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#ifndef CHARLIST_H
#define CHARLIST_H

#include "headers.h"
#include "mcp.h"

class CharList
{
	private:

	char* buffer;
	int charOccurrence;
	unsigned long int len;
	//CharNode charTable[CHAR_MAX]; //Create a lookup table for every ascii character		
	//char* charSort[CHAR_MAX]; //Create a sort table pointing to the lookup table so we can have access to sorted values
	void delBuckets(CharBucket* buckets);	
	void swap(char* &a, char* &b);
	
	protected:
	
	CharNode charTable[CHAR_MAX]; //Create a lookup table for every ascii character		
	char* charSort[CHAR_MAX]; //Create a sort table pointing to the lookup table so we can have access to sorted values

	public:

	CharList(); //default constructor
	void bufferFile(string fname); //Name changed so that it makes more sense
	void populateTable();
	void showCharCount();
	void sortTable();
	~CharList(); //Destructor
};

#endif
