//********************************************************************
//
// Erman Gurses
// Foundation of Computer System Software COSC 5331
// Project #3: Prpducer consumer problem
//
// November 21, 2012 
// Instructor: Dr. Ajay K. Katangur
//********************************************************************

//********************************************************************
//
// Includes and Defines
//
//********************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>


//********************************************************************
//
// Function Prototypes
//
//********************************************************************
bool getAndCheckCommandLineArguments(int argc, char* argv[]);
void dataInitialization(); 
void *Consumer(void* threadid);
void *Producer(void* threadid);
void initBuffer();
bool IsPrime(int number); 
int  buffer_insert_item(int item,int id_ptr);
int  buffer_remove_item(int *item,int id_ptr);
void printResults();


//********************************************************************
//
// Global Variables
//
//********************************************************************
bool isSimilationFinished = false,    
     snapShot,
     isSnapShotCorrect;
int  simulationTimeLength,
     maxSleepTimeOfThreads,
     numberOfProducerThreads,
     numberOfConsumerThreads,
     numberOfItemsRemaningInBuffer,
     numberOfItemsBufferFull,
     numberOfItemsBufferEmpty,
     totalProduced=0,
     totalConsumed=0,
     counterForFullTimes  = 0,
     counterForEmptyTimes = 0;
int  occupiedBuffers = 0;     
int  buffer[5];
int  in  = 0,
     out = 0;     
int  counterInsertRemove = 0;
pthread_mutex_t mutex2;
sem_t full, mutex1, empty;
pthread_t idProducer[302];
pthread_t idConsumer[302];
int producerItemCounter[302];
int consumerItemCounter[302];
int *producerTaskIds[302];
int *consumerTaskIds[302];


//********************************************************************
//
// main Function
//
// This function does;
// 
//    Get command line arguments
//    Initialize buffer
//    Create producer thread(s)
//    Create consumer thread(s)
//    Sleep
//    Join Threads
//    Display Statistics
//    Exit
//
// Return Value
// ------------
//  int         0 
//
// Value Parameters
// ----------------
//    int     argc         It conunts command line arguments
//
// Reference Parameters
// --------------------
//    char  *argv[]        It contains command line arguments
//
// Local Variables
// ---------------
// i            int        Loop Iteration Variable.
// j            int        Loop Iteration Variable.
//
//*******************************************************************
int main(int argc, char *argv[]){

    if(getAndCheckCommandLineArguments(argc, argv) == false){
        printf("Invalid Arguments!!!\n"); 
        return 0; 
    }
    initBuffer();
    dataInitialization();
    printf("Starting Threads...\n");

    if(snapShot){
        printf("(buffers occupied: 0)\n");
        printf("buffers:\t -1\t-1\t-1\t-1\t-1\n");
        printf("\t\t----\t----\t----\t----\t----\n");
        printf("\t\t WR\n");
    }
    // create producer threads
    for(int i = 1; i <= numberOfProducerThreads; i++){
        producerItemCounter[i]=0;
        producerTaskIds[i] = (int *)malloc(sizeof(int));
        *producerTaskIds[i] = i;
        pthread_create(&idProducer[i],NULL,Producer,(void*)producerTaskIds[i]);
    }   
  
    // create consumer threads
    for(int j = 1; j <= numberOfConsumerThreads; j++){
        consumerItemCounter[j]=0;
        consumerTaskIds[j] = (int *)malloc(sizeof(int));        
        *consumerTaskIds[j] = j;
        pthread_create(&idConsumer[j],NULL,Consumer,(void*)consumerTaskIds[j]);
    }
    sleep(simulationTimeLength);
    isSimilationFinished = true;
    printResults();
    return(0);
}
//********************************************************************
//
// initBuffer Function
//
// This function initializes the buffer.
//
// Return Value
// ------------
// 
//
// Value Parameters
// ----------------

//
// Reference Parameters
// --------------------

// Local Variables
// ---------------
// i            int        Loop Iteration Variable.
//
//*******************************************************************
void initBuffer(){
    for(int i = 0; i < 5; i++){
        buffer[i] = -1; 
    }
}

//********************************************************************
//
//getAndCheckCommandLineArgumentsmain Function
//
// This function gets and validates command line arguments.
//
// Return Value
// ------------
// 
//
// Value Parameters
// ----------------
//
// Reference Parameters
// --------------------
//
// Local Variables
// ---------------
//
//*******************************************************************
bool getAndCheckCommandLineArguments(int argc, char* argv[]){
     simulationTimeLength     = atoi(argv[1]);    
     maxSleepTimeOfThreads    = atoi(argv[2]);      
     numberOfProducerThreads  = atoi(argv[3]);    
     numberOfConsumerThreads  = atoi(argv[4]);    
    
    if(argv[5] == NULL){
        return false;
    }
    
    if(!strcmp(argv[5],"yes") || !strcmp(argv[5],"Yes")){
        snapShot = true;
        isSnapShotCorrect = true;
    } else if(!strcmp(argv[5],"no") ||!strcmp(argv[5],"No")){
        snapShot = false;
        isSnapShotCorrect = true;
    }else{
        isSnapShotCorrect = false;    
    }
    // check command line arguments
    if( simulationTimeLength    >   0  && simulationTimeLength    <  60  &&
        maxSleepTimeOfThreads   >=  0  && numberOfProducerThreads >   0  && 
        numberOfProducerThreads < 300  && numberOfConsumerThreads >   0  && 
        numberOfConsumerThreads < 300  && isSnapShotCorrect == true      &&
        simulationTimeLength > maxSleepTimeOfThreads)
    {
    	return true;
    }else{
        return false;
    }
}

