/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../headers/headers.h"
#include "../headers/mcp.h"

void Mcp::compress(char* file_name)
{
	string f_name(file_name);
	Huffman huffman; //create the Hffman object
	huffman.bufferMessage(f_name); //buffer the message to ram
	cout << "Message buffered" << endl;
	huffman.CompPopulateTable(); //Populate the lookup table with characters and counts
	cout << "Table populated" << endl;
	huffman.sortTable(); //Sort the table by occurrence
	cout << "Table sorted" << endl;
	huffman.populate_trie(); //Make and populate the huffman trie
	cout << "Trie built" << endl;
	if(!huffman.get_encoding()) //Attempt to generate the bitcodes
	{
		 cerr << "Overflow of 64 bit data type unable to generate bitcodes" << endl;
		return; 
	}
	cout << "Bit codes generated"  << endl;
	cout << "Compressing..." << endl;
	huffman.compress(); //Compress the contents of the file to bitcode
	huffman.writeHeader(); //Write the header and compressed message to file
	cout << "Done!" << endl;
	cout << "Compression ratio " << huffman.compression_percentage() << "%" << endl;

}

void Mcp::decompress(char* file_name)
{
	string f_name(file_name);	
	Huffman huffman;
		
	huffman.setFileToDecompress(file_name);
	if(huffman.readHeader())
	{
		huffman.DecompPopulateTable();
		cout << "Lookup Table populated" << endl;
		huffman.sortTable();
		cout << "Lookup Table sorted by ocurrence" << endl;
		huffman.printSorted();
		huffman.populate_trie();
		cout << "Trie built" << endl;
		huffman.decompress();
		cout << "File Decompressed" << endl;
		huffman.printBinary();
		huffman.print_orig();
		huffman.writeMessage();
	}
	else cerr << "Invalid .mcp file unable to decompress" << endl;
}
