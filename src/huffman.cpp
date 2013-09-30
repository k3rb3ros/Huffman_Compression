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

void Huffman::bit_write(char &src, char &dest, const short int bits) //write the number of bits passed into the function from char src to char dest
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

short int Huffman::get_byte(int length)
{
	return ceil(((sizeof(unsigned long int)*8)-length)/8);
}

string getMcpName(string fname)
{
	return "";
}

void Huffman::compress() //compress the original message
{
	bool carry = false;
	bool refill = false;
	char ch = 0;
	char carry_over = 0;
	char* write_block = NULL;
	int code_length = 0;
	int local_length = 0;
	int index = 0;
	short int byte = 0;
	unsigned long int bit_code = 0;
	unsigned long int* zed = NULL;

	huffman_buffer = new char[len];//allocate the buffer
	zed = (unsigned long int*)huffman_buffer; //set zed to point to the buffer
	for(short int l=0; l<(CHAR_MAX/sizeof(unsigned long int)); l++) zed[l] = 0; //zeropad the buffer (faster when done with size of system register)
	
	for(unsigned long int i=0; i<len; i++)
	{
		ch = file_buffer[i]; //get the character we are encoding
		bit_code = charTable[ch].encoding; //look up its bit_code
		code_length = charTable[ch].encodeLength; //look up the length (in bits) of the bitcode		

		byte = get_byte(code_length); //get the index in the byte buffer that binary is stored
		//shift the index until it is left aligned with a byte boundry 
		
		if(carry)
		{
		}
		if(refill)
		{
		}
		//code that writes bytes to buffer goes here
	}
	cout << "Done!" << endl;
}

void Huffman::populateHeader(string hfile,string fname) // Write the header
{
	ofstream outf(hfile.c_str());
	cout << "Object from other class: " << charTable[45].character << endl;
	outf << MAGIC_NUMBER << endl << fname << endl;
	for(int i=0; i<CHAR_MAX; i++) if(charTable[i].active) outf <<  charTable[i].character << DELIM << charTable[i].occurrence;
	outf << DELIM << DELIM << endl;
	for(int i=0;i<CHAR_MAX;i++) outf << huffman_buffer[i];
    outf << endl << O_EOF;
	
}

void Huffman::test()
{
	char testsrc[16] {0X1, 0X3, 0X7, 0XF, 0X1F, 0X3F, 0X7F, 0XFF, 0X1, 0X2, 0X4, 0X8, 0X10, 0X20, 0X40, 0X80};
	char testdest = 0;
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
	cout << "bit_write test passed" << endl;
	
	for(int i=1; i<=64; i++) //test the ceiling function to calculate the byte
	{
		assert(get_byte(i) == test_byte);
		if(i%8==0) test_byte --;
		//cout << "i:" << i << " gb():" << get_byte(i) << " :test" << test_byte << endl;
	}
	cout << "get_byte test passed" << endl;
}	

Huffman::~Huffman() //Destructor deletes entire header
{
}
