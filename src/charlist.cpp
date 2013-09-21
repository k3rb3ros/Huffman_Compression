/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../headers/charlist.h"

void CharList::bufferFile(string fname)
{
		unsigned int i = 0;
		//ofstream out("out.txt"); uneeded now that we know it buffers correctly	    
		ifstream in(fname.c_str(), std::ios::in | std::ios::binary);
		
		if (in.good())
		{
			in.seekg(0, in.end); //Go to end of stream
			len = in.tellg(); //Get the length of the file
			buffer = new char[len];
			buffer[len] = 0; //nullpad the string
			len = len -1; //Subtract one for zero based arrays
			in.seekg(0, in.beg); //Reset stream to beginning
			in >> noskipws; //tell fstream not to ignore whitespace
			
			while(i!=len)
			{
				in >> buffer[i];
				//out << buffer[i];
				i++;
			}			
			//in.close();
			cout << "File contents: " << buffer << endl;
			cout << "Read " << len << " characters... ";
			// read data as a block:
		
			if(in) cout << "all characters read successfully.";
			else cout << "error: only " << in.gcount() << " could be read";
			in.close(); //close the fstream
			// ...buffer contains the entire file...
			delete[] buffer;
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
	CharNode* temp = NULL;	
	for(int i=0; i<len; i++)
	{
		temp = new CharNode;
		temp -> char_in_text = buffer[i];
		temp -> count = 0;
		frequencyTable.push_back(temp);
		temp = NULL;
	}
		cout << "Frequency Table succesfully populated" << endl;
}
