/*
* By Jesse Blagg and Ryan Morris
* Huffman Compression program
*/
#include <limits.h>
#include <iostream>
using namespace std;

struct Enc_node
{
	unsigned long int encoding;
	int length;
	Enc_node* buckets;
};

struct Trie_node
{
	bool is_character;	
	char character;
	unsigned long int val;
	Trie_node* left;
	Trie_node* right;
};

class Trie
{
	private:

	Enc_node enc_table[CHAR_MAX];
	Trie_node* root;
	unsigned long int char_count;
	unsigned long int node_count;
	void char_traverse(Trie_node* root, const char lookup);
	void count_traverse(Trie_node* root);
	void delete_buckets(Enc_node* bkt);
	void delete_trie(Trie_node* root);
	void node_traverse(Trie_node* root);
	
	public:
	
	Trie();
	unsigned long int character_count();
	unsigned long int get_encoding(const char Character);
	unsigned long int size_of_trie();
	void insert_trie(const char Character, int count);
	~Trie();
};

void Trie::char_traverse(Trie_node* root, const char lookup)//Recursively traverse through the Trie to the character passed into the function encode a bit string based on the traversal path
{
	//FIXME
}

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
	for(int i=0; i<CHAR_MAX; i++)
	{
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

unsigned long int Trie::get_encoding(const char Character) //Recursively traverse to the character creating the bitstring encoding based on left and right traversals
{	
	unsigned long int encode_buff = 0;
	unsigned long int* enc = &encode_buff;
		
	return 0;
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

int main(int argc, char** argv)
{
	Trie test;
	cout << "Constructed and inited\n";
	cout << "Nodes: " << test.size_of_trie() << endl;
	cout << "Characters: " << test.character_count() << endl;
	return 0;
}
