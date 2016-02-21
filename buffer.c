/*

	James Oliver 
	1279196

*/

#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define RAND_DIVISOR 100000000
#define TRUE 1

// the mutex
pthread_mutex_t mutex;

//two semaphores full and empty
sem_t full, empty;

//the actual buffer
buffer_item buffer[BUFFER_SIZE];

//counter for the buffer
int counter;

//pthread id
pthread_t tid;   
  
//pthread attribute  
pthread_attr_t attr; 

//threads
void *producer(void *param);
void *consumer(void *param);

//function used to insert the passed item into the buffer if it is less than the counter otherwise throws an error
int insert_item(buffer_item item) {
    if(counter < BUFFER_SIZE) {
        buffer[counter] = item;
        counter++;
        return 0;
    }
    else { 
        return -1;
    }
}

//function used to remove the last value in the buffer throws an error if the buffer is already empty
int remove_item(buffer_item *item) {
    if(counter > 0) {
        *item = buffer[(counter-1)];
        counter--;
        return 0;
    }
    else { 
        return -1;
    }
}

//function to alternate between sleeping for a random period of time and inserting a random number into the buffer
//will print out the result
void *producer(void *param) {
    buffer_item item;
    
    while(TRUE) {
		//sleep for a random amount of time
        int random = rand() / RAND_DIVISOR;
        sleep(random);  
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
		
		//insert random number
        item = rand();
        if(insert_item(item)) {
            fprintf(stderr, "report error condition in producer\n");
        }
        else {
            printf("producer produced %d\n", item);
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

//function that sleeps for a random period of time and when it wakes will attempt to remove an item from the buffer
//will print out if its successful
void *consumer(void *param) {
    buffer_item item;
    
    while(TRUE) {
        int random = rand() / RAND_DIVISOR;
        sleep(random);      
        sem_wait(&full);		
        pthread_mutex_lock(&mutex);

        if(remove_item(&item)) {
            fprintf(stderr, "report error condition in consumer \n");
        }
        else {
            printf("consumer consumed %d\n", item);
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

//prompts user to enter the 3 arguments needed sleep time, number of consumer threads and then number of producer threads. 
//It then will initialise the data then create the threads before closing
int main(int argc, const char * argv[]) {
    int i;
    
    // Checks that all arguments are passed
    if(argc != 4) {
        fprintf(stderr, "./buffer.c <sleep time INT> <Producer Threads INT> <Consumer Threads INT>\n");
        exit(0);
    }
	
    //data inserted here from user
    int sleepTime = atoi(argv[1]);
    int numProducers = atoi(argv[2]);
    int numConsumers = atoi(argv[3]);

	//Initialise
    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);   
    pthread_attr_init(&attr);
    counter = 0;
    
    // Create producer thread(s)
    for(i = 0; i < numProducers; i++) {
        pthread_create(&tid,&attr,producer,NULL);
    }
    
    // Create consumer thread(s)
    for(i = 0; i < numConsumers; i++) {
        pthread_create(&tid,&attr,consumer,NULL);
    }
    sleep(sleepTime);
    printf("Program Exiting\n");
    exit(0);
    
}
