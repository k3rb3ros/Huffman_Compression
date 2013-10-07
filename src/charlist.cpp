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
	file_to_compress = "";
	file_to_decompress = "";
	len = 0;
	h_len = 0;
	enc_len = 0;
	table_len = 0;
	table = "";
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

void CharList::sortTable() //bubble sort the values of our lookup table by occurrence
{
	for(int i=0; i<UCHAR_MAX; i++)
	{
		for(int j=0; j<UCHAR_MAX-1; j++)
		{
		if(((CharNode*) (charSort[j]))->occurrence > ((CharNode*) (charSort[j+1]))->occurrence)
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

void CharList::bufferMessage(string fname)
{
	uint64_t i = 0;

	file_to_compress = fname; //add the filename to the class to use later
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
	
		if(in) cout << file_to_compress << " buffered succesfully\n";
		else 
		{
			cerr << "Error buffering message file! " << in.gcount() << " only characters could be read";
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

void CharList::bufferHuffman(string &data) //write the get_Line string to the huffman buffer
{
	huffman_buffer = new unsigned char[enc_len+1]; //allocate the buffer
	//cout << "Data buffer " << data << endl;
	for(unsigned int i=0; i<data.length(); i++) huffman_buffer[i] = data[i]; //populated it
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

void CharList::DecompPopulateTable()//Populate the lookup table from the char, count table we wrote to the header
{
	bool done = false;
	short int delim = 0;
	short unsigned int i = 0;
	string temp_count = "";
	unsigned char ch = 0;
	uint64_t char_count = 0;
	len = 0;
	table_len = 0;
	cout << table << endl;
	while(!done)
	{
		ch = table[i]; //get the character
		charTable[ch].active = true; //set the character to active
		table_len++;//increment the table length
		delim = i; //set delim to the next character	
		while(table[delim] != 1 && table[delim+1] != 27) delim++; //get the value of the next delimiting character
		i++; //increment i
		temp_count = table.substr(i, delim-i); //get the count
		char_count = strtoul(temp_count.c_str(), NULL, 0); //conver it to a number
		len += char_count; //set the length of the original file
		charTable[ch].occurrence = char_count;
		i = delim+2;//update i to point to the next character
		if(i >= table.length()) done = true; //check if we need to break the loop
	}
}

void CharList::getFileToDecompress()
{
	cout << "Filepath " << file_to_decompress << " length:" << file_to_decompress.length() << endl;
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

void CharList::setFileToCompress(string f_name)
{
	file_to_compress = f_name;
}

void CharList::setFileToDecompress(string f_name)
{
	file_to_decompress = f_name;
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