//********************************************************************
//
// dataInitialization Function
//
// This function initializes mutez and semaphore variable.
//
// Return Value
// ------------
// 
//
// Value Parameters
// ----------------
//
// Reference Parameters
// --------------------
//
// Local Variables
// ---------------
//
//*******************************************************************
void dataInitialization(){
    // initialize semaphores and mutexes
    pthread_mutex_init(&mutex2, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, 5);
    sem_init(&mutex1, 0, 1); 
}

//********************************************************************
//
// IsPrime Function
//
// This function checks a given number whether is prime or not.
//
// Return Value
// ------------
//  bool     true  if it is prime 
//           false if it is not prime
//
// Value Parameters
// ----------------
//  int      number   Given number to check is prime or not
//
// Reference Parameters
// --------------------
//    
//
// Local Variables
// ---------------
// i         int      Loop Iteration Variable.

//*******************************************************************
bool IsPrime(int number){
     // check whether or is prime by using sieve  and eratosthenes
     // algorithm 
    for(int i = 2; i*i <= number; i++){
        if (number % i == 0) 
        return false;
    }
    return true;
}

//********************************************************************
//
// Consumer Function
//
// This function applies all consumers's responsibilities. It has
// semaphores
//
// Return Value
// ------------
//  void 
//
// Value Parameters
// ----------------
//
//
// Reference Parameters
// --------------------
//*threadid                  Keeps id of thread
//
// Local Variables
// ---------------
// i           int           Loop Iteration Variable.
// id_ptr      int           Visual ID of thread.
// task_id     int           Visual ID of thread.
// int         item          Inserted Item to buffer    
// int         sleepTime     Amount of sleep time for thread 
// unsigend    seed          Seed for rand_r function
//*******************************************************************
void *Producer(void *threadid){
    unsigned int seed ;
    int *id_ptr;
    int task_id;
    int item;
    int sleepTime; 
    int value;

    id_ptr = (int *) threadid;
    task_id = *id_ptr;
    seed = (unsigned)task_id;
    
    // simulation time check whether is finished or not
    while (!isSimilationFinished){
        // get sleeping time 
        sleepTime = rand_r(&seed)%(maxSleepTimeOfThreads*1000000);    
        // sleep for this spesific thread
        usleep(sleepTime);
        item = rand_r(&seed)%100;
        // critical section for checking counterInsertRemove variable
        pthread_mutex_lock(&mutex2);    
        if(counterInsertRemove == 5){   
            if(snapShot){
            	printf("All buffers full. Producer %d waits\n\n",task_id);
            }
        }
        pthread_mutex_unlock(&mutex2);    
        // critical section for buffer
        sem_wait(&empty);
        sem_wait(&mutex1);
        totalProduced++;    
        producerItemCounter[task_id]++;
        buffer_insert_item( item, task_id ); 
        sem_post(&mutex1);    
        sem_post(&full);            
    }
}

//********************************************************************
//
// Consumer Function
//
// This function applies all consumers's responsibilities. It has
// semaphores
//
// Return Value
// ------------
//  void 
//
// Value Parameters
// ----------------
//
//
// Reference Parameters
// --------------------
//*threadid                   Keeps id of thread
//
// Local Variables
// ---------------
// i            int           Loop Iteration Variable.
// id_ptr       int           Visual ID of thread.
// task_id      int           Visual ID of thread.
// int          item          Inserted Item to buffer    
// int          sleepTime     Amount of sleep time for thread 
// unsigend      seed         Seed for rand_r function
//*******************************************************************
void *Consumer(void* threadid){
    unsigned seed;
    int *id_ptr;
    int task_id;
    int item;
    int sleepTime; 
    id_ptr = (int*) threadid;
    task_id = *id_ptr;
    seed = (unsigned)task_id;
   
    // simulation time check whether is finished or not
    while (!isSimilationFinished){
        // get sleeping time 
        sleepTime = rand_r(&seed)%(maxSleepTimeOfThreads*1000000);        
        // sleep for this spesific thread
        usleep(sleepTime);
        // critical section for checking counterInsertRemove variable
        pthread_mutex_lock(&mutex2);
        if(counterInsertRemove == 0){
            if(snapShot){
                printf("All buffers empty. Consumer %d waits\n\n",task_id);
            }    
        }
        pthread_mutex_unlock(&mutex2); 
        // critical section for buffer
        sem_wait(&full);       
        sem_wait(&mutex1);    
        totalConsumed++;    
        consumerItemCounter[task_id]++;    
        buffer_remove_item(&item,task_id);     
        sem_post(&mutex1);      
        sem_post(&empty);
    }
}

