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
	for(int i=0; i<len; i++)
	{
		write << "/" << (int)file_buffer[i];
	}
	write.close();
	
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

string getLine(ifstream &inf, string delim)
{
    //char* c = calloc(260, sizeof(char)); //Allocate enough storage for any valid line
    //unsigned char c2 = 0;
    //int i = 0;
    //do //Get what the user types until ENTER is pressed
    //{
    //    c2 = getchar();
    //    c[i++] = c2;
    //}
    //while ((c2 != '\n') && (i < 259));
    //c[i-1] = 0; //Eat the newline character from the enter press so it doesn't screw up later stdin inputs
    //return c; //return the string of the line you have just gotten
}

string getMcpName(string fname)
{
	return "";
}

void Huffman::compress() //compress the original message
{
	bool carry = false;
	unsigned char ch = 0;
	unsigned char carry_over = 0;
	unsigned char* write_block = NULL;
	int code_length = 0;
	int index = 0;
	short int byte = 0;
	short int offset = 0;
	short int reallign = 0;
	unsigned long int bit_code = 0;
	//unsigned long int* zed = NULL;

	huffman_buffer = new unsigned char[H_BUF];//allocate the buffer
	//zed = (unsigned long int*)huffman_buffer; //set zed to point to the buffer
	for(short int k=0; k<H_BUF; k++) huffman_buffer[k] = 0;
	//for(short int l=0; l<(H_BUF/sizeof(unsigned long int)); l++) zed[l] = 0; //zeropad the buffer (faster when done with size of system register)
	
	for(unsigned long int i=0; i<len; i++)
	{
		ch = file_buffer[i]; //get the character we are encoding
		bit_code = charTable[ch].encoding; //look up its bit_code
		code_length = charTable[ch].encodeLength; //look up the length (in bits) of the bitcode
		write_block = (unsigned char*)&bit_code; //point write_block to the bitcode
		byte = get_byte(code_length); //get the index in the byte buffer that binary is stored
		offset = get_offset(code_length); //Calculate the offset to byte allign the bit_code
		
		if(carry == false) //byte align bit_code
		{
			bit_code <<= offset;
		}
		else //write the first few bits to byte left over from the previous encoding
		{
			index++;
			reallign = get_bit(reallign, code_length);	//figure out the spacing between the last bit code and next one
			if(reallign <0) //if there is extra space adjust the bitcode left
			{
				reallign *= -1; //invert the value
				bit_code <<= reallign; //shift the bits left to fill the byte
				bit_write(write_block[byte], carry_over, reallign-1); //write the bits to carry_over byte
				huffman_buffer[index++] = carry_over; //write carry_over byte to the buffer
				offset = (8-reallign); //update offset
				code_length -= offset; //adjust the code length
				if(code_length >0) byte++; //move to the next byte if we haven't written the entire bitstring already
			}
			else if (reallign >0) //if there isn't enough space shift the next bitcode right
			{
				bit_code >>= reallign; //shift the bits right to make align the 2nd bitcode
				bit_write(write_block[byte], carry_over, reallign-1); //write the bits to carry_over byte
				huffman_buffer[index++] = carry_over; //write carry_over byte to the buffer
				offset = (8-reallign-1); //update the offset for future calculations
				code_length -= offset; //update the length for future calculations
				bit_code = charTable[ch].encoding; //reload the bitstring to get the bits we lost by right shifting
				bit_code <<= offset; //Shift left to get rid of the 
				//don't increment byte as we need to re-read that part of the code 
			}
			else
			{
				bit_write(write_block[byte], carry_over, reallign-1); //write the bits to carry_over byte
				huffman_buffer[index++] = carry_over; //carry_over to buffer
				code_length -= offset; //update length for future calculations	
				if(code_length >0) byte++; //move to the next byte if we haven't written the entire bitstring already
			}
			carry_over = 0;
			carry == false;
		}
		while(!(code_length < 8))
		{
			huffman_buffer[index++] = write_block[byte++]; //copy the code to buffer as raw binary while we can
			code_length -= 8; //decrement local length by one byte
		}
		if(code_length > 0) //if the remaining bit code isn't at a byte boundry then use our bit_write to write the remaining bits to carry_over byte
		{
			carry = true;
			carry_over = write_block[byte];
			reallign = code_length;
			code_length = 0;
		}
	}
	if(carry == true) //If their is a carry over after for the final encoded character copy it with null padding to the buffer
	{
		huffman_buffer[index++] = carry_over;
	}
		huffman_buffer[index] = 0; //add a null padded character to let us know this is the end of the huffman code
	h_len = index; //record the length in charlist
	
	cout << "Done compressing" << endl;
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
	for(int i=0; i<h_len; i++)
	{
		cout << huffman_buffer[i];
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

	cout << "Dumping file_buffer to file " << endl;
	dump_buffer();
}	

Huffman::~Huffman() //Destructor deletes entire header
{
}
