# Define the compiler
CC=g++
# Define the compiler options
CFLAGS=-O2
# Define the linker
LINK=g++
# Define the dependencies

all: Mancala

# Define the dependencies
Mancala: Mancala.o
	$(LINK) -o Mancala Mancala.o  
Mancala.o: Mancala.cpp 
	$(CC) -c $(CFLAGS) -std=c++0x Mancala.cpp

clean:
	/bin/rm -f Mancala Mancala.o