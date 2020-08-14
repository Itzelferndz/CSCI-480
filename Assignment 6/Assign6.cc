/**********************************************************************
 *Name: Itzel Fernandez
 *Program: Assignment 6
 *Section: 2
 *Description: This program simulates an operating system's memory management
 *system.
 * ******************************************************************/

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <list>
#include <queue>
#include <stddef.h>
#include <sstream>
#include <fstream>

#define HOWOFTEN 6;
const int KB = 1024;
const int MB = KB * KB;
int firstAddress = 4 * MB;



using namespace std;

//creating the class named pBlock

class pBlock{
	public:
		pBlock(int size, int address){
			this->size = size;
			this->firstAddress = address;
		}

		int firstAddress;
		pBlock * prev = nullptr;
		pBlock * next = nullptr;
		int size;
		int pid;
		string flag;
};

list<pBlock> avail;   //list of available memory blocks
list<pBlock> inUse;   // list of menory blocks in use



//function prototypes
void loadFirst(string);
void loadBest(string);
void runBest();
void runFirst();
void terminate(string);
void initializeAvail(pBlock);
void deallocate(string);
void print();



//start of the main function
int main(int argc, char *argv[]){
	char argument = '?';
	if(argc>1){ 
		argument = argv[1][0];

		if(!(argument == 'B' || argument == 'F')){
			cerr << "\nuse the argument 'b' or 'f'\n";
			exit(-1);
		}
	}

	else{
		cerr << "\nNo arguments given\n";
		exit(-1);
	}

	int size[5] = {MB, 2*MB, 2*MB, 4*MB, 4*MB};
	for(int i = 0; i < 5; i++){
		avail.push_back(*(new pBlock(size[i],firstAddress)));
		firstAddress = firstAddress + size[i];
	}

	if(argument == 'B'){
		cerr << "Simulation of Memory Management using Best-FIt algorithm\n\nBeginning of the run\n\n";
		runBest();
	}
	else{
		cerr << " Starting Simulation with First-Fit method\n\nBeginning of the run\n\n";		runFirst();
	}


}//closing braces for main function



//first fit memory system
void runFirst(){
	ifstream file;
	file.open("data6.txt");  //opent the file
	char line = '?';
	string token;
	print();
	while(!file.eof()){   //continue if the end of file is not reached
		getline(file,token);
		line = token[0];
		if(line == 'L')  // load proces
			loadFirst(token);
		else if(line == 'T')  // to terminate the process
			terminate(token);
		else if(token[0] == 'A')   // Allocate memory
			loadFirst(token);
		else if(line == 'D')  //Deallocate memory
			deallocate(token);
		else if(line == '?')   //end
			break;
		print();  // print the state of the system

	}//closing braces for while loop

	file.close(); //close the file
	cerr << "Simulation Ended\n\n";
	print();

}//closing braces for void first_run


//Best-fit memory system
void runBest(){
	ifstream file;
	file.open("data6.txt");  //opent the file
	char line = '?';
	string token;
	print();
	while(!file.eof()){   //continue if the end of file is not reached
		getline(file,token);
		line = token[0];
		if(line == 'L')  // load proces
			loadBest(token);
		else if(line == 'T')  // to terminate the process
			terminate(token);
		else if(token[0] == 'A')   // Allocate memory
			loadBest(token);
		else if(line == 'D')  //Deallocate memory
			deallocate(token);
		else if(line == '?')   //end
			break;
		print();  // print the state of the system

	}//closing braces for while loop

	file.close(); //close the file
	cerr << "Simulation Ended\n\n";
	print();

}//closing braces for void best_run();



//Allocate memory for first-fit
void loadFirst(string token){
	string flag;
	int iD;
	int size;
	char order;
	bool isAvail = false;
	//use the line as input
	istringstream str(token);

	//to break apart input line
	str >> order >> iD >> size >> flag;
	list<pBlock>::iterator itr;
	if(order == 'L')
		cerr << "Transaction: request to load process " << iD << ", block ID " << flag << " using " << size << " bytes\n";
	else
		cerr << "Transaction: request to allocate " << size << " bytes for process " <<iD << ", block ID: " << flag << "\n";
	
	for(itr = avail.begin(); itr != avail.end(); ++itr){
		if((itr->size - size)>= 0){
			isAvail = true;
			cerr << "Found a block of size " << itr->size << '\n';
			break;

		}
	}

	//if there is a block we can use, start using it
	if(isAvail){
		itr->size = itr->size -size;
		pBlock mem(size, itr->firstAddress);
		itr->firstAddress = itr->firstAddress + size;
		mem.pid = iD;
		mem.flag = flag;
		inUse.push_front(mem);

		cerr << "Success in allocationg a block\n\n";
	}
	else{
		cerr << "Unable to comply as no block of adequate size is available\n\n";
	}
}//closing braces for void load_first(stirng);



