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

string getMcpName(string fname)
{
	return "";
}

void Huffman::compress() //compress the original message
{
	huffman_buffer = new char[len];
	for(int i=0; i<CHAR_MAX; i++)
	{
		
	}
}

int Huffman::readHeader(string hfile)
{
	int num;
	string m_number;
	string f_name;
	ifstream inf(hfile.c_str());
	if(!inf) cout << "Error reading file." << endl;
	else getline(inf,m_number);
    
	//sstream conversion found here: http://www.cplusplus.com/forum/articles/9645/
	stringstream convert(m_number); // stringstream used for the conversion initialized with the contents of the string
	if ( !(convert >> num) )//give the value to an integer using the characters in the string
		num = -1;//if that fails set integer value to 0
		
	cout << "m_number as string: " << m_number << "; and converted to an integer num: " << num << endl;
	if(num != MAGIC_NUMBER)
	{
		cout << "Not a valid compressed file. Exiting..." << endl;
		return -1;
	}
	else cout << " magic number matches!" << endl;
	return 0;
}

void Huffman::populateHeader(string hfile,string fname) // Write the header
{
	ofstream outf(hfile.c_str());
	cout << "Object from other class: " << charTable[45].character << endl;
	outf << MAGIC_NUMBER << endl << fname << endl;
	for(int i=0; i<CHAR_MAX; i++) if(charTable[i].active) outf <<  charTable[i].character << DELIM << charTable[i].occurrence;
	outf << DELIM << DELIM << endl;
	//for(int i=0;i<CHAR_MAX;i++) outf << huffman_buffer[i];
    outf << endl << O_EOF;
	outf.close();
}

Huffman::~Huffman() //Destructor deletes entire header
{
	//delete_header(root); need to write the function to deallocate memory
}
