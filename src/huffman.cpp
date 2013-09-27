/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/
#include "../headers/huffman.h"
Huffman::Huffman()
{
	//magic_number = 0x63130;
	file_to_compress = "";
	header_table = "";
	encoded_text = "";
}

void Huffman::populateHeader(string hfile,string fname)
{
	ofstream outf(hfile.c_str());
	cout << "Object from other class: " << charTable[45].character << endl;
	outf << MAGIC_NUMBER << endl << fname << endl;
	for(int i=0; i<CHAR_MAX; i++) if(charTable[i].active) outf <<  charTable[i].character << DELIM << charTable[i].occurrence;
	outf << DELIM << DELIM << endl;
	for(int i=0;i<CHAR_MAX;i++) outf << huffman_buffer[i];
    outf << endl << O_EOF;
	
}

Huffman::~Huffman() //Destructor deletes entire header
{
	//delete_header(root); need to write the function to deallocate memory
}