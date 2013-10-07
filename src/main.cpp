/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../src/mcp.h"

int main(int argv, char **argc)
{	
	MCP mcp;
	if(argv == 2){
		if(argc[1][0] == '-' && argc[1][1] = 'c' || argc[1][1] = 'C')
		{
			mcp.compress(argc[2]);
		}
		else if(argc[1][0] == '-' && argc[1][1] = 'd' || argc[1][1] = 'D')
		{
			mcp.decompress(argc[2]);
		}
	}
	else cout << "Usage: mcp -c <filename> for compress or mcp -d filename for decompress" << endl;
	
	
	return 0;
}
