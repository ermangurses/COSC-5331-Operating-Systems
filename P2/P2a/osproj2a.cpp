//*********************************************************
//
// Erman Gurses
// Foundation of Computer System Software COSC 5331
// Project #2: Part A Benchmark Testing on Montecarlo 
//	           Algorithm with multiple threads
// October 29, 2012 
// Instructor: Dr. Ajay K. Katangur
//*********************************************************

//*********************************************************
//
// Includes and Defines
//
//*********************************************************
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#define MAX_THREADS 512


//*********************************************************
//
// Function Prototypes
//
//*********************************************************
void *compute_pi( void * );


//*********************************************************
//
// Global Variables
//
//*********************************************************
int total_hits;
int total_misses;
int hits[ MAX_THREADS ];
int sample_points;
int sample_points_per_thread;
int num_threads;



//********************************************************************
//
// main Function
//
// This function gets number of random numbers and thread numbers from
// command line. Then it creates threads regarding a number of thereads
// are created. 
//
// Return Value
// ------------
// int      retval               program's status
//
// Value Parameters
// ----------------

// Reference Parameters
// --------------------
//
// Local Variables
// ---------------
// ii			   int				Loop Iteration Variable
// p_threads	   pthread_t	    Keeps thread ID.
// attr			   pthread_attr_t   Keeps attribure information. 
// computed_pi	   double			Keeps pi value
// tim             timeval		   	Keeps current time value
// t1		   	   double			Keeps time value in seconds 
//									and micro seconds
// t2			   double	 		Keeps time value in seconds 
//									and micro seconds
//*******************************************************************
int main( int argc, char *argv[] )
{
  /* local variables */
  
  int retval;
  pthread_t p_threads[MAX_THREADS];
  pthread_attr_t attr;
  double computed_pi;

  ii = 0;
  timeval tim;
  double t1, t2;
  
  /* initialize local variables */
  retval = 0;
	
  int AB[1];

  int gfgf = AB[0.4];

  pthread_attr_init( &attr );
  pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );

  /* parse command line arguments into sample points and number of threads */
 
  // Check Number of Sample Points argument is empty or not
  if(argv[1]==NULL)
  {
	printf("Number of Sample Points Argument \"argv[1]\" is empty!!!\n");
	
	exit(0);
  }
	
	 // Check Number of Thread Argument is empty or not
   if(argv[2]==NULL)
  {
	printf("Number of Thread Argument \"argv[2]\" is empty!!!\n");
	
	exit(0);
  }
  
  // Parse command line arguments into sample points and number of threads
  // Ascii to Integer for sample points and number of threads
  sample_points = atoi(argv[1]);
  num_threads = atoi(argv[2]);
  
  // Number of Thread Argument cannot be bigger than 304 or not
  if(num_threads > 304)
  {
	printf("Number of Thread Argument cannot be bigger than 304 \n");
	
	exit(0);
  }
  

  total_hits = 0;
  sample_points_per_thread = sample_points / num_threads;

  
   gettimeofday(&tim, NULL);
   t1=tim.tv_sec+(tim.tv_usec/1000000.0);
  
  for( ii=0; ii<num_threads; ii++ )
    {
      hits[ii] = ii;
      pthread_create( &p_threads[ ii ], &attr, compute_pi, (void *) &hits[ii] );
    }

  
	
  for( ii=0; ii<num_threads; ii++ )
    {
       pthread_join( p_threads[ ii ], NULL );
       total_hits += hits[ ii ];
    }

   computed_pi = 4.0 * (double) total_hits / ((double) (sample_points));
	
	
	// get current time
	gettimeofday(&tim, NULL);
    
	// convert the time to seconds 
	t2=tim.tv_sec+(tim.tv_usec/1000000.0);
		
	// calculate and print elapsed time	
	printf("%.6lf seconds elapsed\n", t2-t1);
   
   // print value of PI
    printf( "Computed PI = %lf\n", computed_pi );


  /* return to calling environment */
  return( retval );
}


//********************************************************************
// compute_pi Function
//
// This function computes PI by using Montecarlo Method
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
// s       void             	
//
// Local Variables
// ---------------
// ii			int				Loop Iteration Variable
// seed			unsigned int	seeder for random function
// hit_pointer	unsigned int		
// rand_no_x	double      	Random number for x coordinate
// rand_no_y	double			Random number for y coordinate
// local_hits	int				
//
//
//*******************************************************************
void *compute_pi( void *s )
{
  unsigned int seed;
  int ii;
  unsigned int *hit_pointer;
  int local_hits;
  double rand_no_x;
  double rand_no_y;

  hit_pointer = (unsigned int *) s;
  seed = *hit_pointer;
  local_hits = 0;

  for( ii=0; ii < sample_points_per_thread; ii++ )
    {
      rand_no_x = (double) (rand_r( &seed ))/(double)RAND_MAX;
	  
      rand_no_y = (double) (rand_r( &seed ))/(double)RAND_MAX;
	  
      if(((rand_no_x - 0.5) * (rand_no_x - 0.5) +
	  (rand_no_y - 0.5) * (rand_no_y - 0.5)) < 0.25)
	    local_hits++;
      seed *= ii;
    }

  *hit_pointer = local_hits;
  pthread_exit(0);
}

