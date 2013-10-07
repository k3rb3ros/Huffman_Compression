/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/
#include "../headers/huffman.h"

Huffman::Huffman()
{
		header_table = "";
		encoded_text = "";
}

bool Huffman::get_chbit(unsigned char* buffer, unsigned long int index, unsigned short int offset)
{
	unsigned long int mask = 0;
	mask = (0x80 >> offset % (sizeof(unsigned char)*8)); //set the mask to the nth bit from the left side
	return (buffer[index] & mask) != 0; //if the buffer bitwise anded with the mask = return true (1) otherwise its a zero
}

/* Created with the help of Dr. MacEvoy */
bool Huffman::get_ulbit(unsigned long int* buffer, unsigned short int offset)//get the bit value at byte[offset] starting at the right wiht least significant bit
{
	unsigned long int mask = 0;
	if(offset >= MAX_BIT_SIZE) return false;
	mask = (0x8000000000000000 >> offset); //set the mask to the nth bit from the right side
	return (*buffer & mask) != 0;
}

int Huffman::search(unsigned long int* pattern, unsigned short int* length, unsigned char* huffman_character)
{
	cout << "searching for: " << hex << *pattern << endl;
	for(short int ch=UCHAR_MAX; ch >= 0; ch--)
	{
		unsigned long int count = ((CharNode*) &*(charSort[ch]))->occurrence;
		while(count != *length && ch >= 0)
		{
			if(count == 0 || (ch == 0 && (count != *length))) return -1; //if there isn't a bitcode length match, then stop searching
			count = ((CharNode*) &*(charSort[--ch]))->occurrence; //decrement our search table index until we find bitcodes of the same length
			//cout << "search: " << ch << endl;
		}
		if(*pattern == ((CharNode*) &*(charSort[ch]))->encoding)//If this is true we found a match
		{
			*huffman_character = (unsigned char)ch;
			cout << "Match found character is:" << (unsigned char)ch << endl;
			return 1; //return success
		}	
	}
	return -1;
}

void Huffman::dump_buffer()
{
	ofstream write("test.dat");
	for(unsigned int i=0; i<len; i++)
	{
		write << "/" << (int)message_buffer[i];
	}
	write.close();
}

/* Created with the help of Dr. MacEvoy */
void Huffman::set_chbit(unsigned char* buffer, unsigned long int index, unsigned short int offset, bool value) 
{
	unsigned char mask = 0;
	//we want this function to be able to overflow since it carries over to the next byte
	mask = (0x80 >> (offset % (sizeof(unsigned char)*8)));
	if(value) buffer[index] |= mask;
}

void Huffman::set_ulbit(unsigned long* buffer, unsigned short int offset, bool value)
{
	unsigned long int mask = 0;
	if(offset >= sizeof(unsigned long int)*8) return;
	mask = (0x8000000000000000 >> offset);
	if(value) *buffer |= mask;
}

string Huffman::getMcpName(string fname)
{
	int i=0;
	string str = fname;
	while (str[i] != '.') i++;
	if(str[i] == '.' && str[i+1] == 'm' && str[i+2] == 'c' && str[i+3] == 'p')
	{
		cout << "Valid compression file." << endl; 
	}	
	else cout << "Not a valid compressed file. Exiting..." << endl;
	return str;
}

void Huffman::compress() //compress the original message
{
	unsigned char ch = 0;
	int code_length = 0;
	//short int offset = 0;
	unsigned long int* bit_code = NULL;
	unsigned long int bit_count = 0;
	unsigned long int index = 0;
	
	huffman_buffer = new unsigned char[H_BUF];//allocate the buffer
	for(unsigned long int k=0; k<H_BUF; k++) huffman_buffer[k] = 0;
	
	for(unsigned long int i=0; i<len; i++)
	{	
		ch = message_buffer[i]; //get the character we are encoding
		bit_code = &charTable[ch].encoding; //look up its bit_code
		code_length = charTable[ch].encodeLength; //look up the length (in bits) of the bitcode
		
		for(short int j=0; j<code_length; j++)
		{
			set_chbit(huffman_buffer, index, bit_count++ , get_ulbit(bit_code, j)); //write the bitcodes to the buffer
			if(bit_count%8 == 0) index++;
		}
	}
	if(huffman_buffer[index] != 0) huffman_buffer[index++]; //nullpad the buffer if it isn't already
	h_len = index+1; //index; //return the length of the huffman trie in bytes
}

