/************************************************
 * Name :Itzel Fernandez
 * zID: z1850638
 * Program: Assign7
 * ************************************/
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>

using namespace std;

#define HOWOFTEN 6
#define SIZE 512
#define ENTREES 12

const char* fName = "data7.txt";

struct Entry{
	string fName;
	int size;
	int blocks;

	Entry(int a, string b, int c)
	{
		blocks = a;
		fName = b;
		size = c;
	}
};

vector<Entry*> directory;
short FAT[4096];

//Declaration of function prototypesi
bool newFile(string, int);
bool Copy(string, string);
bool Rename(string, string);
bool Delete(string);
bool Modify(string, int);
void printFAT();
void printDirectory();
Entry * findFile(string);
int findEmpty(int);
void Check();


int main(int argc, char ** argv)
{
	FAT[0] = -1;
	FAT[4095] = -1;
	directory.push_back(new Entry(0, ".", 512));
	directory.push_back(new Entry(-1, "..", 0));
	char inLine[256];
	int count = 0;
	bool isCounter = false;
	string name1;
	string name2;
	int counter;

	ifstream inFile;//input stream for inFile
	inFile.open(fName);//open the file

	//if failed to open the file the exit
	if (!inFile) {
		cerr << "unable to open input file";
		exit(-1);
	}

	cout << "Beginning of the FAT simulation\n";

	while (inFile && !isCounter)
	{
		//print every HOWOFTEN
		if (count % HOWOFTEN == 0){//print every howoften
			printDirectory();
			printFAT();
		}//closing braces for if

		memset(inLine, '\0', 256);//to clear previous input

		inFile.getline(inLine, 256);//read

		switch(inLine[0])		{
			case 'C':
				cout << "Transaction:  Copy a file\n";
				strtok(inLine, " \n\r");
				name1 = (string) (strtok(nullptr, " \n\r"));
				name2 = (string) (strtok(nullptr, " \n\r"));
				if(Copy(name1, name2))
					cout << "Successfully copied an existing file, " << name1 << ", to a new file, " << name2 << endl;
					break;

			case 'D':
				cout << "Transaction:  Delete a file\n";

				strtok(inLine, " \n\r");
				name1 = (string) (strtok(nullptr, " \n\r"));

				if(Delete(name1))
					cout << "Successfully deleted a file, "	 << name1 << endl;
				break;

			case 'N':
				cout << "Transaction:  Add a new file\n";

				strtok(inLine, " \n\r");
				name1 = (string) (strtok(nullptr, " \n\r"));
				counter = atoi(strtok(nullptr, " \n\r"));

				if(newFile(name1, counter))
					cout << "Successfuly added a new file, " << name1 << ", of size " << counter << endl;
					break;
			case 'M':
				cout << "Transaction:  Modify a file\n";

				strtok(inLine, " \n\r");
				name1 = (string) (strtok(nullptr, " \n\r"));
				counter = atoi(strtok(nullptr, " \n\r"));

				if(Modify(name1, counter))
					cout << "Successfully modified a file, " << name1 << endl;
				break;

			case 'R':
				cout << "Transaction:  Rename a file\n";

				strtok(inLine, " \n\r");
				name1 = (string) (strtok(nullptr, " \n\r"));
				name2 = (string) (strtok(nullptr, " \n\r"));

				if(Rename(name1, name2))
					cout << "Successfully changed the file name " << name1 << " to " << name2 << endl;
					break;

			case '?':
			isCounter= true;
				break;

			default:
				break;
		}//closing braces for switch

		count++;
	}//closing braces for while loop

	cout << "\nEnd of the FAT simulation\n";
	printDirectory();
	printFAT();

	return 0;
}//closing braces for main

/***************************************************************
 Prints eveything in the current directory
***************************************************************************/

void printDirectory()
{
	cout << "\nDirectory Listing\n";
	int size1 = 0;
	int cluster;

	for (vector<Entry*>::iterator itr = directory.begin(); itr != directory.end(); itr++){
		size1 += (*itr)->size;

		cout << "File name: " << setw(23) << left << (*itr)->fName;
		cout << "File Size: " << setw(5) << right << (*itr)->size << endl;
		cout << "Cluster(s) in use:  ";

		if ((*itr)->blocks != -1){
			cluster = 1;

			if (FAT[(*itr)->blocks] == -1)
				cout << setw(6) << (*itr)->blocks << endl;
			else{
				cout << setw(6) << (*itr)->blocks;
				for(int pos = (*itr)->blocks; FAT[pos] != -1; pos = FAT[pos]){
					if (cluster % 12 == 0)
						cout << endl << "                    ";
					cout << setw(6) << FAT[pos];
					cluster++;

				}//closing braces for for loop
				cout << endl;
			}//closing braces for else
		}//closing braces for if
		else
			cout << "(none)\n";
	}
	cout << "Files:  " << directory.size();
	cout << "  Total Size:  " << size1;
	cout << " bytes\n";

}

/************************************************************************
Prints the FAT up to its 240th block
*****************************************************************************/

void printFAT()
{
	cout << "\nContents of the File Allocation Table";
	int num = 0;
        int count = 0;

	while (count < 240){

		if (count % 12 == 0){ //prints line num
			if (count == 0)
				cout << "\n#" << setw(3) << setfill('0') << num;
			else
				cout << " \n#" << setw(3) << setfill('0') << num;
			num += 12;
			cout << "-" << setw(3) << setfill('0') << num-1 << " ";
		}//closing braces for if
		cout << setw(6) << setfill(' ') << FAT[count];
		count++;
	}
	cout << setfill(' ') << " " << endl << endl;
}//closing braces for while loop

