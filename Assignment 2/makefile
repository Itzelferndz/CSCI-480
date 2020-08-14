
#Name: Itzel Fernandez
#zId:  z1850638
#Assign 2
#Compiler variables
CC = g++
CCFLAGS = -Wall -ggdb

#Rule to link object code files to create executable files
Assign2: Assign2.o
	$(CC) $(CCFLAGS) -o Assign2 Assign2.o

#Rule to compile source code file to object code
Assign2.o : Assign2.cc
	$(CC) $(CCFLAGS) -c Assign2.cc

#pseudo target to remove object code and executable files
clean:
	-rm *.o Assign2

