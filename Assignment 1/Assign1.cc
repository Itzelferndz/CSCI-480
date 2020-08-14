/********************************************************
CSCI 480
Assignment1
Fall 2019
Name: Itzel Fernandez
z-ID: z1850638
Section: 2

Purpose: Use fork() to create child porcess

*****************************************************/

#include <sys/types.h>    /*needed to use pid_t, etc. */
#include <sys/wait.h>     /*needed to use wait()*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>       /*LINUX constants and functions(fork(), etc.)*/

int main()
{
  pid_t pid1; //for first fork
  pid_t pid2; //for seocnd fork

  printf("\nI am the original process. My PID is %d and my parents PID is %d.\n", getpid(), getppid());
  printf("Now we have the first fork.\n");

  pid1 = fork();//first fork creates one parent and one child process

  if(pid1 < 0)
  {
    fprintf(stderr,"The first fork failed.\n");
    exit(-1);
  }//closing braces for if for pid1

  //for parent process
  else if(pid1 > 0)
  {
    printf("I am the parent. MY PID is %d and parent's PID is %d.\n", getpid(), getppid());
    sleep(2);
    printf("I am the parent, having waited on the child, about to call ps.\n");
    system("ps");
    printf("I am the parent, about to exit.\n");
    printf("\n");
    wait(0);
    exit(0);
  }//closing braces for pid1 for else if

  //if pid1 == 0 then this is the child process
  else
  {
    printf("I am the child. My PID is %d and my parent's PID is %d.\n", getpid(), getppid());
    printf("Now we have second fork.\n");

    pid2 = fork();

      //creates another child process for the first child process
      if(pid2 < 0)
      {
        fprintf(stderr,"The second fork failed.\n");
        exit(-1);
      }//closing braces for pid2 for if

      //parent process for second fork
      else if(pid2 > 0)
      {
        printf("I am the child. My PID is %d and my parent's PID is %d.\n", getpid(), getppid());
        sleep(1);
        wait(0);
        printf("I am the child, about to exit.\n");
      }//closing braces for pid2 else if

     //if pid == 0, this is the grandchild process
      else{
        printf("I am the grandchild. MY PID is %d and my parent's PID is %d.\n", getpid(), getppid());
        printf("I am the grandchild process and about to exit.\n");
        exit(0);
       }//closing braces for pid2 else

}//closing braces for pid1 for else

return (0);


}//closing braces for main
