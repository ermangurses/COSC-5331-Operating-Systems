//*********************************************************
//
// Erman Gurses
// Foundation of Computer System Software COSC 5331
// Project #1: Gnu Island Shell - Writing Your Own Shell
// September 8, 2012 
// Instructor: Dr. Ajay K. Katangur
//*********************************************************


//*********************************************************
//
// Includes and Defines
//
//*********************************************************
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <cstdlib>
#include <time.h>
#include <string.h>
#include <sys/wait.h>    // wait()
#include <string>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
using namespace std;



//*********************************************************
//
// Function Prototypes
//
//*********************************************************
void type_prompt();
void parent( char **toks, pid_t pid );
void child(char **toks);
void ctrlC_handler ( int signo );
void ctrlBS_handler( int signo );
void ctrlZ_handler ( int signo );
void forkOperation(char ** toks, pid_t pid);
int  guishHistory( char ** toks, string history[], int i);
void guishHistoryOptionR( char ** toks, string history[], int i);

//*********************************************************
//
// Extern Declarations
//
//*********************************************************
using namespace std;
extern "C"
{
  extern char **gettoks();
} 


//*********************************************************
//
// Global Variables
//
//*********************************************************

		
 static int ctrlC_counter  = 0;
 static int ctrlBS_counter = 0;
 static int ctrlZ_counter  = 0;
 
 
