/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../headers/charlist.h"

CharList::CharList()
{
	message_buffer = NULL;
	huffman_buffer = NULL;
	charOccurrence = -1;
	file_to_compress = "us_constitution.txt";
	file_to_decompress = "";
	len = 0;
	h_len = 0;
	table_len = 0;
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
	for(int i=0; i<UCHAR_MAX; i++)
	{
		for(int j=0; j<UCHAR_MAX-1; j++)
		{
		if(((CharNode*) &*(charSort[j]))->occurrence > ((CharNode*) &*(charSort[j+1]))->occurrence)
			{
				swap(charSort[j], charSort[j+1]);
			}
		}
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

void CharList::bufferMessage()
{
	unsigned long int i = 0;

	ifstream in(file_to_compress.c_str(), std::ios::in | std::ios::binary); //attempt to open a binary filestream to the message file
		
	if (in.good()) 
	{
		in.seekg(0, in.end); //Go to end of stream
		len = in.tellg(); //Get the length of the file
		message_buffer = new unsigned char[len+1];
		message_buffer[len] = 0; //null terminate the string
		in.seekg(0, in.beg); //Reset stream to beginning
		in >> noskipws; //tell fstream not to ignore whitespace
			
		while(i!=len)
		{
			in >> message_buffer[i];
			i++;
		}			
		//cout << "Read " << len << " characters... ";
		// read data as a block:
	
		if(in) cout << file_to_compress << " buffered succesfully\n";
		else 
		{
			cout << "Error buffering message file! " << in.gcount() << " only characters could be read";
			return;
		}
		in.close(); //close the fstream
		// ...buffer contains the entire file...
	}

	else
	{
		cerr << " Unable to buffer message file " << file_to_compress << " exiting\n";
		return;
	}
		//cout << endl << " length is = " << len << endl;
}

void CharList::bufferHuffman()
{
	unsigned long int i = 0;

	ifstream in(file_to_decompress.c_str(), std::ios::in | std::ios::binary); //attempt to open a binary filestream to the message file
	if(in.good())
	{
		in.seekg(0, in.end); //Go to end of stream
		h_len = in.tellg(); //Get the length of the file
		message_buffer = new unsigned char[len+1];
		message_buffer[len] = 0; //null terminate the string
		in.seekg(0, in.beg); //Reset stream to beginning
		in >> noskipws; //tell fstream not to ignore whitespace
			
		while(i!=len)
		{
			in >> huffman_buffer[i];
			i++;
		}
			
		if(in) cout << file_to_decompress << " buffered succesfully\n";
		else
		{
			cout << "Error buffering compressed file! " << in.gcount() << " only characters could be read";
			return;
		}
		in.close();
	}
	else
	{
		cerr << " Unable to buffer compressed file " << file_to_decompress << " exiting\n";
		return;
	}
}

void CharList::CompPopulateTable()
{
	unsigned char ch = 0;
	if(message_buffer != NULL)
	{
		for(unsigned int i=0; i<len; i++)
		{
			ch = message_buffer[i];
			if(charTable[ch].active == false)
			{
				charTable[ch].active = true; //set this character to active
				charTable[ch].occurrence = 1; //set occurance to 1				
				table_len++; //incriment table length
			}
			else charTable[ch].occurrence ++;
		}
		cout << "Frequency Table succesfully populated" << endl;
	}
}

void CharList::DecompPopulateTable(vector<string> char_list)
{
	string temp_count = "";
	unsigned char ch = 0;
	for(int i=0; i<char_list.size(); i+=2)
	{
		ch = char_list[i][0];
		//ch = temp_char[0];	
		temp_count = char_list[i+1];//get the count
		
		charTable[ch].active = true; //set the character to active
		charTable[ch].occurrence = strtoul(temp_count.c_str(), NULL, 0); //set the character count of that character in the charlist
	}
}

void CharList::printMessageBuffer()
{
	cout << "########Start Message Buffer########\n";
	for(unsigned int i=0; i<len; i++)
	{
		cout << message_buffer[i];
	}
	cout << endl << "########End Message Buffer########" << endl;
}

void CharList::printSorted()
{
	cout << endl << "Sorted characters by count:" << endl;
	for(int i=0; i<UCHAR_MAX; i++)
	{
		if((((CharNode*) &*(charSort[i])))->occurrence) cout << "|" << (((CharNode*) &*(charSort[i])))->character << ":" << (((CharNode*) &*(charSort[i])))->occurrence << "| ";
	}
	cout << endl;
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
	if(message_buffer != NULL) delete[] message_buffer; //Free message_buffer
	if(huffman_buffer != NULL) delete[] huffman_buffer; //Free hufmman_buffer
	for(int i=0; i<UCHAR_MAX; i++)
	{
		delBuckets(charTable[i].buckets);
	}
}
