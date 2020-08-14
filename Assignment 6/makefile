#Programmer: Bhumika Niraula
#zID: z1832116
#Program: Assignment6

#Compiler Variables
CCFLAgs = -Wall -std=c++11

#Rules to link object code to create executable file
Assign6: Assign6.o
	g++ $(CCFLAGS) -o Assign6 Assign6.o

#Rules to compile source code file to object code
Assign6.o: Assign6.cc
	g++ $(CCFLAGS) -c Assign6.cc

#Pseudo-target to remove object code and executable files
clean:
	-rm *.o Assign6
