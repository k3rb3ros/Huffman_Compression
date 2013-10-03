set path=c:\Mingw\bin;.;

g++ -L./ -std=c++0x huffman.cpp huffman_trie.cpp charlist.cpp mcp.cpp -o mcp.exe -lmingw32 
pause