//Allocate memory for best fit
void loadBest(string token){
	string flag;
	int iD;
	int size;
	char order;
	bool isAvail = false;
	//use the line as input
	istringstream str(token);

	//to break apart input line
	str >> order >> iD >> size >> flag;
	list<pBlock>::iterator itr1;
	list<pBlock>::iterator itr2;

	int availMemory = 15 * MB;

	if(order == 'L')
		cerr << "Transaction: request to load process " << iD << ", block ID " << flag << " using " << size << " bytes\n";
	else
		cerr << "Transaction: request to allocate " << size << " bytes for process " <<iD << ", block ID: " << flag << "\n";
	
	for(itr1 = avail.begin(); itr1 != avail.end(); ++itr1){
		if((itr1->size - size)> 0 && (itr1->size - size) < availMemory){
			availMemory = (itr1->size -size);
			itr2 = itr1;
			isAvail = true;
		}
	}
	//if there is a block we can use, start using it
	if(isAvail){
		cerr << "Found a block of size " << itr2->size << "\n";
		itr2->size = itr2->size -size;
		pBlock mem(size, itr2->firstAddress);
		itr2->firstAddress = itr2->firstAddress + size;
		mem.pid = iD;
		mem.flag = flag;
		inUse.push_back(mem);

		cerr << "Success in allocationg a block\n\n";
	}
	else{
		cerr << "Unable to comply as no block of adequate size is available\n\n";
	}
}//closing braces for void load_best(stirng);


//Deallocate a memory block
void deallocate(string token){
	string flag;
	int iD;
	char order;
	istringstream str(token); //use line as input
	str >>order >> iD >> flag;

	cerr << "Transaction: request to deallocate block ID " << flag << " for process " << iD << "\n";

	list<pBlock>::iterator itr;
	for(itr = inUse.begin(); itr != inUse.end(); itr++){  //Loop through inUse block
		if(itr->pid == iD && itr->flag == flag){ //Look for the block to deallocate
			//put blocks in available
			initializeAvail(*(new pBlock(itr->size, itr->firstAddress)));
			inUse.erase(itr);//remove block from inUse
			cerr << "Success in deallocating a block\n\n";
			return;
		}
	}
	cerr << "Unable to comply as the indicated block cannot be found.\n\n";
}//closing braces for void deallocate(string token);



//terminate a process
void terminate(string token){
	int iD;
	char order;
	istringstream str(token);
	bool isTerminated = false;

	str >> order >> iD;
	list<pBlock>::iterator itr;

	cerr << "Transaction: request to terminate process " << iD << "\n";

	for(itr = inUse.begin(); itr != inUse.end(); itr++){  //Loop through inUse block
		if(itr->pid == iD){
			initializeAvail(*(new pBlock(itr->size, itr->firstAddress)));
			inUse.erase(itr);
			itr--;
			isTerminated = true;
		}
	}

	if(!isTerminated)
		cerr << "Unable to comply as the indicated process could not be found.\n\n";
	else
		cerr << "Success in terminating a process\n\n";
}//closing braces for void terminate(string token);


void initializeAvail(pBlock mem){
	list<pBlock>::iterator itr1;
	list<pBlock>::iterator itr2;

	bool isInAvail = false;
	for(itr1 = avail.begin(); itr1 != avail.end(); itr1++){
		if(itr1->firstAddress >mem.firstAddress){
			avail.insert(itr1,mem);
			isInAvail = true;
			break;
		}
	}
	if(!isInAvail){
		avail.push_front(mem);
	}

	for(itr1 = avail.begin(); itr1 != --avail.end(); itr1++){
		itr2 = itr1;
		itr2++;
		if((itr1->size + itr1 -> firstAddress) == (itr2-> firstAddress)){
			if((itr1->size + itr2->size) <= 4*MB){
				cerr << "Merging two blocks at " << itr1->firstAddress << " and "<< itr2->firstAddress << "\n";
				itr1->size = itr1->size + itr2->size;
				avail.erase(itr2);
				itr1--;
			}
		}
	}
}//closing braces for void initialize_Avail(pBlock mem)


//print the current status
void print(){
	list<pBlock>::iterator itr;
	cerr << "List of available blocks\n";
	int total = 0;

	if(avail.size()>0){
		for(itr = avail.begin(); itr != avail.end(); ++itr){
			cerr << "Start Address =" << setw(9) << itr -> firstAddress;
			cerr << " Size =" <<setw(9) << itr->size << "\n";
			total = total + itr->size;
		}
	}
	else{
		cerr << "(none)\n";
	}

	cerr << "Total size of the list is: " << total << "\n\n";
	cerr << "List of block in use\n";
	total = 0;

	if(inUse.size() > 0){
		for(itr=inUse.begin(); itr != inUse.end(); ++itr){
			cerr << "Start Address =" << setw(9) << itr->firstAddress;
			cerr << " Size =" <<setw(8) << itr->size;
			cerr << " Process ID =" << setw(5) << itr->pid;
			cerr << " Block ID =" << setw(6) << itr->flag << "\n";
			total = total + itr->size;
		}
	}

	else{
		cerr << "(none)\n";
	}
	cerr << "Total size of the list is: " <<total << "\n\n";
}//closing braces for void print();


