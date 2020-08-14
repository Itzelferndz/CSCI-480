/****************
 *Program: Assign5
 *Programmer: Itzel Fernandez
 *zId: z1850638
 * ********************/
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <queue>
using namespace std;

//Declaration of Constant variables
#define P_NUMBER 7     //number of producers
#define C_NUMBER 5     //number of consumers
#define P_STEPS 5     //number of iteration for each producer thread
#define C_STEPS 7     //number of iteration for each consumer thread

#define BUFFER 35

//Declaration of Global variables
pthread_mutex_t mutexLock;
sem_t notFull;     //declaration of not full semaphore
sem_t notEmpty;    //declaration of not empty semaphore
int widgets = -1;   //number of widgets
int counter= 0;
pair<int, int> widget;
queue<pair<int,int>>temp;
queue<pair<int,int>>b;
//Function Prototypes
void* Produce(void* threadId);
void* Consume(void* threadId);
void Insert(int threadId);
void Remove(int threadId);


int main(){
	pthread_t consumer[C_NUMBER]; //array of consumer thread
	pthread_t producer[P_NUMBER];//array of producer thread
	pthread_mutex_init(&mutexLock,NULL);  //set up mutex
	sem_init(&notFull,0, BUFFER);//initialization of notFull
	sem_init(&notEmpty, 0, 0);//initialization of notEmpty

	cout << "Simulation of Producer and Consumer" << endl;
	cout << endl;
	cout << "The semaphores and mutex have been initialized." << endl;
	cout << endl;

	//creating thread Ids

	for(long i = 0; i < C_NUMBER; i++){
		if(pthread_create(&consumer[i], NULL,Consume, (void*) i)!= 0){
			//print error message
			fprintf(stderr, "Error in Consumer create\n");
			exit(-1);
		}//closing braces for if
	}//closing braces for for loop


	for(long i = 0; i < P_NUMBER; i++){
		if(pthread_create(&producer[i],NULL,Produce,(void*) i)!= 0){
			fprintf(stderr, "Failed to create producer thread");
			exit(-1);
		}//closing braces for if
	}//closing braces for for loop

	for(int i = 0; i < P_NUMBER; i++){
		if(pthread_join(producer[i], NULL)!=0)
		{
		 printf("error teminating producer");
		 exit(-1);
		}

	}//closing braces for for loop

	for(int i = 0; i < C_NUMBER; i++){
		if(pthread_join(consumer[i], NULL)!=0)
		{
		 printf("error terminating consumer");
		 exit(-1);

		}
	}


	cout << "All the producer and consumer threads have been closed." << endl;
	cout << endl;
	cout << "The semaphores and mutex have been deleted." << endl;

	pthread_mutex_destroy(&mutexLock);
	pthread_exit(NULL);

	sem_destroy(&notFull); //delete notFull semaphore
	sem_destroy(&notEmpty);//delete notEmpty semaphore

	return 0;



}//closing braces for main


/*This function inserts wideget into buffer for  p_STEPS times*/
void* Produce(void* threadId){
	long id = (long) threadId;
	for(int i = 0; i < P_STEPS; i++){
		if(sem_wait(&notFull)){
			fprintf(stderr, "Failed to wait semaphore notFull\n");
			exit(-1);
		}
		Insert(id);

		if(sem_post(&notEmpty)){
			fprintf(stderr, "Failed to post semaphore notEmpty\n");
			exit(-1);
		}
		sleep(1);
	}
	pthread_exit(0);  //exit the thread


}//closing braces for function void *producer_work(void* threadID)



/*This function removes widget from buffer C_STEPS times*/
void* Consume(void* threadId){
	long id = (long) threadId;

	for(int i = 0; i < C_STEPS; i++){
		if(sem_wait(&notEmpty)){
			fprintf(stderr, "Failed to wait Semaphore notEmpty\n");
			exit(-1);
		}

		Remove(id);

		if(sem_post(&notFull)){
			fprintf(stderr,"Failed to post semapore notEmpty\n ");
			exit(-1);
		}
		sleep(1);
	}
	pthread_exit(0);

}//closing braces for function void *consumer_work(void* threadID)



/*This function inserts the widget into the buffer  */
void Insert(int threadId){
	if(pthread_mutex_lock(&mutexLock)){
		fprintf(stderr, "mutexLock failed to lock\n");
		exit(-1);
	}
	if(threadId == temp.front().first)
	  widgets++;
	b.push(make_pair(threadId, widgets));
	counter++;
	queue<pair<int,int>>temp= b;

	printf("Producer %d inserted one item.   Total is now %d.  ", threadId, (int)widgets);
	printf("Buffer now contains: ");
	while(!temp.empty())
	{
           printf("P %d W %d   ", temp.front().first,temp.front().second);
	   temp.pop();
	}
	printf("\n");
	if(pthread_mutex_unlock(&mutexLock)){
		fprintf(stderr, "mutexLock failed to unlock\n");
		exit(-1);
	}
}//closing braces for function void insertWidget(int threadID)




/*This function removes widget from the buffer */
void Remove(int threadId){
	if(pthread_mutex_lock(&mutexLock)){
		fprintf(stderr, "mutexLock failed to lock\n");
		exit(-1);
	}
	b.pop();
	counter--;
	queue<pair<int,int>>temp = b;
	printf("Consumer %d removed one item.   Total is now %d.  ", threadId,(int)counter);
	printf("Buffer now contains: ");
	while(!temp.empty())
	{
	 printf( "P %d W %d    ", temp.front().first, temp.front().second);
	temp.pop();
	}
	printf("\n");
	if(pthread_mutex_unlock(&mutexLock)){
		fprintf(stderr, "mutexLock failed to unlock\n");
		exit(-1);
	}
}//closing braces for function void removeWidget(int threadId)
