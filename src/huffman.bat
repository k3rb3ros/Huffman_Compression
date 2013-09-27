set path=c:\Mingw\bin;.;

g++ -L./ huffman_header.cpp huffman_trie.cpp charlist.cpp mcp.cpp -o mcp.exe -lmingw32 
pause
