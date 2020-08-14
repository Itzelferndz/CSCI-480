
/************************************************************
 * Name: Itzel Fernandez
 * Assignment2
 * ZID: z1850638
 * Purpose:
 * This program uses pipe to communicate between  processes
 * ***********************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <string.h>
#include <unistd.h>   //for fork, pipe
#include <sys/wait.h>    //for wait system call
#include <stdlib.h>  //system,atoi

using namespace std;

int pipeA[2];   //Parent pipe
int pipeB[2];  //Child pipe
int pipeC[2];  //Grandchild pipe

int PWork();   //function prototype for parent work
int CWork();   //function prototype for child work
int GWork();   //function prototype for grandchild work

int main(){
	//open all pipes
	if(pipe(pipeA) == -1) {cerr << "Failed to create Parent"; return -5;}
	if(pipe(pipeB) == -1) {cerr << "Failed to create Child"; return -5;}
	if(pipe(pipeC) == -1) {cerr << "Failed to create Grandchild"; return -5;}

	int pid = fork();
	if(pid == -1) {cerr << "Failed to Fork child";}

	//parent process
	else if(pid != 0){
		cerr << "Parent PID: " << getpid() << endl;
		return PWork();
	}
	else{
		pid = fork();
		if(pid == -1) {cerr << "Failed to fork grandchild";}

		//start child process
		else if(pid != 0) {
			cerr << "Child PID: " << getpid() << endl;
			return CWork();
		}

		//start grandchild process
		else{
			cerr << "Granchild PID: " << getpid() << endl;
			return GWork();
		}

	}//closing braces for else
	return 0;

}//closing braces for main



//Functions declarations

int PWork(){
	//closes pipes
	close(pipeA[1]); //closing the parent write  end
	close(pipeB[0]); //closing the read end of child
	close(pipeB[1]); //closing the write end of child
	close(pipeC[0]); //closing the read end of grandchild

	//set read and write ID
	int readID = pipeA[0];  //read from the parent pipe
	int writeID = pipeC[1]; //write to the grandchild pipe
	long M = 1;
	char WriteBuff[100] = "";
	cerr << "Parent process is ready to proceed.\n";

	sleep(1);

	//set initial value
	cerr <<"\nParent:               1\n";
	write(writeID, "1", 2);

	//if m is too samll
	while(M < 99999999999){
		string buff = "";
		char buffChar;

		//Read input one byte at a time and store in buff
		do{
			read(readID, &buffChar, 1);
		 	buff += buffChar;
		  }while(buffChar != '\0');

		//convert buff to num
		M = atol(buff.c_str());

		//calculate new number
		M = 200 - 3 * M;
		sprintf(WriteBuff, "%ld", M);
		cerr << setw(10) << left << "Parent: " << setw(12) << right << WriteBuff << endl;
		write(writeID, WriteBuff, strlen(WriteBuff) + 1);
	}//closing braces for while loop

	return 1;
}//closing braces for PWork


//child body
int CWork(){
	//closes pipes
	close(pipeB[1]); //closing the child write pipe end
	close(pipeC[0]); //closing the read grandchild pipe end
	close(pipeC[1]); //closing the write grandchild pipe end
	close(pipeA[0]); //closing the parent read end

	//set read and write Ids
	int readID = pipeB[0]; //read from the child pipe
	int writeID = pipeA[1]; //write to the parent pipe

	long M = 1;
	char WriteBuff[100] = "";
	cerr << "Child process is ready to proceed\n";

	//while m is too small
	while(M < 99999999999){
		string buff = "";
		char buffChar;

		//Read from the pipe one byte at a time and stor in buff
		do{
			read(readID, &buffChar, 1);
			buff += buffChar;
		}while(buffChar != '\0');


		//calculate new m
		M = atol(buff.c_str());
		M = 7 * M - 6;
		sprintf(WriteBuff, "%ld", M);
	//	sprintf(WriteBuff, "child:     %d\n",m);
		cerr << setw(10) << left <<  "Child: " << setw(14) << right << WriteBuff << endl;
	//	cout << endl;
		write(writeID, WriteBuff, strlen(WriteBuff)+1);
//		cout << endl;

	}//closing braces for while loop

	return 1;
}//closing braces for CWork


//Grandchild
int GWork(){
	//close pipes
	close(pipeC[1]); //closing the write end of the grandchild pipe
	close(pipeA[0]); //closing the read end of the parent pipe
	close(pipeA[1]); //closing the write end of the parent
	close(pipeB[0]); //closing the read end of the child

	//set read and write IDs
	int readID = pipeC[0]; //read from grandchild pipe
	int writeID = pipeB[1];//write to child pipe

	long M = 1;
	char WriteBuff[1000] = "";
	cerr << "Grandchild process is ready to proceed.\n";

	//if m i s too small
	while (M< 99999999999){
		string buff = "";
		char buffChar;

		//Read from pipe one byte at a time
		do{
			read(readID, &buffChar, 1);
			buff += buffChar;
		}while(buffChar != '\0');

		//convert and calculate
		M = atol(buff.c_str());
		M = 30 - 4 * M;

		sprintf(WriteBuff, "%ld", M);

	        cerr << setw(10) << left <<  "Grandchild: " << setw(10) << right << WriteBuff << endl;  
		write(writeID, WriteBuff, strlen(WriteBuff)+1);

		//write to next process
	//	write(writeID, buff, strlen(WriteBuff)+ 1);
	}
	return 1;
}//closing braces for GWork