/*******************************************************************************************
Creates a new file in the dir and fills the FAT as neccessay
***************************************************************************************************/
bool newFile(string name, int sizeH)
{
	Entry * tempFile = findFile(name);

	int next = -1;
	bool found = true;
	int recent = 0;

	if (tempFile == nullptr)	{
		if (sizeH != 0)	{
			recent = findEmpty(0);//to find the available first spot
			directory.push_back(new Entry(recent, name, sizeH));

			sizeH -= SIZE;

			while (sizeH > 0)	{
				next = findEmpty(recent+1);
				FAT[recent] = next;
				recent = next;

				sizeH -= SIZE;
			}//closing braces for while loop
			FAT[recent] = -1;
		}//closing braces for inner if
		else
			directory.push_back(new Entry(-1, name, 0));

	}//closing braces for if
	else
	{
		cout << "File already exsists!\n";
		found = true;
	}
	Check();
	return found;
}

/**************************************************************************
Renames a file from one name to another
*********************************************************************************/
bool Rename(string line1, string line2)
{
	Entry * fName1 = findFile(line1);
	if (fName1 == nullptr)
	{
		cout << "Error!  The old file name, " << line1 << ", was not found.\n";
		return false;
	}

	Entry * fName2 = findFile(line2);
	if (fName2 != nullptr)
	{
		cout << "File already exsists!\n";
		return false;
	}

	fName1->fName = line2;
	return true;
}

/***************************************************************************************
 Modifies an exsisting file, used to change the 
	 filesize
**********************************************************************************************/
bool Modify(string line, int size)
{
	Entry * fName1 = findFile(line);
	if (fName == nullptr)
	{
		cout << "Error!  The file name, " << line << ", was not found.\n";
		return false;
	}

	string temp = (fName1->fName) + "     ";
	string old = fName1->fName;

	//create new file, delete old file, rename file back
	newFile(temp, size);
	Delete(old);
	Rename(temp, old);
	return true;
}

/**************************************************************
 deletes a file from dir and corrispondingly removes
     it's place in the FAT
***********************************************************************************/

bool Delete(string line)
{
	Entry * fName1 = findFile(line);
	if (fName1 == nullptr)
	{
		cout << "Error!  The file name, " << line << ", was not found.\n";
		return false;
	}

	int location = fName1->blocks;
	int nextLocation;

	if (location != -1){
		while (FAT[location] != -1)
		{
			nextLocation = FAT[location];
			FAT[location] = 0;
			location = nextLocation;
		}//closing braces for while loop
		FAT[location] = 0;
	}//closing braces for if

	bool stopState = false;
	for (vector<Entry*>::iterator itr = directory.begin(); itr != directory.end() && !stopState; itr++){
		if ((*itr)->fName == line) 	{
			directory.erase(itr);
			stopState = true;
		}//closing braces for if
	}//closing braces for for loop

	Check();

	return true;
}

/*******************************************************************************************
copies a file with a given filename to another name
***********************************************************************************************/
bool Copy(string line1, string line2)
{
	Entry * fName1 = findFile(line1);
	if (fName1 == nullptr)
	{
		cout << "Error!  The old file name, " << line1 << ", was not found.\n";
		return false;
	}

	Entry * fName2 = findFile(line2);
	if (fName2 != nullptr)
	{
		cout << "Error!  The new file name, " << line2 << ", already exists.\n";
		return false;
	}

	newFile(line2, fName1->size);
	return true;
}

/*******************************************************************************
 checks to see if the current dir size
     isn't sufficient or superfluous
***************************************************************************************/

void Check()
{
	//shrink
	if (((float)directory.front()->size / (float)SIZE) > ceil((float)directory.size() / (float)ENTREES)){
		int old = 0;
		int cursor = 0;
		while (FAT[cursor] != -1)	{
			old = cursor;
			cursor = FAT[cursor];
		}//closing braces for while loop

		FAT[cursor] = 0;
		FAT[old] = -1;
		directory.front()->size = (int) ((float)SIZE * ceil((float)directory.size() / (float)ENTREES));
	}
	//grow
	else if (((float) directory[0]->size / (float) SIZE) < ceil( (float) directory.size() / (float) ENTREES))
	{
		int cursor = 0;
		int newPos;
		while (FAT[cursor] != -1)
		{
			cursor = FAT[cursor];
		}
		newPos = findEmpty(cursor);
		FAT[cursor] = newPos;
		FAT[newPos] = -1;
		directory.front()->size = (int) ((float)SIZE * ceil((float)directory.size() / (float)ENTREES));
	}
}

/***********************************************************************************************
 Finds a '0' in the FAT following initalPos
**********************************************************************************/

int findEmpty(int command)
{
	int value = -1;
	for (bool found = false; !found && command < 4096; command++)
	{
		if (FAT[command] == 0)
		{
			found = true;
			value = command;
		}
	}
	return value;
}

/******************************************************************************************
Finds a file in dir with the same name
     as the 'name' parameter
*********************************************************************************************/

Entry * findFile(string name)
{
	Entry * value = nullptr;
	bool stopState = false;
	for (vector<Entry*>::iterator itr = directory.begin(); itr != directory.end() && !stopState; itr++)
	{
		if ((*itr)->fName == name)
		{
			value = *itr;
			stopState = true;
		}
	}
	return value;
}

