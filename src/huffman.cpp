/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/
#include "../headers/huffman.h"

Huffman::Huffman()
{
	file_to_compress = "";
	header_table = "";
	encoded_text = "";
}

void Huffman::bit_write(unsigned char &src, unsigned char &dest, const short int bits) //write the number of bits passed into the function from unsigned char src to unsigned char dest
{
	enum bit_codes{ BIT8 = 0x80, BIT7 = 0x40, BIT6 = 0X20, BIT5 = 0X10, BIT4 = 0X8, BIT3 = 0X4, BIT2 = 0X2, BIT1 = 0X1 };
	switch(bits)
	{
		case 8:
		dest |= (src & BIT8);
		case 7:
		dest |= (src & BIT7);
		case 6:
		dest |= (src & BIT6);
		case 5:
		dest |= (src & BIT5);
		case 4:
		dest |= (src & BIT4);
		case 3:
		dest |= (src & BIT3);
		case 2:
		dest |= (src & BIT2);
		case 1:
		dest |= (src & BIT1);
		break;
		
		default:
		break;
	}
}

void Huffman::dump_buffer()
{
	ofstream write("test.dat");
	for(unsigned int i=0; i<len; i++)
	{
		write << "/" << (int)file_buffer[i];
	}
	write.close();
}

/* Created with the help of Dr. MacEvoy */
bool Huffman::getbit(unsigned long int* buffer, short int offset)//get the bit value at byte[offset] starting at the right wiht least significant bit
{
	unsigned long int mask = 0;
	if(offset >= MAX_BIT_SIZE) return false;
	mask = (1 << offset);
	return (*buffer & mask) != 0;
}

/* Created with the help of Dr. MacEvoy */
void Huffman::setbit(unsigned char* buffer, int index, short int offset, bool value) 
{
	unsigned long int mask = 0;
	if(offset >= MAX_BIT_SIZE) return;
	mask = (0x80 >> ((offset % 8)-1));
	if(value) buffer[index] |= mask;
}

short int Huffman::get_bit(int l_carry, short int length)
{		
	short int offset = (8 - (length % 8));
	if(offset == 8) offset = 0;
	return (l_carry - offset);
}

short int Huffman::get_byte(int length)
{
	return ceil(((sizeof(unsigned long int)*8)-length)/8);
}

short int Huffman::get_offset(int length)
{
	short int offset = (8 - (length % 8));
	if(offset == 8) offset = 0;
	return offset;
}

string getMcpName(string fname)
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
	bool write_over = false;
	unsigned char ch = 0;
	int code_length = 0;
	int index = 0;
	short int offset = 0;
	unsigned long int* bit_code = NULL;
	unsigned long int bit_count = 1;
	
	huffman_buffer = new unsigned char[H_BUF];//allocate the buffer
	for(unsigned short int k=0; k<H_BUF; k++) huffman_buffer[k] = 0;
	
	for(unsigned long int i=0; i<len; i++)
	{	
		ch = file_buffer[i]; //get the character we are encoding
		bit_code = &charTable[ch].encoding; //look up its bit_code
		code_length = charTable[ch].encodeLength; //look up the length (in bits) of the bitcode
		
		for(short int j=code_length-1; j>=0; j--)
		{
			setbit(huffman_buffer, index, bit_count++ , getbit(bit_code, j)); //write the bitcodes to the buffer
			if(bit_count%8 == 0) index++;
		}
	}
	if(huffman_buffer[index] != 0) huffman_buffer[index++]; //nullpad the buffer if it isn't already
	h_len = index+1; //index; //return the length of the huffman trie in bytes
}

void Huffman::table_char_count(string table)
{	
	string temp_str;
	string str;
	int temp_num=0;
	
	//insert the characters and their counts into a vector in order to split them from the string
	for(int i=0; i< table.length();i++)
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
	for(int j=0; j < mylist.size(); j++)
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

string setMcpName(string fname)
{
	int i=0;
	string temp;
	while (!fname.length())
	{	
		if(fname[i] == '.') 
		{
			fname[i+1]='m';
			fname[i+1]='c';
			fname[i+1]='p';
		}
		i++;
	}
	cout << "temp = " << temp << endl;
	return temp = fname;
}

int Huffman::readHeader(string hfile)
{
	int num;
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

void Huffman::populateHeader(string hfile,string fname) // Write the header
{
	ofstream outf(hfile.c_str());
	outf << MAGIC_NUMBER << endl; //write magic number
	outf << fname << endl; //Write file name

	//Write char table and int with with double delimiter
	for(int i=0; i<UCHAR_MAX; i++) if(charTable[i].active) outf <<  charTable[i].character << DELIM << charTable[i].occurrence << ' ' ;
	outf << DELIM << DELIM << endl; 

	//Write encoded text to header file
	for(int i=0;i<h_len;i++)
	{
		outf << huffman_buffer[i];
	}
	outf << endl;
	
	outf << O_EOF; //write our "Output" EOF character
	outf.close(); //close the filestream
}

void Huffman::print_huffman()
{
	for(unsigned int i=0; i<h_len; i++)
	{
		//cout << "\\" <<  (int)huffman_buffer[i];
	}
	cout << endl << "Length of huffman codeing = " << h_len << " characters" << endl;
}

void Huffman::test()
{
}	

Huffman::~Huffman() //Destructor deletes entire header
{
}
