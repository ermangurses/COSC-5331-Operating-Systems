//********************************************************************
//
// Erman Gurses
// Foundation of Computer System Software COSC 5331
// Project #2: Part C It parses sentence to words by using two threads
//			   Thread 1 prints words started with vowels letter 
//			   and Thread 2 prints words started with  consonant letter
//
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
#define SIZE 2000
//********************************************************************
//
// Function Prototypes
//
//********************************************************************
void *vow(void *t);
void *cons(void *t);

//********************************************************************
//
// Global Variables
//
//********************************************************************

char **temp = new char *[SIZE];
int i = 1; 

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
//	int	     argc				It counts how many argument is entered 
//								in command line  
//
// Reference Parameters
// --------------------
//	char 	*argv[]				Pointers of Arrays that keeps command 
//								line parameters 
//
// Local Variables
// ---------------
// threadID1	pthread_t	    Keeps thread ID.
// threadID2	pthread_t	    Keeps thread ID.
// rc			int				control flag to check creation of thread 
//								succesful or not. 
// i			int				Loop Iteration Variable.
// t			int				Visual ID of thread.
//
//*******************************************************************
int main(int argc, char *argv[])
{
	// Local variables
	pthread_t threadID1;
	pthread_t threadID2;
	int rc, t = 0;
	int i = 0;

	// assign argv to temp until it reaches null
	while(argv[i] != NULL)
	{
	
		 temp[i] = argv[i]; 
		 
		 i++;

	
	}

	printf("Main: creating thread %d\n", t);

	
	rc = pthread_create(&threadID1, NULL, vow, (void *)t); 

	
	printf("Main: creating thread %d\n", t+1);
	
	rc = pthread_create(&threadID2, NULL, cons, (void *)t); 
	
	

	pthread_join( threadID1, NULL); 
	pthread_join( threadID2, NULL); 
	

	
	pthread_exit(NULL);
}


//********************************************************************
// vow Function
//
// This function belongs to cons thread it prints vowel words
//
// Return Value
// ------------
// void                         no return value
//
// Value Parameters
// ----------------
// 								no Value Parameters 
// 
//
// Reference Parameters
// --------------------
// t    	    char            Visual ID of thread.       	
//
// Local Variables
// ---------------
// vowTemp		char			Keeps current word
//
//*******************************************************************
void *vow(void *t)
{
	char * vowTemp;
	
		// check temp array of pointers is noll or not 
		while(temp[i] != NULL)
		{
			
			vowTemp = temp[i];
			
		
				// Check first letter of char array
			if(	vowTemp[0] =='A' || vowTemp[0] =='E' ||  vowTemp[0] =='I' || 
				vowTemp[0] =='O' || vowTemp[0] =='U' ||  vowTemp[0] =='a' || 
				vowTemp[0] =='e' || vowTemp[0] =='i' ||  vowTemp[0] =='o' || 
				vowTemp[0] =='u' )
			{
						// print this word
						printf(" vow: %s\n",vowTemp);
						
						// increase i by 1
						i++;
				
			}
			else// Yield to the other thread
			{
			
				sched_yield();
			
			
			}
				
		
		}
	
	
  
}

//********************************************************************
// cons Function
//
// This function belongs to cons thread it prints consonant words
//
// Return Value
// ------------
// void                         no return value
//
// Value Parameters
// ----------------
// 								no Value Parameters 
// 
//
// Reference Parameters
// --------------------
// t    	    char            Visual ID of thread.           	
//
// Local Variables
// ---------------
// consTemp		char			Keeps current word
//
//
//*******************************************************************
void *cons(void *t)
{
	
	char * consTemp;
	
	// check temp array of pointers is noll or not 
	while(temp[i] != NULL)
	{
		
		//Assign current word to char array. 
		consTemp = temp[i];
		
		// Check first letter of char array
		if(	consTemp[0] !='A'  && consTemp[0] !='E' &&  consTemp[0] !='I' && 
			consTemp[0] !='O'  && consTemp[0] !='U' &&  consTemp[0] !='a' &&
			consTemp[0] !='e'  && consTemp[0] !='i' &&  consTemp[0] !='o' && 
			consTemp[0] !='u' )
		{
			
					// print this word
					printf(" cons: %s\n",consTemp);
					
					// increase i by 1
					i++;
			
			
		}
		else// Yield to the other thread
		{
		
			sched_yield();
		
		
		}
			
	
	}
	
	
  
}