//********************************************************************
//
// main Function
//
// This function performs function call regarding entered commands
// from users. 
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
// tempStr		string	    	Temporary string for swap operation
// jj			int				Loop Iteration Variable
// i			int				Loop Iteration Variable
// rFlag		int				flag to check whether r is used first 
//								time or not
// rArgument	int				checks argument of r is proper range
// location		int				find proper location in string array
// retval		int				Temporary Return Value
// tempCharArray char 		    it is a pointer of char it keeps 
//								temporary command string
// pch			char			it is a pointer of char it keeps 
//								temporary tokens
// toks 		char 			it is a pointer of char it keeps 
//								tokens
// pid		   pid_t			it goes to fork operation
//*******************************************************************
int main( )
{
  // local variables
  int i,
	  rFlag,
	  rArgument,
	  location,
	  retval;
  
  char **toks;
   
  string history[10];
  string histZero;
  
  // initialize local variables
 
  i =  0;
  rFlag = 0;
  rArgument = 0;
  location = 0;
  toks = NULL;
  retval = 0;
  pid_t  pid;
 
  char * tempCharArray = new char;
  char * pch = new char;
  
  // main (infinite) loop
	while( true )
	{
		// Call signal handler when they are pressed
		signal( SIGINT,  ctrlC_handler );
		signal( SIGQUIT, ctrlBS_handler );
		signal( SIGTSTP, ctrlZ_handler );
 
		// call guish prompt 
		type_prompt();	

		// get tokens from lexical analyzer 
		toks = gettoks();

		// check whether token is entered or not 
		if( toks[0] != NULL )
		{	
			
			// Keep last command string into temporary string 
			histZero = history[0];
			
			// call guishHistory function
			i = guishHistory(toks,history,i);
			
			// check toks[0] wheter is r or nor and
			// check r entered whether is first time or not
			if(!strcmp( toks[0], "r") && rFlag)
			{
			
				// check (toks[1] whether is null time or not
				if(toks[1]==NULL)
				{
				
					// convert command string into tokens
					strcpy (tempCharArray, history[i-2].c_str());
					int jj = 0;
			
					pch = strtok (tempCharArray," ");
					toks[jj] = pch;
					
					jj++;
					
					while (pch != NULL)
					{
						
						pch = strtok (NULL," ");
						
						toks[jj] = pch;
						
						jj++;
						
					}
					
					printf("-Guish: r command is: %s\n", tempCharArray);
					history[i-1] = history[i-2];
				
				}
				else
				{	
					// gets r's argument
					rArgument = atoi(toks[1]);
					
					
					// check rArgument is proper  				
					if( rArgument < 11 && rArgument > 1 )
					{
						
						if( i == 10 )
						{
							// find coirect location in history array
							location = 10 - rArgument;
							
							// convert command string into tokens
							strcpy (tempCharArray, history[i-2-location].c_str());
							int jj = 0;
							
							printf("-Guish: r %d command is: %s\n", rArgument, tempCharArray);
									
							
							pch = strtok (tempCharArray," ");
							toks[jj] = pch;
							jj++;
							
							while (pch != NULL)
							{
								
								pch = strtok (NULL," ");
								
								toks[jj] = pch;
								
								jj++;
								
							}
							
							history[i-1] = history[i-location-2];
												
						}
						else if( i < 10 && i >= rArgument)
						{
							// find coirect location in history array
							location = i - rArgument - 1;
							
							// convert command string into tokens
							strcpy (tempCharArray, history[i-2-location].c_str());
							int jj = 0;
							
							printf("-Guish: r %d command is: %s\n", rArgument, tempCharArray);
							
							pch = strtok (tempCharArray," ");
							toks[jj] = pch;
							jj++;
							
							while (pch != NULL)
							{
								
								pch = strtok (NULL," ");
								
								toks[jj] = pch;
								
								jj++;
								
							}			
							
							history[i-1] = history[i-location-2];							
						}
					
					}
					else if(rArgument == 1) 
					{
						
						// finds correct location in history array
						strcpy (tempCharArray, histZero.c_str());
						int jj = 0;
												
						printf("-Guish: r %d command is: %s\n", rArgument, tempCharArray);

						
						// convert command string into tokens
						pch = strtok (tempCharArray," ");
						toks[jj] = pch;
						jj++;
						
						while (pch != NULL)
						{
							
							pch = strtok (NULL," ");
							
							toks[jj] = pch;
							
							jj++;
							
						}
						
						history[i-1] = histZero;					
					
					}
					else
					{
						
						printf("Argument r must be between 1 and 10 inclusive\n");
						
						
					
					}
			
			
				}
				
			}
			
			
			if( !strcmp( toks[0], "exit" ))
			{
			
				// prompt the user how many times signals occured.
				cout<< "Ctrl C is Pressed: "<<ctrlC_counter<<" Times"<<endl;
				cout<< "Ctrl \\ is Pressed: "<<ctrlBS_counter<<" Times"<<endl;
				cout<< "Ctrl Z is Pressed: "<<ctrlZ_counter<<" Times"<<endl;
					
				exit(0);
			
			}
			else if( !strcmp( toks[0], "hist" ))
			{
				// show user when hist command is entered
				if(i<10)
				{
						for(int x = 0; x < i; x++ )
								cout<<x+1<<" "<<history[x]<<endl;
						
				}
				else
				{
						for(int x = 0; x < 10; x++ )
								cout<<x+1<<" "<<history[x]<<endl;
					
				
				}
					
				rFlag = 1;
			
			}
			else
			{	
				// apply external command with child process
				rFlag = 1; 
				forkOperation(toks,pid);
	
			}
							
		}		
		
	}
  
	

  // return to calling environment
  return( retval );
  
}

//********************************************************************
//
// guishHistory Function
//
// This function keeps history information in the history array.
// After the array is full, it gets new command in to last element
// of array then shifts all array. So It always keeps last 10
// commands. 
//
// Return Value
// ------------
// int      i                   returns location of last enrty of
//								history array.
//
// Value Parameters
// ----------------
// int		i					it is previous location of entry
// 
//
// Reference Parameters
// --------------------
// toks     char             	The location where toks's reference
//								is found
// history 	string[]			The location of first element in the 
//								history array
//
// Local Variables
// ---------------
// tempStr		string	    	Temporary string for swap operation
// 
//*******************************************************************
int guishHistory( char ** toks, string history[], int i)
{
	
	string tempStr;
	
			if( i < 10 )
			{	
				// convert tokens to string 
				for(int j=0; toks[j] != NULL; j++ )
				{
					history[i].append(toks[j]);
					history[i].append(" ");
			  
				}
				i++;
			}
			else
			{
				// convert tokens to string 
				for(int j=0; toks[j] != NULL; j++ )
				{
					tempStr.append(toks[j]);
					tempStr.append(" ");
			  
				}
			
				
				// shift strings in string array
				for(int j = 0;  j < 9; j++)
				{
					history[j] = history[j+1];
				
				}
				
				// put last element in to head of string array
				history[9]=tempStr;
			
				// clear temp string 
				tempStr.clear();
			
			}		
			
	return i;
}


