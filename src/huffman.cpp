/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/
#include "../headers/huffman.h"
Huffman::Huffman()
{
	magic_number = 0x63130;
	file_to_compress = "";
	header_table = "";
	encoded_text = "";
	end_of_text_marker = 247;
	end_of_file = 26;
}

void Huffman::populateHeader(string hfile,string fname)
{
	ofstream outf(hfile.c_str());
	cout << "Object from other class: " << charTable[0].character << endl;
	outf << hex << magic_number << endl << fname << endl << "chars & counts here." 
	     << endl << "encoded text here." << endl << end_of_text_marker
         << endl << end_of_file;
	
}

Huffman::~Huffman() //Destructor deletes entire header
{
	//delete_header(root); need to write the function to deallocate memory
}