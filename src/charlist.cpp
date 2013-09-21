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
	for(int i=0; i<CHAR_MAX; i++)
	{
		charTable[i].active = false;
		charTable[i].encodeLength = 0;		
		charTable[i].encoding = 0;
		charTable[i].occurrence = 0;
		charTable[i].buckets = NULL;
	}
}

void CharList::delBuckets(CharBucket* buckets) //lots of deleting bucket action up in here!
{
	if(buckets == NULL) return;
	delBuckets(buckets -> buckets);
	delete buckets; 	
}

void CharList::bufferFile(string fname)
{
		unsigned int i = 0;
		//ofstream out("out.txt"); uneeded now that we know it buffers correctly	    
		ifstream in(fname.c_str(), std::ios::in | std::ios::binary);
		
		if (in.good())
		{
			in.seekg(0, in.end); //Go to end of stream
			len = in.tellg(); //Get the length of the file
			buffer = new char[len+1];
			buffer[len] = 0; //nullpad the string
			//len = len -1; //Subtract one for zero based arrays
			in.seekg(0, in.beg); //Reset stream to beginning
			in >> noskipws; //tell fstream not to ignore whitespace
			
			while(i!=len)
			{
				in >> buffer[i];
				//out << buffer[i];
				i++;
			}			
			//in.close();
			//cout << "File contents: " << buffer << endl;
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

bool CharList::isFound(char a)
{
	return 1;
}

void CharList::populateTable()
{
	char ch = 0;
	//CharNode* temp = NULL;	
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
		
		//temp = new CharNode;
		//temp -> char_in_text = buffer[i];
		//temp -> count = 0;
		//frequencyTable.push_back(temp);
		//temp = NULL;
	}
		cout << "Frequency Table succesfully populated" << endl;
}

void CharList::showCharCount()
{
	cout << "Character Count:" << endl;
	for(char i=0; i<CHAR_MAX; i++)
	{
		if(charTable[i].active == true)
		{
			cout << "\"" << i << "\": " << charTable[i].occurrence << ", ";
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
