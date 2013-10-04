/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../headers/huffman_trie.h"

Trie::Trie() //Constructor inits root node
{
	char_count = 0;
	node_count = 0;
	root = new Trie_node;
	root -> is_character = false;
	root -> val = 0;
	root -> left = NULL;
	root -> right = NULL;
}

Trie_node* Trie::insert_node(Trie_node* Root, unsigned char character, unsigned long int val)
{
	Trie_node* knew_node = NULL;
	Trie_node* knew_root = NULL;

	if(Root == NULL) return Root;
	
	knew_node = new Trie_node; //allocate storage for the new node and set the values
	knew_node -> is_character = true;
	knew_node -> character = character;
	knew_node -> val = val;
	knew_node -> left = NULL;
	knew_node -> right = NULL;
	
	knew_root = new Trie_node;
	knew_root -> is_character = false;
	knew_root -> character = 0;
	knew_root -> val = (root -> val) + val;	

	if(Root -> val == 0) //if this is the first insertion delete Root so that we don't have an extra unpopulated node
	{	
		delete Root;
	}
	else// insert new node to the right
	{
		knew_root -> left = Root;
		knew_root -> right = knew_node;
	}
	
	return knew_root; //knew_root;
}

Trie_node* Trie::insert_2nodes(Trie_node* Root, unsigned char char1, unsigned long int val1, unsigned char char2, unsigned long int val2)
{
	Trie_node* knew_root = NULL;
	Trie_node* knew_parent = NULL;
	Trie_node* knew_left = NULL;
	Trie_node* knew_right = NULL;

	if(Root == NULL) return Root; //Check if Root is NULL, if it is abort
	
	//allocate storage for the new nodes and set the values
	knew_root = new Trie_node;
	knew_root -> is_character = false;
	knew_root -> character = 0;
	knew_root -> val = (val1 + val2 + (Root -> val));
	
	knew_left = new Trie_node;
	knew_left -> is_character = true;
	knew_left -> left = NULL;
	knew_left -> right = NULL;
	
	knew_right = new Trie_node;
	knew_right -> is_character = true;
	knew_right -> left = NULL;
	knew_right -> right = NULL;
	
	knew_parent = new Trie_node;
	knew_parent -> is_character = false;
	knew_parent -> character = 0;
	knew_parent -> val = (val1 + val2);

	if(val1 >= val2)
	{
		knew_left -> character = char1;
		knew_left -> val = val1;
		knew_right -> character = char2;
		knew_right -> val = val2;
	}
	else
	{	
		knew_left -> character = char2;
		knew_left -> val = val2;
		knew_right -> character = char1;
		knew_right -> val = val1;
	}

	//set the knew_parent to point to the new children
	knew_parent -> right = knew_right;
	knew_parent -> left = knew_left;
	
	if(Root -> val == 0) //in the case this is the first node we are inserting
	{
		delete knew_root;	
		delete Root;
		return knew_parent;
	}
	if((knew_parent -> val) >= (Root -> val))
	{
		knew_root -> left = knew_parent;
		knew_root -> right = Root;
	}
	else
	{
		knew_root -> left = Root;
		knew_root -> right = knew_parent;
	}
	return knew_root;
}

void Trie::count_traverse(Trie_node* Root) //Recursively count the number of characters represented in the try **FOR DEBUGGING PURPOSES**
{
	if(Root == NULL) return; //terminate if we go outside the trie or we advance into the character section
	if(Root -> right != NULL) count_traverse(Root -> right);
	if(Root -> left != NULL) count_traverse(Root -> left);
	if(Root -> is_character == true) char_count += Root -> val; 
}

void Trie::delete_trie(Trie_node* Root) //Recursively delete Trie data structure
{
	if(Root == NULL) return;
	if(Root -> right != NULL) delete_trie(Root -> right); //Recurse down right leaf
	if(Root -> left != NULL) delete_trie(Root -> left); //Recurse down left leaf
	delete Root; //Deallocate the (current) root node
	Root = NULL; //set the pointer to Null to prevent any confusion that Root has been erased
}

void Trie::enc_traverse(Trie_node* Root, short int bit, unsigned long int bitcode, unsigned short int length) //Recursively traverse the Trie and get the bitcode for ever active character
{
	bool is_char = false;
	short int* code_length = NULL;
	unsigned char ch = 0;
	unsigned long int* code = NULL;
	unsigned long int mask = 0x8000000000000000;

	if(Root == NULL) return;
	
	if((bit == 0 || bit == 1) && length <= sizeof(unsigned long int)*8) // if we have recursed at least 1 level deep then add the bit for the direction we traversed
	{ //write bits the the buffer big endian style
		mask >>= length; //shift the mask by the depth of the traversal (number of bits)
		if(bit == 1) bitcode |= mask; //write the bitcode to buffer 
		length ++; 
	}		
	else if(length > sizeof(unsigned long int)*8) 
	{
		cerr << "Unsigned long int overflow error, cannot continue" << endl;
	}
	
	is_char = Root -> is_character;
	ch = Root -> character;
	
	if(Root -> left != NULL)
	{
		enc_traverse(Root -> left, 0, bitcode, length);
	}
	if(Root -> right != NULL)
	{
		enc_traverse(Root -> right, 1, bitcode, length);
	}
	if(is_char == true)//if node represents a character in our trie
	{
		ch = Root -> character; //Keep track of the character to look it up in our look up table
		code = &(charTable[ch].encoding); //Create a pointer to where the characters encoding is stored in the charTable
		code_length = &(charTable[ch].encodeLength); //Create a point to where the characters encoding length is stored in the charTable
		*(code) = bitcode; //set the bitcode
		*(code_length) = length; //set the code length
	}
}

