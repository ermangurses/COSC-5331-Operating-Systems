//********************************************************************
//
// Erman Gurses
// Foundation of Computer System Software COSC 5331
// Project #2: Part B Calculating Fibonacci sequence by 
//             using one thread. Id of this thread is 0
// October 29, 2012 
// Instructor: Dr. Ajay K. Katangur
//********************************************************************

//********************************************************************
//
// Includes and Defines
//
//********************************************************************
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//********************************************************************
//
// Function Prototypes
//
//********************************************************************
void *Fibonacci(void *t);

//********************************************************************
//
// Global Variables
//
//********************************************************************
int numbersOfFibonacci = 0; 
int * sequenceOfFibonacci;

//********************************************************************
//
// main Function
//
// This function gets sequence number from the user then created a 
// child thread to calculate fibonacci sequence. After child finishes
// its job it prints the results. 
//
// Return Value
// ------------
// 
//
// Value Parameters
// ----------------
// int         argc         It counts how many argument is entered 
//                            in command line  
//
// Reference Parameters
// --------------------
// char          *argv[]    Pointers of Arrays that keeps command 
//                            line parameters 
//
// Local Variables
// ---------------
// threadID      pthread_t  Keeps thread ID.
// rc            int        Control flag to check creation of 
//                            thread succesful or not. 
// i             int        Loop Iteration Variable.
// t             int        Visual ID of thread.
//
//********************************************************************
int main(int argc, char *argv[]){
    // Local variables
    pthread_t threadID;
    int rc, t = 0;
    // Check the user whether is entered command line a sequence 
    // number or not. 
    if(argv[1] == NULL){
        printf("This program requires sequence number for argv[1]\n");
        printf("Please Run one more time the program and enter" 
        "an argument for argv[1]\n");
        exit(0);
    }
    // Parse sequence number from ASCII to Integer
    numbersOfFibonacci = atoi(argv[1]);
    // Allocate memory for the sequence 
    sequenceOfFibonacci = 
                (int *)malloc((numbersOfFibonacci+1)*sizeof(int));
    printf("Main: creating thread %d\n", t);     
    // Create a new thread    
    rc = pthread_create(&threadID, NULL, Fibonacci, (void *)t); 
    
    // Check thread creation whether is succesfull or not
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    // Wait Child thread until it finished its job.
    pthread_join( threadID, NULL); 
    printf("Main: printing the results...\n\n");
    // Print Fibonacci sequence to screen
    for (int i=0; i <= numbersOfFibonacci; i++){
        printf("Fibonacci %d: %d\n",i, sequenceOfFibonacci[i]);      
    }// for
    printf("\nMain: program completed.\n");
    pthread_exit(NULL);
}

//********************************************************************
// compute_pi Function
//
// This function performs a child thread operation. It calculates  
// Fibonacci sequence then puts this value into an array.
//
// Return Value
// ------------
// void                   no return value
//
// Value Parameters
// ----------------
//                        no Value Parameters 
// 
//
// Reference Parameters
// --------------------
// t            char      Visual ID of thread. 
//
// Local Variables
// ---------------
// tid           int      Visual ID of thread. 
//********************************************************************
void *Fibonacci(void *t){
    int tid;
    tid = (int)t;
    printf("Thread %d starting...\n",tid);

    // Check whether the user entered 0 or not 
    if(numbersOfFibonacci == 0){
        sequenceOfFibonacci[0] = 0; 
    }
    else if(numbersOfFibonacci == 1){
        // Check whether the user entered 1 or not 
        sequenceOfFibonacci[0] = 0;
        sequenceOfFibonacci[1] = 1;
    }else{
         // if the user entered greater than 1    
        sequenceOfFibonacci[0] = 0;
        sequenceOfFibonacci[1] = 1;
        // Create the fibonacci sequence  and assign it to the array    
        for (int i = 2; i <= numbersOfFibonacci; i++){
            sequenceOfFibonacci[i] =     
            sequenceOfFibonacci[i-1] + sequenceOfFibonacci[i-2];  
        }// for
      }// if 
      printf("Thread %d finished...\n",tid);
    pthread_exit(NULL);
}