//********************************************************************
//
// parent Function
//
// This function performs a parent process operations. Mainly, it
// checks child's status. This status is showed to the user.
//
// Return Value
// ------------
// void                         no return value
//
// Value Parameters
// ----------------
// pid		pid_t				child's process id
// 
//
// Reference Parameters
// --------------------
// toks     char             	The location where toks's reference
//								is found
//
// Local Variables
// ---------------
// stat		int		status of child
// 
//*******************************************************************
void parent(char **toks, pid_t pid )
{
	// local variables
	int stat;	

		
		waitpid(pid,&stat,0);
		// check whether chils is teminated normally or not
		if (WIFEXITED(stat))
		{	
			// check child's status
			if(WEXITSTATUS(stat) == 1)
			{
					
				if(toks[1] != NULL)
				{
					cout<<toks[0]<<": cannot access "<<toks[1]<<endl;
						
					cout<<"[ Program returned exit code 1 ] "<<endl;
					
				}

			}	// check child's status
			else if(WEXITSTATUS(stat) == 2)
			{
				
				cout<<toks[0]<<": cannot access "<<toks[1]<<endl;
				
				cout<<"[ Program returned exit code 2 ] "<<endl;
			}
			
		}
		else
		{
		
			cout<<"process is not terminated normally"<<endl;
		
		}	
}

