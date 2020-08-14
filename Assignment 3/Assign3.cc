/***********************************************************
* Name: Itzel Fernandez
* Class: CSCI 480
* Date: 2/20/20
***********************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdlib>
#include <cmath>


using namespace std;


int main()
{
	int arg = 32;
	char input[1024];
	bool val = false;
	bool val2 = false;

	cout<<"480shell: ";

	while (fgets(input,1024,stdin) != NULL)
	{
		input[strlen(input) -1] = '\0';
		char* argv[arg];
		argv[0] = strtok(input, " ");


		for(int i = 1; i < arg; i ++)
		{
			argv[i] = strtok(NULL, " ");
			if(argv[i] == NULL)
			{
				argv[i] = NULL;

				break;
			}
			if(strcmp(argv[i], "<+") == 0)		//search for <+
			{
				val = true;
			}
			else if(strcmp(argv[i], ">+") == 0)		//search for >+
                        {
                                val2 = true;
                        }

		}

		if(strcmp(argv[0], "q") == 0 || strcmp(argv[0], "quit") == 0)
		{
			exit(0);
		}
		else if(val == true)
		{
			pid_t  pid1;
			int    status1;

			if ((pid1 = fork()) < 0) {
                           cout<<"ERROR: forking child process failed\n";
                           exit(1);
			} else if(pid1 == 0) {

				if (strcmp(argv[0], "<+") == 0) {
					char * argRead[32];

				for(int i = 0; i < arg; i ++)
					argRead[i] = argv[1];

				int fileOpen = open(argv[arg], O_RDONLY);

				if(fileOpen == -1)
				{
					close(0);
					dup(fileOpen);
				}
				else
				{
					cout << "failed to open" << endl;
					exit(-1);
				}

				execvp(argRead[0], argRead);

			}
			}
			else
			{
				while (wait(&status1) != pid1);
			}

			val =false;
		}
		else if(val2 == true)
                {
                        cout<<"write to the file"<<endl;
			val2 = false;
                }

		else if(strcmp(argv[0], "about") == 0)
                {
                        cout<<"This shell program is the work of Itzel, 2020"<<endl;
                }

		else
		{
			pid_t  pid;
			int    status;

			if ((pid = fork()) < 0)
			{
          			cout<<"ERROR: forking child process failed\n";
          			exit(1);
     			}
     			else if (pid == 0)
			{
          			if (execvp(*argv, argv) < 0)
				{
			                cout<<"ERROR: exec failed\n";
					exit(1);
          			}
     			}
     			else
			{
          			while (wait(&status) != pid)
               				;
			}
		}
		cout<<"480shell: ";
	}
}
