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
bool Huffman::getbit(unsigned long int* buffer, short int offset)
{
	unsigned long int mask = 0;
	if(offset >= MAX_BIT_SIZE) return false;
	mask = (1 << offset);
	return (*buffer & mask) != 0;
}

void Huffman::setbit(unsigned char* buffer, int index, short int offset, bool value)
{
	unsigned long int mask = 0;
	offset = offset % 8; //wrap the bits around
	if(offset >= MAX_BIT_SIZE) return;
	mask = (1 << (offset % 8));
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
	return "";
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
		for(unsigned short int j=0; j<(code_length); j++)
		{
			setbit(huffman_buffer, index, bit_count++ , getbit(bit_code, j)); //write the bitcodes to the buffer
			if(bit_count%8 == 0) index++;
		}
	}
	h_len = index+1; //index; //return the length of the huffman trie in bytes
}

int Huffman::readHeader(string hfile)
{
	int num;
	int i=0;
	int f_len = hfile.size();
	cout << endl << endl << "  charmax = " << UCHAR_MAX << endl << endl;
	string m_number;
	string f_name;
	string table;
	string e_o_f;
	ifstream inf(hfile.c_str());
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
		//getline(inf,table);	
		//cout << " Table: " << endl << table;
		for (int i=0;i<UCHAR_MAX;i++) // trouble reading in table with getline and double delimiter ... 762 retrieves complete table.
		{			
			inf >> table[i];
			if(table[i]=='ª' && table[i+1] == '«')
			{
				i+=2;
				cout << ' ';
			}
			cout << table[i];
			i++;
		} 
		getline(inf, e_o_f);
		cout  << endl  << "eof: " << e_o_f; // not working...
	}
	return 0;
}

void Huffman::populateHeader(string hfile,string fname) // Write the header
{
	ofstream outf(hfile.c_str());
	outf << MAGIC_NUMBER << endl << fname << endl;
	for(int i=0; i<UCHAR_MAX; i++) if(charTable[i].active) outf <<  charTable[i].character << DELIM << charTable[i].occurrence;
	outf << DELIM << DELIM << endl;
	//for(int i=0;i<UCHAR_MAX;i++) outf << huffman_buffer[i]; // DON'T DELETE!!
    	outf << endl << O_EOF;
	outf.close();
}

void Huffman::print_huffman()
{
	for(unsigned int i=0; i<4; i++)
	{
		cout << "\\" <<  (int)huffman_buffer[i];
	}
	cout << endl << "Length of huffman codeing = " << h_len << " characters" << endl;
}

void Huffman::test()
{
	unsigned char testsrc[16] {0X1, 0X3, 0X7, 0XF, 0X1F, 0X3F, 0X7F, 0XFF, 0X1, 0X2, 0X4, 0X8, 0X10, 0X20, 0X40, 0X80};
	unsigned char testdest = 0;
	short int bits = 0;
	short int test_byte  = 7;
	for(int i=0; i<16; i++) //test bit_write
	{
		if(i<8) bits = (i+1);
		else bits = (i+1)-8;
		bit_write(testsrc[i], testdest, bits);
		//cout << dec << "bits written = " << bits << endl;
		//cout << hex << "testsrc = 0x" << (int)testsrc[i] << endl;
		//cout << hex << "testdest = 0x" << (int)testdest << endl;	
		assert(testdest == testsrc[i]);
		testdest = 0;
	}
	cout << "bit_write tests passed" << endl;
	
	for(int i=1; i<=64; i++) //test the ceiling function to calculate the byte
	{
		assert(get_byte(i) == test_byte);
		if(i%8==0) test_byte --;
		//cout << "i:" << i << " gb():" << get_byte(i) << " :test" << test_byte << endl;
	}
	cout << "get_byte tests passed" << endl;

 	assert(get_offset(15) == 1);
	assert(get_offset(12) == 4);
	assert(get_offset(16) == 0);
	assert(get_offset(1) == 7);
	cout << "get_offset tests passed" << endl;

	assert(get_bit(2, 15) == 1);
	assert(get_bit(4, 12) == 0);
	assert(get_bit(2, 14) == 0);
	assert(get_bit(5, 15) == 4);
	assert(get_bit(2, 10) == -4);	
	cout << "get_bit tests passed" << endl;
	
	cout << "Dumping file_buffer to file " << endl;
	dump_buffer();
}	

Huffman::~Huffman() //Destructor deletes entire header
{
}