//********************************************************************
// child Function
//
// This function performs a child process operations. Mainly, it does
// all background operations which is executed by exec family
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
// toks     char             	The location where toks's reference
//								is found
//
// Local Variables
// ---------------
// flagOut		bool	indicates whether output redirection symbol 
//						is avaiable or not
// flagIn		bool	indicates whether input redirection symbol 
//						is avaiable or not
// in			int		file descriptor for input
// out			int		file descriptor for output
// counter		int		counter for detect length of command line
// counter1		int		counter for detect location of ">" character
//						array						
// counter2		int		counter for detect location of "<" character
//						array
// ii			int		Loop Iteration Variable
// newToks		char 	temporary storage for tokens
//*******************************************************************
void child(char **toks )
{
	// local variables
	bool flagOut = false,
		 flagIn =  false;
		 
	int in,
		out,
		counter = 0,
		counter1 = 0,
		counter2 = 0;
		
		
		// check command string is there any > or < exist
		for( int ii=0; toks[ii] != NULL; ii++ )
		{
	    
			if(!strcmp( toks[ii], ">"))
			{			
				flagOut = true;	
			}
			
			if(!strcmp( toks[ii], "<"))
			{			
				flagIn = true;
			}
			
			counter++;
			
		}
		
		// if just output directions exist 
		if(flagOut && !flagIn)
		{	
			// create temporary array of pointer 
			char *newToks[100];
			
			
			// initialize array with keyword
			for( int ii=0; newToks[ii] != NULL; ii++ )
			{
			
				newToks[ii] = "erman";
				
			}
			
	
			// copy toks up to > token
			while( strcmp( toks[counter1], ">"))
			{	
				
				newToks[counter1]=toks[counter1];				
					
				counter1++;
			}
					
			// erase keywords		
			for( int ii=0; newToks[ii] != NULL; ii++ )
			{
			
				if(!strcmp( newToks[ii], "erman"))
				{
				
					newToks[ii]=NULL;

				
				}
				
							
			}
			
			// get file descriptor 
			out = open(toks[counter-1], O_CREAT|O_TRUNC|O_WRONLY, 0777);
			
			// duplicate stdout file descriptor to out file descriptor
			dup2(out, 1);
				
			
			// execute command
			execvp(newToks[0],newToks);	
				
			// indicate error if occurs
			perror(newToks[0]);
				
			exit(EXIT_FAILURE);
			
		}
		
		// if just input directions exist 
		if(flagIn && !flagOut)	
		{	
			// create temporary array of pointer 
			char *newToks[100];

			// initialize array with keyword
			for( int ii=0; newToks[ii] != NULL; ii++ )
			{
			
				newToks[ii] = "erman";
				
			}
			
			// copy toks up to < token
			while( strcmp( toks[counter2], "<"))
			{	
				
				newToks[counter2]=toks[counter2];				
					
				counter2++;
			}
					
			// erase keywords	
			for( int ii=0; newToks[ii] != NULL; ii++ )
			{
			
				if(!strcmp( newToks[ii], "erman"))
				{
				
					newToks[ii]=NULL;

				
				}
				
							
			}
			
			// get file descriptor 
			in = open(toks[counter-1], O_RDONLY);
		
			// duplicate stdout file descriptor to out file descriptor
			dup2(in, 0);
			
			
			// execute command
			execvp(newToks[0],newToks);	
			
			// indicate error if occurs
			perror(newToks[0]);
				
			exit(EXIT_FAILURE);
		}
		
		// flagIn and flagOut exist
		if(flagIn && flagOut)	
		{
			// create temporary array of pointer 
			char *newToks[100];
				
			// initialize array with keyword
			for( int ii=0; newToks[ii] != NULL; ii++ )
			{
			
				newToks[ii] = "erman";
				
			}
			
			
			// find location of > token
			while( strcmp( toks[counter1], ">"))
			{		
				counter1++;
			}
			
			// find location of < token
			while( strcmp( toks[counter2], "<"))
			{			
				counter2++;
			}
			
			// check which token has lower subscript
			if(counter1 < counter2)
			{
			
				counter1 = 0; 
				// copy toks up to > token
				while( strcmp( toks[counter1], ">"))
				{	
				
					newToks[counter1]=toks[counter1];				
					
					counter1++;
				}
			
			}
			else
			{
				
				counter2 = 0; 
				// copy toks up to < token
				while( strcmp( toks[counter2], "<"))
				{	
				
					newToks[counter2]=toks[counter2];				
					
					counter2++;
				}
			
					
			}
			// erase keywords
			for( int ii=0; newToks[ii] != NULL; ii++ )
			{
			
				if(!strcmp( newToks[ii], "erman"))
				{
				
					newToks[ii]=NULL;

				
				}
				
							
			}
			
			
			//  get file descriptors according to < and > tokens
			if(counter1 < counter2)
			{
				//get file descriptors
				in = open(toks[counter-1], O_RDONLY);
				out = open(toks[counter-3], O_CREAT|O_TRUNC|O_WRONLY, 0777);
				
				// duplicate stdin file descriptor to out file descriptor
				dup2(in, 0);
				
				// duplicate stdout file descriptor to out file descriptor
				dup2(out, 1);
				
				
				// execute entered command
				execvp(newToks[0],newToks);	
				
				perror(newToks[0]);
					
				exit(EXIT_FAILURE);
			
			}
			else
			{
				//get file descriptors
				in = open(toks[counter-3], O_RDONLY);
				out = open(toks[counter-1], O_CREAT|O_TRUNC|O_WRONLY, 0777);
				
				// duplicate stdin file descriptor to out file descriptor
				dup2(in, 0);
				
				// duplicate stdout file descriptor to out file descriptor
				dup2(out, 1);
			
				
				// execute entered command
				execvp(newToks[0],newToks);	
				
				// indicate error if occurs
				perror(newToks[0]);
					
				exit(EXIT_FAILURE);
		
			}
		
			
			
		}
		
		// flagIn and flagOut do not exist
		if(!flagIn && !flagOut)
		{
		
				int isFound = 1;
				
				// execute command
				isFound = execvp(toks[0], toks);
				
				
				// indicate an error if occurs
				if(isFound == -1 && toks[1] == NULL)
				{
					cout<<"-Guish: "<<toks[0]<<": command not found"<<endl;
					
				
				}

				exit(EXIT_FAILURE);
		

		}

}

