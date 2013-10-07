/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../headers/mcp.h"

int parse_flag(char* arg_flag)
{
	if(strlen(arg_flag) == 2 && arg_flag[0] == '-' && (arg_flag[1] == 'c' || arg_flag[1] == 'C')) return 1; //compress
	if(strlen(arg_flag) == 2 && arg_flag[0] == '-' && (arg_flag[1] == 'd' || arg_flag[1] == 'D')) return 2; //decompress
	return -1;
}

void print_usage()
{
	cout << "Usage: mcp -c <filename> for compress or mcp -d filename for decompress" << endl;
}

int main(int argv, char **argc)
{	
	Mcp mcp;
	if(argv == 3)
	{
		if(parse_flag(argc[1]) == 1)
		{
			cout << "Attempting to compress " << argc[2] << endl;
			mcp.compress(argc[2]);
		}
		else if(parse_flag(argc[1]) == 2)
		{
			cout << "Attempting to decompress " << argc[2] << endl;
			mcp.decompress(argc[2]);
		}
		else
		{
			print_usage();
			return -1;
		}
	}
	else 
	{
		print_usage();
		return -1;
	}
	return 0;
}
