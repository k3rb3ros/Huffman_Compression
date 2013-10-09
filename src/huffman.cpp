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

bool Huffman::get_chbit(uint8_t* buffer, uint64_t index, unsigned short int offset)
{
	uint64_t mask = 0;
	mask = (0x80 >> offset % (sizeof(uint8_t)*8)); //set the mask to the nth bit from the left side
	return (buffer[index] & mask) != 0; //if the buffer bitwise anded with the mask = return true (1) otherwise its a zero
}

/* Created with the help of Dr. MacEvoy */
bool Huffman::get_ulbit(uint64_t* buffer, unsigned short int offset)//get the bit value at byte[offset] starting at the right wiht least significant bit
{
	uint64_t mask = 0;
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

uint64_t Huffman::get_Line(ifstream& inf, string &buffer , const string &delim)
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
void Huffman::set_chbit(uint8_t* buffer, uint64_t index, unsigned short int offset, bool value) 
{
	uint8_t mask = 0;
	//we want this function to be able to overflow since it carries over to the next byte
	mask = (0x80 >> (offset % (sizeof(uint8_t)*8)));
	if(value) buffer[index] |= mask;
}

void Huffman::set_ulbit(uint64_t* buffer, unsigned short int offset, bool value)
{
	uint64_t mask = 0;
	if(offset >= sizeof(uint64_t)*8) return;
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
	uint8_t ch = 0;
	int code_length = 0;
	uint64_t* bit_code = NULL;
	uint64_t bit_count = 0;
	uint64_t index = 0;
	
	huffman_buffer = new uint8_t[H_BUF];//allocate the buffer
	for(uint64_t k=0; k<H_BUF; k++) huffman_buffer[k] = 0;
	for(uint64_t i=0; i<len; i++)
	{	
		ch = message_buffer[i]; //get the character we are encoding
		bit_code = &charTable[ch].encoding; //look up its bit_code
		code_length = charTable[ch].encodeLength; //look up the length (in bits) of the bitcode
		
		for(short int j=0; j<code_length; j++)
		{
			//if(get_ulbit(bit_code, j) == true) cout << "1";
			//else cout << "0";
			set_chbit(huffman_buffer, index, bit_count++ , get_ulbit(bit_code, j)); //write the bitcodes to the buffer
			if(bit_count%8 == 0) index++;
		}
	}
	enc_len = index+1; //index; //return the length of the huffman trie in bytes
}

void Huffman::decompress() //decompress our Huffman encoded message
{
	Trie_node* Test = NULL;
	uint8_t ch = 0;
	uint64_t bit_count = 0;
	uint64_t index = 0;
	unsigned short int length = 0;

	if(huffman_buffer == NULL) 
	{
		cerr << "Huffman buffer is empty exiting" << endl;
		return;
	}
	if(message_buffer != NULL) delete[] message_buffer; //free anything in the buffer if it is already populated
	
	message_buffer = new uint8_t[len+1]; //allocate the buffer
	for(uint64_t k=0; k<len; k++) message_buffer[k] = 0; //zero fill the buffer
	
	for(uint64_t i=0; i<len; i++) //fill every character in the message buffer after it has been decoded
	{
		bool bit = false;
		length = 0; //reset the bitcount 
		Test = root; //set the test node to the Root of the Trie
		
		while(!is_a_leaf(Test) && (length <= enc_len*8))
		{
			bit = (get_chbit(huffman_buffer, index, bit_count++));// get the current bit in the buffer
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

double Huffman::compression_percentage()
{
	return 100-(((double)h_len / (double)len) * 100); 
}

string Huffman::setMcpName(string fname)
{
	string temp =fname;
	bool found = false;
	for(unsigned int i=0; i<temp.length();i++)
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

//sstream conversion found here: http://www.cplusplus.com/forum/articles/9645/
int Huffman::readHeader()
{
	int num;
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
		getline(inf,m_number); //get the magic number
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
		get_Line(inf, table, double_delim); //Get the coding table
		get_Line(inf, enc_text, double_delim); //get the huffman code
		enc_len = enc_text.length(); //set the length of the huffman_code for later operations
		bufferHuffman(enc_text); //write the huffman code to a char* Buffer
		getline(inf, e_o_f); //get the end of file character
	}
	inf.close();
	return 1;
}

void Huffman::writeHeader() // Write the header (including huffman code)
{
	unsigned int count = 0;
	file_to_decompress = setMcpName(file_to_compress);
	ofstream outf(file_to_decompress.c_str()); //convert the file extension to .mcp
	outf << MAGIC_NUMBER << endl; //write magic number
	outf << file_to_compress << endl; //Write file name
	for(uint8_t i=0; i<UCHAR_MAX; i++) 
	{
		if(charTable[i].active) outf << charTable[i].character << charTable[i].occurrence << DELIM; //Write the active characters and their counts
		count ++;
	}
	outf << DELIM << endl; //Write char table and int with with double delimiter
	for(uint64_t i=0;i<enc_len;i++)
	{
		outf << huffman_buffer[i]; //write the huffman code 
		count ++;
	}
		outf << DELIM << DELIM << endl; //append our double delimiter to it so we know when the string is done
    	outf << O_EOF; //write our "Output" EOF character
	outf.close(); //close the filestream
	count += file_to_compress.length();
	count += 12;
	h_len = count; //sum the count for ratio calculation
}

void Huffman::writeMessage() // Write the original message back to file form buffer
{
	ofstream out(file_to_compress);
	if(out.good())
	{
		for(unsigned int i=0; i<len; i++)
		{
			out << message_buffer[i];
		}
	}
	else cerr << "Unable to open output file" << endl;
	out.close();
}

void Huffman::print_orig()
{
	
	cout << "########START UNCOMPRESSED BUFFER########\n";
	for(unsigned int i=0; i<len; i++)
	{
		cout << message_buffer[i];
	}
	cout << "\n########END UNCOMPRESSED BUFFER " << len << " characters########\n";
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
