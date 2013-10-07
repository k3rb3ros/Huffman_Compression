/*
t By Jesse Blagg and Ryan Morris
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

bool Huffman::delim_match(string &buffer, const string delim)
{
	string temp = "";
	unsigned int length = buffer.length();
	if(length <= delim.length()) return false;
	else
	{
		temp = buffer.substr(length-(delim.length()), delim.length());
		if(temp.compare(delim) == 0)
		{
			buffer.erase(length-delim.length(), delim.length()); //eat the final delimanting pattern
			return true;
		}
	}
	return false;
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

void Huffman::dump_buffer()
{
	ofstream write("debug.dat");
	for(unsigned int i=0; i<enc_len; i++)
	{
		write << "/" << (int)huffman_buffer[i];
	}
	write.close();
}

unsigned long int Huffman::get_Line(ifstream& inf, string &buffer , const string &delim)
{
	char ch = 0;

	while(buffer.length() <= 4 || !delim_match(buffer, delim)) //while we haven't reached our double delimeter
	{
		inf.get(ch);
		buffer += ch;
	}	
	return 1;
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
	enc_len = index+1; //index; //return the length of the huffman trie in bytes
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

	if(huffman_buffer == NULL) 
	{
		cerr << "Huffman buffer is empty exiting" << endl;
		return;
	}
	if(message_buffer != NULL) delete[] message_buffer; //free anything in the buffer if it is already populated
	
	message_buffer = new unsigned char[len]; //allocate the buffer
	for(unsigned long int k=0; k<len; k++) message_buffer[k] = 0; //zero fill the buffer
	
	for(unsigned long int i=0; i<len; i++) //fill every character in the message buffer after it has been decoded
	{
		bool bit = false;
		length = 0; //reset the bitcount 
		Test = root; //set the test node to the Root of the Trie
		
		while(!is_a_leaf(Test) && (length <= enc_len*8))
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
/*	
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
		enc_len = temp_num; //Set the length of the header file in the class
		cout << "Characters in table: " << enc_len << endl;
		huffman_buffer = new unsigned char[enc_len]; //allocate the buffer for the size we need
*/
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
		if(temp[i] == '.') 
		{
			found =true;
			temp[i+1]='m';
			temp[i+2]='c';
			temp[i+3]='p';
			return temp;
		}
	}
	if (found == false) cout << "Not a valid file!" << endl;
		return temp;
}

int Huffman::readHeader()
{
	int num;
	int i=0;
	int f_len = file_to_decompress.size();
	bool done = false;
	char t_char;
	string tableFile = ".mcp_temp";
	string m_number;
	string f_name;
 	const string double_delim = "\1\27\1\27\n";
	string enc_text;
	string e_o_f;
	ifstream inf(file_to_decompress.c_str());
	ofstream out(tableFile.c_str());
	if(!inf) cerr << "Error reading file..." << endl;
	else 
	{
		getline(inf,m_number);
    
		//sstream conversion found here: http://www.cplusplus.com/forum/articles/9645/
		stringstream convert(m_number); // stringstream used for the conversion initialized with the contents of the string
		if ( !(convert >> num) )//give the value to an integer using the characters in the string
		{
			cerr << "Not a valid compressed file. Exiting..." << endl;
			return -1;
		}
		else cout << "Magic number matches!" << endl;	
		
		getline(inf,f_name);
		setFileToCompress(f_name); //Save the original file name so we can write to it later
		cout << "Original file: " << f_name << endl;
		get_Line(inf, table, double_delim);
		get_Line(inf, enc_text, double_delim);
		enc_len = enc_text.length();
		bufferHuffman(enc_text);
		getline(inf, e_o_f);
	}
	inf.close();
	return 1;
}

void Huffman::writeHeader() // Write the header
{
	ofstream debug("compress.dat");
	file_to_decompress = setMcpName(file_to_compress);
	ofstream outf(file_to_decompress.c_str());
	outf << MAGIC_NUMBER << endl; //write magic number
	outf << file_to_compress << endl; //Write file name
	for(unsigned char i=0; i<UCHAR_MAX; i++) if(charTable[i].active) outf << charTable[i].character << charTable[i].occurrence << DELIM; //Write the active characters and their counts
	outf << DELIM << endl; //Write char table and int with with double delimiter
	for(unsigned long int i=0;i<enc_len;i++) outf << huffman_buffer[i]; outf << DELIM << DELIM << endl; //write the huffman code 
	for(unsigned long int i=0;i<enc_len;i++) debug << "\\" << (int)huffman_buffer[i]; 
    	outf << O_EOF; //write our "Output" EOF character
	outf.close(); //close the filestream
}

void Huffman::print_orig()
{
	
	cout << "########START UNCOMPRESSED BUFFER########\n";
	for(unsigned int i=0; i<len; i++)
	{
		cout << message_buffer[i];
	}
	cout << "\n########END UNCOMPRESSED BUFFER " << enc_len << " characters########\n";
}

void Huffman::print_huffman()
{
	cout << "########START HUFFMAN BUFFER########\n";
	for(unsigned int i=0; i<enc_len; i++)
	{
		cout << "\\" << (int)huffman_buffer[i];
	}
	cout << "\n########END HUFFMAN BUFFER " << enc_len << " characters########\n";
}

void Huffman::test()
{
	dump_buffer();
}	

Huffman::~Huffman() //Destructor deletes entire header
{
}