//********************************************************************
// type_prompt Function
//
// This function performs creates prompt for Guish
//
// Return Value
// ------------
// void                         no Return value
//
// Value Parameters
// ----------------
// 								no Value Parameters
// 
//
// Reference Parameters
// --------------------
// 								no Reference Parameters
//
//
// Local Variables
// ---------------
// delimeterPos size_t			variable for detecting last "/"
//								character in string
// rawtime		time_t			variable for ctime() function
// i			int				counter for detecting of'\n' 
//								character in  cursorTimestring 
// counter		static int		counter for cursor number on shell
// cwd			string			current working directory indicator
//								for cursor
// cursorTime	string			variable for date and time information
//								on the cursor
//
//*******************************************************************
void type_prompt()
{ 	
	// local variables
	size_t delimeterPos;
	time_t rawtime;
	int i;
	string cwd,
		   cursorTime; 
		   
	static int counter = 0;	 // commmand line number
	i = 0;	   
	
	// call time function
	time ( &rawtime );

	// get cursor time
	cursorTime = ctime (&rawtime);
	
	// find return key's location
	while(cursorTime[i]!='\n')
	{
			i++;
		
	}
	
	// get rid if return key from string
	if(cursorTime[i]=='\n')
	{
			cursorTime[i]='\0';
	}
		
	// get current directory name
	cwd = get_current_dir_name();	
	delimeterPos = cwd.find_last_of("/");

	// print cursor
	cout<<"["<<cursorTime<<" "
		<<cwd.substr(delimeterPos+1)
		<<" Prompt "<<counter++<<"]$ ";
	
}

//********************************************************************
//
// ctrlC_handler Function
//
// This function handles when user push ctrl c and counts it
//
// Return Value
// ------------
// 
//
// Value Parameters
// ----------------
// 
// int	signo					Indicates signal number
//
// Reference Parameters
// --------------------
// 
//
// Local Variables
// ---------------
// 
//
//*******************************************************************
void ctrlC_handler( int signo )
{ 
  // count howmany times ctrl c is entered
  ctrlC_counter++;
  
}


///********************************************************************
//
// ctrlBS_handler Function
//
// This function handles when user push ctrl \ and counts it
//
// Return Value
// ------------
// 
//
// Value Parameters
// ----------------
// 
// int	signo					Indicates signal number
//
// Reference Parameters
// --------------------
// 
//
// Local Variables
// ---------------
// 
//
//*******************************************************************
void ctrlBS_handler( int signo )
{
   // count howmany times ctrl back slash is entered
  ctrlBS_counter++;
 
  
  
}

//********************************************************************
//
// ctrlZ_handler Function
//
// This function handles when user push ctrl z and counts it
//
// Return Value
// ------------
// 
//
// Value Parameters
// ----------------
// 
// int	signo					Indicates signal number
//
// Reference Parameters
// --------------------
// 
//
// Local Variables
// ---------------
// 
//
//*******************************************************************
void ctrlZ_handler( int signo )
{
    // count howmany times ctrl_z is entered
  ctrlZ_counter++;
 
  
}

//********************************************************************
//
// forkOperation Function
//
// This function performs fork operation so the process becomes 2 
// identical processes except their process IDs
//
// Return Value
// ------------
// void                         no return value
//
// Value Parameters
// ----------------
// pid		pid_t				keep process id of child and 0
//
//
// Reference Parameters
// --------------------
// toks     char             	The location where toks's reference
//								is found
// Local Variables
// ---------------
// 
// 
//
//*******************************************************************
void forkOperation(char ** toks, pid_t pid)
{
	// fork operation
	pid = fork();	
		
	if( pid < 0 ) /* error occurred */
	{ 	
		
		fprintf(stderr, "Fork Failed");
		
		exit(-1);
	}
	else if (pid == 0) /* call child process */
	{
		
		child(toks);
			
	}
	else /* call parent process */
	{
		 
		 parent(toks,pid);
				
	}

}


