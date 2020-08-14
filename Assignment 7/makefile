#
#Program: Assign7
#Programmer: Bhumika Niraula
#zID: z1832116
#

#Compiler variables
CCFLAGS = -Wall -std=c++11

#Rule to link object code files to create executable file
Assign7: Assign7.o
	g++ $(CCFLAGS) -o Assign7 Assign7.o

#Rules to compile source code files to object code
Assign7.o: Assign7.cc
	g++ $(CCFLAGS) -c Assign7.cc

clean:
	-rm *.o Assign7
