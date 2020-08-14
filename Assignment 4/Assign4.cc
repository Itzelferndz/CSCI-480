/*********************************************
* Name: Itzel Fernandez
* Z-Id: z1850638
* Program: Assignment 4
* Date: 3/24/2020
************************************************/
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stack>
#include <queue>


using namespace std;


struct Entry {
	int pageNum;
	int frameNum = -1;
	bool modified;
};

void printPageTable(Entry[]);

int findOldPage(Entry[], int);


const int FRAME_NUM = 15;
const int PAGE_NUM = 25;
const int HOW_OFTEN = 5;
const int PRINT_LINES = 5;
const int PAGE_SIZE = 200;

int main(int argc, char* argv[]) {
    int inputLines = 0;
    int totalPageFaults = 0;
    string firstArg = argv[1];
    string secondArg = argv[2];
    int logicalAddress;
    int count = 0;
    string letter;

    stack<int> lRU;
    stack<int> lRU2;
    queue<int> fIFO;

    fstream infile;
    infile.open("A4data.txt", ios::in);

    Entry pageTable[PAGE_NUM];
    const int frames[FRAME_NUM] = {101, 102, 103, 104, 105, 201, 202, 203, 204, 205, 301, 302, 303, 304, 305};

    if(argc > 3) {
        cerr << "Over max arguments" << endl;
        exit(-1);
    } else if (argc < 3) {
        cerr << "Need more arguments" << endl;
        exit(-1);
    }

    if (firstArg != "F" && firstArg != "L") {
        cerr << "Enter F or L for the first argument" << endl;
        exit(-2);
    } else if (secondArg != "P" && secondArg != "D") {
        cerr << "Enter P or D for the second argument" << endl;
        exit(-2);
    }

    if (secondArg == "P") {
        for (int i = 0; i < FRAME_NUM; i++) {
            pageTable[i].frameNum = frames[i];
            if (firstArg == "F") {
                fIFO.push(frames[i]);
            } else {
                lRU.push(frames[i]);
            }
        }
    }


    if (firstArg == "L") {
        cerr << "---------------Starting program with LRU";
    } else {
        cerr << "---------------Starting program with FIFO";
    }

    if (secondArg == "P") {
        cerr << " and Pre Paging---------------" << endl << endl;
    } else {
        cerr << " and Demand Paging---------------" << endl << endl;
    }

    while(infile >> logicalAddress >> letter) {
        inputLines++;

        int pageNum = 0;

        pageNum = logicalAddress / PAGE_SIZE;


        if (pageTable[pageNum].frameNum != -1 && (firstArg == "L")) {
            int frameToFind = pageTable[pageNum].frameNum;
            int stack1size = lRU.size();
            for (int i = 0; i < stack1size; i++){
                int top = lRU.top();
                if (top == frameToFind) {
                    lRU.pop();
                } else {
                    lRU2.push(top);
                    lRU.pop();
                }
            }
            int stack2size = lRU2.size();
            for (int i = 0; i < stack2size; i++){
                int top = lRU2.top();
                lRU.push(top);
                lRU2.pop();
            }
            lRU.push(frameToFind);
        }


        if(pageTable[pageNum].frameNum == -1) {
            totalPageFaults++;
            if ((secondArg == "P") || count == (FRAME_NUM)) {
		int oldPage = 0;
		int nextFrameToUse = 0;
                if (firstArg == "F") {
                    nextFrameToUse = fIFO.front();
                    oldPage = findOldPage(pageTable, nextFrameToUse);
                    fIFO.pop();
                    pageTable[pageNum].frameNum = nextFrameToUse;
                    pageTable[oldPage].frameNum = -1;
                    fIFO.push(nextFrameToUse);
                } else {
                    int size = lRU.size();

                    for (int i = 0; i < size; i++) {
                        int top = lRU.top();
                        lRU.pop();
                        lRU2.push(top);
                    }

                    nextFrameToUse = lRU2.top();
                    oldPage = findOldPage(pageTable, nextFrameToUse);
                    lRU2.pop();
                    pageTable[pageNum].frameNum = nextFrameToUse;
                    pageTable[oldPage].frameNum = -1;
                    int size2 = lRU2.size();

                    for (int i = 0; i < size2; i++) {
                        int top = lRU2.top();
                        lRU2.pop();
                        lRU.push(top);
                    }
                    lRU.push(nextFrameToUse);
                }

                if (pageTable[oldPage].modified == true) {
                    cerr << "Write page " << oldPage << " from " << nextFrameToUse << " to the disk\n";
                    pageTable[oldPage].modified = false;
                }
              cerr << "Read page " << pageNum << " from the disk into frame " << pageTable[pageNum].frameNum << endl;
            } else {
		pageTable[pageNum].frameNum = frames[count];
		cerr << "Read page " << pageNum << " from the disk into frame " << pageTable[pageNum].frameNum << endl;
                if (firstArg == "F") {
                    fIFO.push(frames[count]);

                } else {
                    lRU.push(frames[count]);

                }
                count++;
            }
        }

        if (inputLines % PRINT_LINES == 0) {
            printPageTable(pageTable);
        }
    }

    infile.close();
    cout << "Total page faults: " << totalPageFaults << endl;

    return 0;
}

/*********************************************************
*This function prints the page table and the frame table
*********************************************************/
void printPageTable(Entry array[]){
    cout <<setw(16)<< "Page Number"
    << setw(16) << "Frame Number"
    << setw(16) << "Modified?" << endl;

    for (int i = 0; i < PAGE_NUM; i++) {
        if (array[i].modified == true) {
            cout <<setw(16)<< i << setw(16)
            << array[i].frameNum << setw(16)
            <<  "Yes" << endl;
        } else {
            cout <<setw(16)<< i <<setw(16)
            << array[i].frameNum << setw(16)<< "No" << endl;
        }
    }
}
/*********************************************************************
*This function will look for an old page and take the frame number
*********************************************************************/
int findOldPage(Entry pageTable[], int newFrame) {
    for (int i = 0; i < PAGE_NUM; i++) {
        if (newFrame == pageTable[i].frameNum){
            return i;
        }
    }
   return 0;
}
