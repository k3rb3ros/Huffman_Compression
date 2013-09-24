################################################
# Makefile for Huffman Compression CSCI480 CMU #
################################################
CC=g++ -std=c++0x
CFLAGS= -O0 -g -Wall -Wextra
LDFLAGS=
SOURCES=charlist.cpp huffman_trie.cpp mcp.cpp
OBJECTS=$(SOURCES:.cpp=.o)
BINARY=mcp

all: $(BINARY) $(OBJECTS)
.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

clean: 
	rm -fv $(BINARY) $(OBJECTS)
