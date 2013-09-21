#include <string>
#include <cstdlib>
using namespace std;
const int MAXLENGTH = 100000;

struct CharNode{
	unsigned long int count;
	char char_in_text;
};


class CharList{
	char* buffer;
	int charOccurrence;
	unsigned long int len;
	public:
	bool isFound(char a){
		return 1;
	}
	void initBuffer(string fname){
		char ch;
		int i=0;
		//string str;
		ofstream out("out.txt");	    
		ifstream in(fname.c_str(), std::ios::in | std::ios::binary);
		if (in.good())
		{
			in.seekg(0, in.end); //Go to end of stream
			len = in.tellg(); //Get the length of the file
			buffer = new char[len];
			buffer[len] = 0; //nullpad the string
			len = len -1; //Subtract one for zero based arrays
			in.seekg(0, in.beg); //Reset stream to beginning
			in >> noskipws;
			
			while(i!=len)
			{
				in >> buffer[i];
				out << buffer[i];
				i++;
			}
			
			in.close();
			cout << "File contents: " << buffer << endl;
			cout << "Read " << len << " characters... ";
			// read data as a block:
		
				if (in) std::cout << "all characters read successfully.";
			else
				std::cout << "error: only " << in.gcount() << " could be read";
				in.close();

			// ...buffer contains the entire file...

			delete[] buffer;
		}
		else
		{
			cerr << " File Not Found! " << endl;  
			return;
		}
		cout << std::endl << " length is = " << len << endl;
	}
	
	void populateTable(vector<CharNode*> &frequencyTable)
	{
		CharNode* temp = NULL;
	//		unsigned long int count;
	//		char char_in_text;
		for(int i=0; i<len; i++)
		{
			temp = new CharNode;
			temp -> char_in_text = buffer[i];
			temp -> count = 0;
			frequencyTable.push_back(temp);
			temp = NULL;
		}
		cout << "If I get this far then I don't explode" << endl;
	}
};