/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../headers/huffman_trie.h"

Trie_node* Trie::insert_node(Trie_node* Root, char character, unsigned long int val)
{
	Trie_node* knew_node = NULL;
	Trie_node* knew_root = NULL;
	unsigned long int sum = 0;
	if(Root == NULL) return Root;
	
	sum = sum_nodes(Root, 0); //Sum all the weights of the current root node

	knew_node = new Trie_node; //allocate storage for the new node and set the values
	knew_node -> is_character = true;
	knew_node -> character = character;
	knew_node -> val = val;
	knew_node -> left = NULL;
	knew_node -> right = NULL;
	
	knew_root = new Trie_node;
	knew_root -> is_character = false;
	knew_root -> character = 0;
	knew_root -> val = sum + val;	
	
	if(sum > val) //insert new node to the left
	{
		knew_root -> left = knew_node;
		knew_root -> right = Root;
	}
	else// insert new node to the right
	{
		knew_root -> left = Root;
		knew_root -> right = knew_node;
	}
	
	return knew_root; //knew_root;
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

void Trie::enc_traverse(Trie_node* Root, stack<int> huffman)
{
	bool is_char = false;
	bool reallocate = false;
	char ch = 0;
	CharBucket* bucket = NULL;
	short int direction = 0;
	int* code_length = NULL;
	unsigned long int* code = NULL;
	//unsigned long int code = 0;
	if(Root == NULL) return;
	is_char = Root -> is_character;
	ch = Root -> character;
	if(Root -> left != NULL)
	{
		huffman.push(0);
		enc_traverse(Root -> left, huffman);
	}
	if(Root -> right != NULL)
	{
		huffman.push(1);
		enc_traverse(Root -> right, huffman);
	}
	if(is_char == true)//if node represents a character in our trie
	{
		ch = Root -> character; //Keep track of the character to look it up in our look up table
		code = &(charTable[ch].encoding); //Create a pointer to where the characters encoding is stored in the charTable
		code_length = &(charTable[ch].encodeLength); //Create a point to where the characters encoding length is stored in the charTable
		
		while(!huffman.empty())
		{
			direction = huffman.top(); //get the 0 or 1 on the top of the stack
			huffman.pop(); //pop it off the stack
			if(false)//(*(code_length) | 0x8000000000000000) == 0x8000000000000000) //if we are about to overflow our long unsigned int
			{
				cerr << "bitcode not exectuting" << endl;
				//cout << "code: " << *(code) << " length: " << *(code_length) << endl;
				//FIXME
			}
			else
			{
				*(code) = *(code) | direction; //Add the least significant bit to the buffer 
				*(code) = *(code) << 1; //bitshift whatever is in the buffer to the left by 1 bit
				*(code_length) ++;
			}
		}
		cout << "Code for \"" << ch << "\":" << hex << *(code) << dec << endl;
	}

}

void Trie::node_traverse(Trie_node* Root) //Recursively count the nodes existing in the Trie
{
	if(Root == NULL) return; //terminate recursion
	if(Root -> right != NULL) node_traverse(Root -> right); //recursively traverse to the right node
	if(Root -> left != NULL) node_traverse(Root -> left); //recursively traverse to the left node
	node_count ++;
}

unsigned long int Trie::sum_nodes(Trie_node* Root, unsigned long int sum)
{	
	if(Root == NULL) return sum;
	sum += sum_nodes(Root->left, sum);
	sum += sum_nodes(Root->right, sum);
	sum += Root -> val;
	return sum;
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

void Trie::populate_trie()
{
	char character = 0;
	unsigned long int occurrence = 0;
	for(char i=0; i<CHAR_MAX; i++)
	{
		character = (((CharNode*) &*(charSort[i]))->character);
		occurrence = (((CharNode*) &*(charSort[i]))->occurrence);
		if(occurrence > 0)
		{
			root = insert_node(root, character, occurrence);
		}
	}
}

void Trie::get_encoding() //Recursively traverse to the character creating the bitstring encoding based on left and right traversals
{	
	stack<int> code;
	enc_traverse(root, code);	
}

void Trie::print_encoding_table()
{
	cout << endl << "Encoding Table" << endl;
	for(char i=0; i<CHAR_MAX; i++)
	{
		if(charTable[i].active == true)
		{
			cout << "|" << i << ":0x" << hex << charTable[i].encoding << dec << "| ";
		}
	}
}

void Trie::print_sort_table() //prints the active characters and their occurence in sorted order
{
	for(char l=0; l<CHAR_MAX; l++)
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
	/*for(int i=0; i<CHAR_MAX; i++) //Recursively delete any buckets that were allocated on the heap
	{
		delete_buckets(enc_table[i].buckets); 
	}*/
}