//********************************************************************
//
// buffer_insert_item Function
//
// This function inserts an item from buffer.
//
// Return Value
// ------------
//  int      0
//
// Value Parameters
// ----------------
//  int    id_ptr              Consumet Id        
//
// Reference Parameters
// --------------------
//
//
// Local Variables
// ---------------
// i        int                Loop iteration Variable.
// int      item               Inserted Item to buffer        
//
//*******************************************************************
int buffer_insert_item(int item,int id_ptr){

   buffer[in] = item; 
   // organize pointers for inner item
   in++;
   counterInsertRemove++;
   in  = in % 5;
   // shack snapshot variable 
   if(snapShot){
       printf("Producer %d writes %d\n",id_ptr, item);
       printf("(buffers occupied: %d)\n",counterInsertRemove);
       printf("buffers:  ");        
       printf("\t");
       for(int i = 0; i < 5; i++){
           printf(" %d    ",buffer[i]); 
       }        
       printf("\n\t\t----\t----\t----\t----\t----\n");  
       printf("\t\t ");
        
       // organize W and R letter
       if(in == out){
           counterForFullTimes++;
           for(int i = 0; i < out; i++){
               printf(" \t ");        
       }
       printf("RW");
            
       }else{
           if(in > out){
               for(int i = 0; i < out; i++){
                   printf(" \t ");
                
               } 
               printf("R");
               for(int i = out; i < in; i++){
                   printf(" \t ");
               }
               printf("W");
            }else{
                for(int i = 0; i < in; i++){
                    printf(" \t ");
                }
                printf("W");
                for(int i = in; i < out; i++){
                    printf(" \t ");
                }
                printf("R");
            }
        }
        printf("\n\n");
    }
    return 0;  
}


//********************************************************************
//
// buffer_remove_item Function
//
// This function removes an item from buffer.
//
// Return Value
// ------------
// int      0
//
// Value Parameters
// ----------------
// int      id_ptr           Consumet Id        
//
// Reference Parameters
// --------------------
// char     *item            Inserted Item to buffer        
//
// Local Variables
// ---------------
// i        int              Loop Iteration Variable.
//
//*******************************************************************
int buffer_remove_item(int *item,int id_ptr){
    *item = buffer[out]; 
    out++;
    counterInsertRemove--;
    out  = out % 5;

    if(in==out){
        counterForEmptyTimes++;
    }
    
    if(snapShot){
        printf("Consumer %d reads %d",id_ptr, *item);
        // check it is prime or not
        if(IsPrime(*item)){
            printf(" * * * PRIME * * * ");
        }
        
        printf("\n");
        printf("(buffers occupied: %d)\n",counterInsertRemove);
        printf("buffers:  ");
        
        printf("\t");
        for(int i = 0; i < 5; i++){
            printf(" %d    ",buffer[i]);    
        } 
        printf("\n\t\t----\t----\t----\t----\t----\n");
        printf("\t\t ");
        // organize W and R letter
        if(in > out){
            for(int i = 0; i < out; i++){
                printf(" \t ");
            }
            printf("R");
            for(int i = out; i < in; i++){
                printf(" \t ");
            }
            printf("W");
        }else{
            for(int i = 0; i < in; i++){
                printf(" \t ");
            }
            printf("W");
            for(int i = in; i < out; i++){
                printf(" \t ");
            }
            printf("R");
        }   
        printf("\n\n");
    }
   return 0;                                    
}

//********************************************************************
//
// printResults Function
//
// This function prints all results to screen
//
// Return Value
// ------------
// 
//
// Value Parameters
// ----------------

//
// Reference Parameters
// --------------------

//
// Local Variables
// ---------------
// i            int        Loop Iteration Variable.
//
//********************************************************************
 void printResults(){
    
    // Show user formatted results
    printf("PRODUCER / CONSUMER SIMULATION COMPLETE\n");    
    printf("=======================================\n");
    printf("Simulation Time:\t\t\t%d\n",simulationTimeLength);    
    printf("Maximum Thread Sleep Time:\t\t%d\n",maxSleepTimeOfThreads);
    printf("Number of Producer Threads:\t\t%d\n",numberOfProducerThreads);
    printf("Number of Consumer Threads:\t\t%d\n",numberOfConsumerThreads);
    printf("Size of Buffer:\t\t\t\t5\n\n");
    printf("Total Number of Items Produced:\t\t%d\n",totalProduced);
    for(int i = 1; i <= numberOfProducerThreads; i++){
        printf("  Thread %d:\t\t\t\t%d\n",i,producerItemCounter[i]);
    } 
    printf("\n");
    printf("Total Number of Items Consumed:\t\t%d\n",totalConsumed);
    for(int i = 1; i <= numberOfConsumerThreads; i++){
        printf("  Thread %d:\t\t\t\t%d\n",i,consumerItemCounter[i]);
    }
    printf("\n");
    printf("Number of Items Remaining in Buffer:\t%d\n",counterInsertRemove);
    printf("Number Of Times Buffer Was Full:\t%d\n",counterForFullTimes);    
    printf("Number Of Times Buffer Was Empty:\t%d\n",++counterForEmptyTimes);    
 }