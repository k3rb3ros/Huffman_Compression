/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
* for CSCI 480
* September 2014
*/

#include "../headers/huffman_trie.h"

Trie_node* Trie::insert_node(Trie_node* Root, Trie_node* New_node)
{
	Trie_node* knew_root = NULL;
	
	if(Root == NULL || New_node == NULL) return Root; //check for valid input and return otherwise
	
	knew_root = new Trie_node; //allocate the new node
	knew_root -> is_character = false;
	knew_root -> val = (Root -> val + New_node -> val);
	knew_root -> character = 0;
	if(Root -> val >= New_node -> val)
	{
		knew_root -> left = New_node;
		knew_root -> right = Root;
	}
	else
	{
		knew_root -> left = Root;
		knew_root -> right = New_node;
	}
	return knew_root;
}

Trie_node* Trie::insert_node(Trie_node* Root, unsigned char character, unsigned long int val)
{
	Trie_node* knew_node = NULL;
	Trie_node* knew_root = NULL;
	unsigned long int sum = 0;

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
	knew_root -> val = sum + val;	
	
	if((Root -> val) >= val) //insert new node to the left
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

Trie_node* Trie::insert_2nodes(Trie_node* Root, unsigned char char1, unsigned long int val1, unsigned char char2, unsigned long int val2)
{
	Trie_node* knew_root = NULL;
	Trie_node* knew_left = NULL;
	Trie_node* knew_right = NULL;

	if(Root == NULL) return Root; //Check if Root is NULL, if it is abort
	
	//allocate storage for the new nodes and set the values
	knew_root = new Trie_node;
	knew_root -> is_character = false;
	knew_root -> character = 0;
	knew_root -> val = val1 + val2 + (Root -> val);
	knew_left = new Trie_node;
	knew_left -> is_character = true;
	knew_left -> left = NULL;
	knew_left -> right = NULL;
	knew_right = new Trie_node;
	knew_right -> is_character = true;
	knew_right -> left = NULL;
	knew_right -> right = NULL;

	if(char1 <= char2)
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

	//set the root to point to the knew children
	knew_root -> left = knew_left;
	knew_root -> right = knew_right;
	
	return insert_node(Root, knew_root); //recursively insert the sub tree into root
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
	//bool reallocate = false;
	unsigned char ch = 0;
	//CharBucket* bucket = NULL;
	short int direction = 0;
	int* code_length = NULL;
	unsigned long int* code = NULL;
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
		
		int temp = 0;/*DEBUG*/
		while(!huffman.empty())
		{
			direction = huffman.top(); //get the 0 or 1 on the top of the stack
			huffman.pop(); //pop it off the stack
			if((*(code_length) & 0x8000000000000000) == 0x8000000000000000) //if we are about to overflow our long unsigned int (small endian only)
			{
				cerr << "bitcode not exectuting" << endl;
				//FIXME
			}
			else
			{
				*(code) = *(code) | direction; //Add the least significant bit to the buffer 
				*(code) = *(code) << 1; //bitshift whatever is in the buffer to the left by 1 bit
				temp ++;
				(*code_length) ++;
			}
		}
		//cout << "Code for \"" << ch << "\":" << hex << *(code) << dec << " " << temp << endl;
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
	unsigned long int binary = Node -> encoding;
	int len = Node -> encodeLength;
	for(int i=len-1; i>=0; i--)
	{
		printf("%lu", (binary >> i) % 2);
	}
}

void Trie::printBinary(){
	//unsigned long int temp = 0;
	//int len = 0;
	for(int i=0;i<UCHAR_MAX;i++){
		//temp = charTable[i].encoding;
		//len = charTable[i].encodeLength;
		if(charTable[i].active == true)
		{
			cout << charTable[i].character << ": ";
			print_binary(&charTable[i]);
			cout << endl;
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

void Trie::populate_trie() //Fills the trie data structure with sorted character and character occurences from the sort table
{
	bool even = false;
	unsigned char char1 = 0;
	unsigned char char2 = 0;
	unsigned long int occur1 = 0;
	unsigned long int occur2 = 0;
	for(unsigned char h=0; h<UCHAR_MAX; h++)//determine if there is an even or odd number of nodes
	{
		if(charSort[h] > 0)
		{
			if(h%2 == 0) even = true;
			break;
		}
	} 
	for(unsigned char i=0; i<UCHAR_MAX; i++) 
	{
		char1 = (((CharNode*) &*(charSort[i]))->character);
		occur1 = (((CharNode*) &*(charSort[i]))->occurrence);
		if(occur1 > 0)
		{
			if(even && i < (UCHAR_MAX-1)) //if there are an even number of nodes and we aren't at the end of the array then insert with insert2
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
				else root = insert_node(root, char1, occur1);
			}
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
