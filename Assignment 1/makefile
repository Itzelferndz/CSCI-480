#
#Program : Assign1
#Name: Itzel Fernandez
#z1850638
#

#Compiler variables
CC = g++
CCFLAGS = -Wall

#Rule to link object code files to create execcutable files
Assign1: Assign1.o
	$(CC) $(CCFLAG) -o Assign1 Assign1.o

#Rule to compile source code file to object code
Assign1.o : Assign1.cc
	$(CC) $(CCFLAGS) -c Assign1.cc

#Pseudo-target to remove object code and executable files
clean:
	-rm *.o  Assign1