void Huffman::decompress() //decompress our Huffman encoded message
{
	bool match = false;
	Trie_node* Test = NULL;
	unsigned char ch = 0;
	unsigned long int bit_count = 0;
	unsigned long int current_bit_code = 0;
	unsigned long int index = 0;
	unsigned short int length = 0;

	if(message_buffer != NULL) delete[] message_buffer; //free anything in the buffer if it is already populated
	
	message_buffer = new unsigned char[len]; //allocate the buffer
	for(unsigned long int k=0; k<len; k++) message_buffer[k] = 0; //zero fill the buffer
	
	for(unsigned long int i=0; i<len; i++) //fill every character in the message buffer after it has been decoded
	{
		bool bit = false;
		length = 0; //reset the bitcount 
		Test = root; //set the test node to the Root of the Trie
		
		while(!is_a_leaf(Test) && (length <= h_len*8))
		{
			bit = (get_chbit(huffman_buffer, index, bit_count++));// get the current bit in the buffer
			cout << bit;
			if(bit_count %8 == 0) index++; //incriment index every byte			
			length++;
			if(Test != NULL)
			{
				if(bit) Test = Test -> right;
				else Test = Test -> left;
			}
		}
		if(is_a_leaf(Test))
		{
			ch = Test -> character;
			message_buffer[i] = ch;
		}
		else cerr << "bitcode does not exist\n";
	}
}
void Huffman::table_char_count(string table)
{	
	string temp_str;
	string str;
	int temp_num=0;
	
	//insert the characters and their counts into a vector in order to split them from the string
	for(unsigned int i=0; i< table.length();i++)
	{
		if(table[i]!= ' ') temp_str += table[i];
		if(table[i]== ' ')
		{
			i++;
			mylist.push_back (temp_str);
			temp_str="";
		}	
	}
	//convert every other element in the array back into an integer and add them up
	for(unsigned int j=0; j < mylist.size(); j++)
		if (j%2 == 1)
		{
			temp_num += atoi(mylist[j].c_str());	
		}
		cout << "Characters in table: " << temp_num << endl;
}

void Huffman::deleteVector(vector<string> mylist)
{
	mylist.erase (mylist.begin(),mylist.end());
}

double Huffman::compression_percentage(double &percent_compressed,int compressed_count,int uncompressed_count)
{
	percent_compressed = (compressed_count / uncompressed_count) * 100; 
	return percent_compressed;
}

string Huffman::setMcpName(string fname)
{
	string temp =fname;
	bool found = false;
	for(int i=0; i<temp.length();i++)
	{	
		cout << " temp[" << i << "] in for loop = " << temp[i] << endl;
		if(temp[i] == '.') 
		{
			found =true;
			cout << " temp[i] = " << temp[i];
			temp[i+1]='m';
			temp[i+2]='c';
			temp[i+3]='p';
			return temp;
		}
	}
	if (found == false) cout << "Not a valid file!" << endl;
		return temp;
}

int Huffman::readHeader(string hfile)
{
	int num;
	cout << endl << endl << "  charmax = " << UCHAR_MAX << endl << endl;
	int i=0;
	int f_len = hfile.size();
	bool done = false;
	//	cout << endl << endl << "  charmax = " << UCHAR_MAX << endl << endl;
	char t_char;
	string tableFile = "tableFile.txt";
	string m_number;
	string f_name;
	string table;
	string double_delim = "\1\27\1\27";
	string enc_text;
	string e_o_f;
	ifstream inf(hfile.c_str());
	ofstream out(tableFile.c_str());
	if(!inf) cout << "Error reading file..." << endl;
	else 
	{
		getline(inf,m_number);
    
		//sstream conversion found here: http://www.cplusplus.com/forum/articles/9645/
		stringstream convert(m_number); // stringstream used for the conversion initialized with the contents of the string
		if ( !(convert >> num) )//give the value to an integer using the characters in the string
		{
			cout << "Not a valid compressed file. Exiting..." << endl;
			return -1;
		}
		else cout << " Magic number matches!" << endl;	
		
		getline(inf,f_name);
		cout << "Original file: " << f_name << endl;
		getline(inf,table);	
		cout << " Table: " << endl << table;
		//extract table data from header file
		while(!done)
		{
			inf.get(t_char);
			table+=t_char;
			//make a string comparison with the last four characters in the array in order to break the loop.
			if(i++ > 4 && table.compare(table.length()-4,4,double_delim)==0) done=true;
		}
		i=0;

		//remove delimiters from table and replace them with white space 
		while(i!=table.length())
		{
			t_char=table[i];
			if(t_char=='\1' || t_char=='\27')
			{	
				table[i]=' ';		
			}
			i++;
		}
		out << table;
		table_char_count(table);
		out.close();
		
		getline(inf,enc_text);
	//	cout << endl << "enc_text = "  << enc_text << endl;
		getline(inf, e_o_f);
		cout  << endl  << "eof: " << e_o_f;
	}
	return 0;
}

void Huffman::writeHeader() // Write the header
{
	file_to_decompress = setMcpName(file_to_compress);
	ofstream outf(file_to_decompress.c_str());
	outf << MAGIC_NUMBER << endl; //write magic number
	outf << file_to_compress << endl; //Write file name
	for(unsigned char i=0; i<UCHAR_MAX; i++) if(charTable[i].active) outf <<  charTable[i].character << DELIM << charTable[i].occurrence; //Write the active characters and their counts
	outf << DELIM << DELIM << endl; //Write char table and int with with double delimiter
	for(unsigned long int i=0;i<h_len;i++) outf << huffman_buffer[i]; outf << endl; //write the huffman code 
    	outf << O_EOF; //write our "Output" EOF character
	outf.close(); //close the filestream
}

void Huffman::print_huffman()
{
	cout << "########START HUFFMAN BUFFER########\n";
	for(unsigned int i=0; i<h_len; i++)
	{
		cout << "\\" << (int)huffman_buffer[i];
	}
	cout << "\n########END HUFFMAN BUFFER " << h_len << " characters########\n";
}

void Huffman::test()
{
}	

Huffman::~Huffman() //Destructor deletes entire header
{
}
