/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#ifndef CHARLIST_H
#define CHARLIST_H

#include "headers.h"

struct CharNode //Structure used for count table
{
	unsigned long int count;
	char char_in_text;
};

class CharList
{
	private:

	char* buffer;
	int charOccurrence;
	unsigned long int len;
	vector<CharNode*> frequencyTable; //Added to class to be compliant with Object Oriented methodology
	
	public:

	void bufferFile(string fname); //Name changed so that it makes more sense
	bool isFound(char a);
	void populateTable();			
};

#endif
