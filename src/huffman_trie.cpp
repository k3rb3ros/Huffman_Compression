/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../headers/huffman_trie.h"


void Trie::count_traverse(Trie_node* Root) //Recursively count the number of characters represented in the try **FOR DEBUGGING PURPOSES**
{
	if(Root == NULL || Root -> is_character == true) return; //terminate if we go outside the trie or we advance into the character section
	if(Root -> right != NULL) count_traverse(Root -> right);
	if(Root -> left != NULL) count_traverse(Root -> left);
	char_count += Root -> val; 
}

void Trie::delete_buckets(Enc_node* bkt)
{
	if(bkt == NULL) return;
	delete_buckets(bkt -> buckets);
	bkt -> buckets = NULL;
	delete bkt; 
}

void Trie::delete_trie(Trie_node* Root) //Recursively delete Trie data structure
{
	if(Root == NULL) return;
	if(Root -> right != NULL) delete_trie(Root -> right); //Recurse down right leaf
	if(Root -> left != NULL) delete_trie(Root -> left); //Recurse down left leaf
	delete Root; //Deallocate the (current) root node
	Root = NULL; //set the pointer to Null to prevent any confusion that Root has been erased
}

void Trie::enc_traverse(Trie_node* Root, stack<int> huffman, int code_length)
{
	bool is_char = false;
	char ch = 0;
	unsigned long int direction = 0;
	unsigned long int code = 0;
	if(Root == NULL) return;
	is_char = Root -> is_character;
	ch = Root -> character;
	if(Root -> left != NULL)
	{
		huffman.push(0);
		enc_traverse(Root -> left, huffman, code_length++);
	}
	if(Root -> right != NULL)
	{
		huffman.push(1);
		enc_traverse(Root -> right, huffman, code_length++);
	}
	if(is_char == true)//if node represents a character in our trie
	{
		enc_table[ch].active = true; //activate that node in the table 
		while(!huffman.empty())
		{
			direction = huffman.top(); //get the 0 or 1 on the top of the stack
			huffman.pop(); //pop it off the stack
			if(code_length < 64)
			{
				code = code << 1; //bitshift whatever is in the buffer to the left by 1 bit
				code = code | direction; //Add the least significant bit to the buffer 
			}
			else
			{
				//FIXME
			}
			//bitshift anyhting in code to the left
			code = code | direction; //bitwise and it with code
		}
	}

}
void Trie::node_traverse(Trie_node* Root) //Recursively count the nodes existing in the Trie
{
	if(Root == NULL) return; //terminate recursion
	else if(Root -> left == NULL && Root -> right == NULL)
	{
		node_count ++;
		return; //then only Root node exists so it is the only node
	}
	if(Root -> right != NULL) Root = Root -> right; //recursively traverse to the right node
	if(Root -> left != NULL) Root = Root -> left; //recursively traverse to the left node
	node_count ++;
}

Trie::Trie() //Constructor inits root node
{
	char_count = 0;
	node_count = 0;
	root = new Trie_node;
	root -> is_character = false;
	root -> val = 0;
	root -> left = NULL;
	root -> right = NULL;
	for(int i=0; i<CHAR_MAX; i++) //Initialize the encoding table
	{
		enc_table[i].active = false;
		enc_table[i].encoding = 0;
		enc_table[i].length = 0;
		enc_table[i].buckets = NULL; 
	}
}
unsigned long int Trie::character_count() //Recursively count the number of characters represented by the Trie **FOR DEBUGGING**
{
	count_traverse(root);
	return char_count;
}

void Trie::get_encoding() //Recursively traverse to the character creating the bitstring encoding based on left and right traversals
{	
	stack<int> branch;
	enc_traverse(root, branch, 0);	
}

unsigned long int Trie::size_of_trie() //Recursively count the number of nodes i the trie **FOR DEBUGGING**
{
	node_traverse(root);
	return node_count;
}

Trie::~Trie() //Destructor deletes entire trie
{
	delete_trie(root);
	for(int i=0; i<CHAR_MAX; i++) //Recursively delete any buckets that were allocated on the heap
	{
		delete_buckets(enc_table[i].buckets); 
	}
}
