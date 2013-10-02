/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../headers/charlist.h"

CharList::CharList()
{
	file_buffer = NULL;
	huffman_buffer = NULL;
	charOccurrence = -1;
	len = 0;
	h_len = 0;
	for(unsigned char i=0; i<UCHAR_MAX; i++)
	{
		charTable[i].active = false;
		charTable[i].character = i;
		charTable[i].encodeLength = 0;		
		charTable[i].encoding = 0;
		charTable[i].occurrence = 0;
		charTable[i].buckets = NULL;
		charSort[i] = (unsigned char*)&charTable[i]; //Initialize charSort to point to every index in charTable
		//cout << "Addr " << i+1 << " : " << &charSort[i] << ", ";
	}
}

void CharList::sortTable()
{
	cout << endl;
	for(int i=0; i<UCHAR_MAX; i++)
	{
		for(int j=0; j<UCHAR_MAX-1; j++)
		{
		if((((CharNode*) &*(charSort[j])))->occurrence > (((CharNode*) &*(charSort[j+1])))->occurrence)
			{
				swap(charSort[j], charSort[j+1]);
			}
		}
	}
	for(int i=0; i<UCHAR_MAX; i++)
	{
		if((((CharNode*) &*(charSort[i])))->occurrence) cout << "|" << (((CharNode*) &*(charSort[i])))->character << ":" << (((CharNode*) &*(charSort[i])))->occurrence << "| ";
	}
}

void CharList::delBuckets(CharBucket* buckets) //recursively delete any buckets that have been added to charTable
{
	if(buckets == NULL) return;
	delBuckets(buckets -> buckets);
	delete buckets; 	
}

void CharList::swap(unsigned char* &a, unsigned char* &b)
{
	unsigned char* temp = NULL;
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
			//len -= EOF_LENGTH; //Subtract the length of the eof character
			file_buffer = new unsigned char[len+1];
			file_buffer[len] = 0; //null terminate the string
			in.seekg(0, in.beg); //Reset stream to beginning
			in >> noskipws; //tell fstream not to ignore whitespace
			
			while(i!=len)
			{
				in >> file_buffer[i];
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
	unsigned char ch = 0;
	if(file_buffer != NULL)
	{
		for(unsigned int i=0; i<len; i++)
		{
			ch = file_buffer[i];
			//if(ch < 0 || ch > 255)
			//{
			//	cerr << "invalid (standard range) ascii character encountered\n";
			//}
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
	cout << "Character Occurance Table:" << endl;
	for(unsigned char i=0; i<UCHAR_MAX; i++)
	{
		if(charTable[i].active == true)
		{
			cout << "|" << i << "| " << charTable[i].occurrence << ", ";
		}
	}
}

CharList::~CharList()
{
	if(file_buffer != NULL) delete[] file_buffer; //Free file_buffer
	if(huffman_buffer != NULL) delete[] huffman_buffer; //Free hufmman_buffer
	for(int i=0; i<UCHAR_MAX; i++)
	{
		delBuckets(charTable[i].buckets);
	}
}
