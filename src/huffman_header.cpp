/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/
#include "../headers/huffman_header.h"

HuffmanHeader::HuffmanHeader()
{
	int magic_number = 0x63130707;
	string file_to_compress = "";
	string header_table = "";
	string encoded_text = "";
	char end_of_text_marker = 247;
	char eof = 26;
}
void HuffmanHeader::populateHeader(string fname)
{
	cout << " magic_number = " << magic_number << endl;
	
	//cout << "Test reached..." << endl;
}
HuffmanHeader::~HuffmanHeader() //Destructor deletes entire header
{
	//delete_header(root); need to write the function to deallocate memory
}