/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../headers/charlist.h"

CharList::CharList()
{
	buffer = NULL;
	charOccurrence = -1;
	len = 0;
	for(char i=0; i<CHAR_MAX; i++)
	{
		charTable[i].active = false;
		charTable[i].character = i;
		charTable[i].encodeLength = 0;		
		charTable[i].encoding = 0;
		charTable[i].occurrence = 0;
		charTable[i].buckets = NULL;
		charSort[i] = (char*)&charTable[i]; //Initialize charSort to point to every index in charTable
		//cout << "Addr " << i+1 << " : " << &charSort[i] << ", ";
	}
}

void CharList::sortTable()
{
	cout << endl;
	for(int i=0; i<CHAR_MAX; i++)
	{
		for(int j=0; j<CHAR_MAX-1; j++)
		{
		if((((CharNode*) &*(charSort[j])))->occurrence > (((CharNode*) &*(charSort[j+1])))->occurrence)
			{
				swap(charSort[j], charSort[j+1]);
			}
		}
	}
	cout << endl;
	for(char l=0; l<CHAR_MAX; l++)
	{
		if(((CharNode*) &*(charSort[l]))->occurrence > 0) 
		{
			cout << "|\"" << ((CharNode*) &*(charSort[l]))->character << "\":" << ((CharNode*) &*(charSort[l]))->occurrence << "|  ";
		}
	}
}

void CharList::delBuckets(CharBucket* buckets) //recursively delete any buckets that have been added to charTable
{
	if(buckets == NULL) return;
	delBuckets(buckets -> buckets);
	delete buckets; 	
}

void CharList::swap(char* &a, char* &b)
{
	char* temp = NULL;
	temp = a;
	a = b;
	b = temp;
}

void CharList::bufferFile(string fname)
{
		unsigned int i = 0;
		ifstream in(fname.c_str(), std::ios::in | std::ios::binary);
		
		if (in.good())
		{
			in.seekg(0, in.end); //Go to end of stream
			len = in.tellg(); //Get the length of the file
			buffer = new char[len+1];
			buffer[len] = 0; //nullpad the string
			in.seekg(0, in.beg); //Reset stream to beginning
			in >> noskipws; //tell fstream not to ignore whitespace
			
			while(i!=len)
			{
				in >> buffer[i];
				i++;
			}			
			cout << "Read " << len << " characters... ";
			// read data as a block:
		
			if(in) cout << "all characters read successfully.";
			else cout << "error: only " << in.gcount() << " characters could be read";
			in.close(); //close the fstream
			// ...buffer contains the entire file...
		}

		else
		{
			cerr << " File Not Found! " << endl;  
			return;
		}
		cout << endl << " length is = " << len << endl;
}

void CharList::populateTable()
{
	char ch = 0;
	if(buffer != NULL)
	{
		for(unsigned int i=0; i<len; i++)
		{
			ch = buffer[i];
			if(charTable[ch].active == false)
			{
				charTable[ch].active = true; //set this character to active
				charTable[ch].occurrence = 1; //set occurance to 1
			}
			else charTable[ch].occurrence ++;
		}
		cout << "Frequency Table succesfully populated" << endl;
	}
}

void CharList::showCharCount()
{
	cout << "Character Count:" << endl;
	for(char i=0; i<CHAR_MAX; i++)
	{
		if(charTable[i].active == true)
		{
			cout << "\"" << i << "\"" << charTable[i].occurrence << ", ";
		}
	}
}

CharList::~CharList()
{
	if(buffer != NULL) delete[] buffer; //Free anything we allocated in buffer
	for(int i=0; i<CHAR_MAX; i++)
	{
		delBuckets(charTable[i].buckets);
	}
}