void Trie::node_traverse(Trie_node* Root) //Recursively count the nodes existing in the Trie
{
	if(Root == NULL) return; //terminate recursion
	if(Root -> right != NULL) node_traverse(Root -> right); //recursively traverse to the right node
	if(Root -> left != NULL) node_traverse(Root -> left); //recursively traverse to the left node
	node_count ++;
}

void Trie::print_binary(CharNode* Node)
{
	unsigned long int binary = 0;
	unsigned long int mask = 0;
	int len = Node -> encodeLength;
	for(int i=0; i<len; i++)
	{
		mask = (0x8000000000000000 >> i); 
		binary = (Node -> encoding) & mask;
		if(binary != 0) printf("1");
		else printf("0");
	}
}

void Trie::printBinary(){
	//unsigned long int temp = 0;
	//int len = 0;
	for(unsigned char i=0;i<UCHAR_MAX;i++)
	{
		//temp = charTable[i].encoding;
		//len = charTable[i].encodeLength;
		if(charTable[i].active == true)
		{
			cout << "[" << i << ":";
			print_binary(&charTable[i]);
			cout << "]" << endl;
		}
	}
}
unsigned long int Trie::sum_nodes(Trie_node* Root, unsigned long int sum)
{	
	if(Root == NULL) return sum;
	sum += sum_nodes(Root->left, sum);
	sum += sum_nodes(Root->right, sum);
	sum += Root -> val;
	return sum;
}

unsigned long int Trie::character_count() //Recursively count the number of characters represented by the Trie **FOR DEBUGGING**
{
	count_traverse(root);
	return char_count;
}

unsigned long int Trie::size_of_trie() //Recursively count the number of nodes i the trie **FOR DEBUGGING**
{
	node_traverse(root);
	return node_count;
}

void Trie::populate_trie() //Fills the trie data structure with sorted character and character occurences from the sort table
{
	bool even = false;
	unsigned char char1 = 0;
	unsigned char char2 = 0;
	unsigned long int occur1 = 0;
	unsigned long int occur2 = 0;
	
	if(table_len %2 == 0) even = true; 
	for(unsigned char i=(UCHAR_MAX-table_len); i<UCHAR_MAX; i++) 
	{
		char1 = (((CharNode*) &*(charSort[i]))->character);
		occur1 = (((CharNode*) &*(charSort[i]))->occurrence);
			if(even) //if there are an even number of nodes we can insert the entire trie with insert2
			{
				char2 = (((CharNode*) &*(charSort[i+1]))->character);
				occur2 = (((CharNode*) &*(charSort[i+1]))->occurrence);
				root = insert_2nodes(root, char1, occur1, char2, occur2);
				i++; //increment i so that we don't double insert a node
			}
			else
			{
				if(i < (UCHAR_MAX-1))
				{
					char2 = (((CharNode*) &*(charSort[i+1]))->character);
					occur2 = (((CharNode*) &*(charSort[i+1]))->occurrence);
					root = insert_2nodes(root, char1, occur1, char2, occur2);
					i++; //increment i so that we don't double insert a node
				}
				else 
				{
					root = insert_node(root, char1, occur1); //insert the last node with insert1
				}
			}
	}
}

void Trie::get_encoding() //Recursively traverse the Trie and write the bitstring and length for every character in it
{
	enc_traverse(root, -1, 0, 0);	
}

void Trie::print_encoding_table()
{
	cout << endl << "Encoding Table" << endl;
	for(unsigned char i=0; i<UCHAR_MAX; i++)
	{
		if(charTable[i].active == true)
		{
			cout << "|" << i << ":";
			print_binary(&charTable[i]);
			cout << "| ";
		}
	}
}

void Trie::print_sort_table() //prints the active characters and their occurence in sorted order
{
	for(unsigned char l=0; l<UCHAR_MAX; l++)
        {
                if(((CharNode*) &*(charSort[l]))->occurrence > 0)
                {
                        cout << "|\"" << ((CharNode*) &*(charSort[l]))->character << "\":" << ((CharNode*) &*(charSort[l]))->occurrence << "|  ";
                }
        }
}

Trie::~Trie() //Destructor deletes entire trie
{
	delete_trie(root);
}
