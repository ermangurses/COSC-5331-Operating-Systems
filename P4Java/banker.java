//********************************************************************
//
// Erman Gurses
// Foundation of Computer System Software COSC 5331
// Project #4: Implementation of Banker's Algorithm
//
// September 28, 2012 
// Instructor: Dr. Ajay K. Katangur
//********************************************************************

//********************************************************************
//
// Imports and Defines
//
//********************************************************************

import java.io.*;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.StringTokenizer;
import java.lang.String;

//********************************************************************
//
// Banker Class
//
//********************************************************************

public class banker {

//********************************************************************
//
// Class Variables
//
//********************************************************************
 public static  int[][] allocationMatrix = new int[1000][1000];
 public static  int[][] maxMatrix        = new int[1000][1000];
 public static  int[][] needMatrix       = new int[1000][1000];
    
 public static  int[]      available         = new int[1000];
 public static  int[]      request           = new int[1000];
 public static  int[]      work              = new int[1000];
 public static  boolean[]  finish            = new boolean[1000];
    
 public static  int numberOfProcesses;
 public static  int numberOfResourceType;
 public static  int process; 
 public static  int index;    
 
 public static  boolean isSafeState  = true;
 public static  boolean goSafe3      = true;
   
 public static  StringTokenizer stringTokanizer;
 public static  String          delimetersEmpty = " ";
 public static  String          delimetersColon = ":"; 
    
 public static FileInputStream fstream;

 public static DataInputStream in;
 
 public static BufferedReader br;
 public static String   tempString1;
 public static String   tempString2;
 public static String   tempString3; 
 
//********************************************************************
//
// main Method
//
// This Method calls proper methods in proper sequence in order to 
// implement Banker's Algorithm
//
// Return Value
// ------------
// void                         no return value
//
// Value Parameters
// ----------------
//	
//
// Reference Parameters
// --------------------
// args			String[]		String Array that keeps command 
//								line parameters 
//
// Local Variables
// ---------------
//
//*******************************************************************

 public static void main(String[] args) 
 {
		// Methods call
        openReadInputFile(args);
		calculateNeedMatrix();
        printAll ();
        System.out.printf("\n");
        
 }
   
//********************************************************************
// openReadInputFile Method
//
// This Method opens file and reads data from file to 1D and 2D arrays.
// Then it closes file  
//
// Return Value
// ------------
// void                         no return value
//
// Value Parameters
// ----------------
// 								no Value Parameters 
// 
// Reference Parameters
// --------------------
// args			String[]		String Array that keeps command 
//								line parameters   	
//
// Local Variables
// ---------------
// i			int				Loop Iteration Variable.
// j			int				Loop Iteration Variable.
//
//*******************************************************************    
    public static void openReadInputFile (String [] args)
    {
	
					
       try
       {
			// Open input file stream and read from  BufferedReader
             fstream = new FileInputStream(args[0]);
             in 	 = new DataInputStream(fstream);
             br 	 = new BufferedReader(new InputStreamReader(in));
            
           
			// Read first line from file
            tempString1 = br.readLine();  
                
			// Create string tokanizer for tempString1
            stringTokanizer =  new StringTokenizer(tempString1,delimetersEmpty);
            
			// get next token
            tempString2 = stringTokanizer.nextToken();
            
			// parse tempString2 to integer and assign it to numberOfProcesses variable
            numberOfProcesses  = Integer.parseInt(tempString2);
            
			// skip one line 
            tempString1 = br.readLine(); 
            
            
            
            // read next line
            tempString1 = br.readLine(); 
                 
			// Create string tokanizer for tempString1	 
            stringTokanizer =  new StringTokenizer(tempString1,delimetersEmpty);
            
			// get next token
            tempString2 = stringTokanizer.nextToken();
            
			// parse to integer tempString2
            numberOfResourceType  = Integer.parseInt(tempString2);
            
            // skip one line 
            tempString1 = br.readLine();
            
            for(int i = 0; i < numberOfProcesses; i++)
            {
				// read next line
				tempString1 = br.readLine();
				
				// Create string tokanizer for tempString1
                stringTokanizer =  new StringTokenizer(tempString1,delimetersEmpty);
		

				for(int j = 0; j < numberOfResourceType; j++)
				{
					
					// get next token
                    tempString2 = stringTokanizer.nextToken();
					
					// parse tempString2 to integer  and assign to allocationMatrix
                    allocationMatrix[i][j]   = Integer.parseInt(tempString2);
				}

            }
            
			 // skip one line 
             tempString1 = br.readLine();
            
            for(int i = 0; i < numberOfProcesses; i++)
            {
				// read next line
				tempString1 = br.readLine();
                
				// Create string tokanizer for tempString1
                stringTokanizer =  new StringTokenizer(tempString1,delimetersEmpty);
		

				for(int j = 0; j < numberOfResourceType; j++)
				{
					// get next token
                    tempString2 = stringTokanizer.nextToken();
					
					// parse tempString2 to integer  and assign to maxMatrix
                    maxMatrix[i][j] = Integer.parseInt(tempString2);
				}

            }
            
			// skip one line 
            tempString1 = br.readLine();
            
            // read next line
            tempString1 = br.readLine();
            
			// Create string tokanizer for tempString1
            stringTokanizer =  new StringTokenizer(tempString1,delimetersEmpty);

            
            for(int i = 0; i < numberOfResourceType; i++)
            {
				// get next token
				tempString2  = stringTokanizer.nextToken();

				// parse tempString2 to integer  and assign to available array
				available[i] = Integer.parseInt(tempString2);

            }
            
			// skip one line
            tempString1 = br.readLine();
			
			// read next line
            tempString1 = br.readLine();
            
			// assign tempString1 to tempString3
            tempString3 = tempString1;
            
			// Create string tokanizer for tempString3
            stringTokanizer =  new StringTokenizer(tempString3,delimetersColon);
            
			// get next token
            tempString3 = stringTokanizer.nextToken();
                    
			// parse tempString3 to integer  and assign to process variable					
			process = Integer.parseInt(tempString3);
			  
			 // detect index of colon in tempString1 
            index = tempString1.indexOf(":");
            
			// assign substring of tempString1 to tempString3
            tempString1 = tempString1.substring(index+1);
            
			// Create string tokanizer for tempString1
            stringTokanizer =  new StringTokenizer(tempString1,delimetersEmpty);
  
			
            for(int i = 0; i < numberOfResourceType; i++)
            {
				// get next token
				tempString2  = stringTokanizer.nextToken();

				// parse tempString2 to integer  and assign to request array	
				request[i] = Integer.parseInt(tempString2);
				
            }
            
			// close file
            in.close();
       }// catch exception if any of them occurs 
       catch (Exception e)
       {	
			// print error
			System.err.println("Error: " + e.getMessage());
       }
       
       
  }
 //********************************************************************
// printAll Method
//
// This Method prints 	An n x m allocation matrix
//						An n x m max matrix
//						A 1 x m available vector
//						A i : 1 x m request vector
//						and
//						THE SYSTEM IS IN A SAFE STATE! or
//						THE SYSTEM IS NOT IN A SAFE STATE!
// 						and
//						THE REQUEST CAN BE GRANTED! or
//						THE REQUEST CANNOT BE GRANTED!
//
// Return Value
// ------------
// 0			int           	return value 0 to check given parameters
//
// Value Parameters
// ----------------
// void	 						no Value Parameters 
// 
//
// Reference Parameters
// --------------------
 // void	 					no Reference Parameters     	
//
// Local Variables
// ---------------
// i			int				Loop Iteration Variable.
// j			int				Loop Iteration Variable.
//
//*******************************************************************   
  public static int printAll ()
   {
		// Print numberOfProcesses, numberOfResourceType
		System.out.printf("There are %d processes in the system\n\n",numberOfProcesses);
		System.out.printf("There are %d resource types\n\n",numberOfResourceType);

		System.out.printf("The Allocation Matrix is...\n");
		System.out.printf("   ");
		
		if(numberOfProcesses > 9)
		{
			System.out.printf(" ");
				
		}
		
		// Call printfColumn function
		printfColumn();
		System.out.printf("\n");

		// Print allocationMatrix
		for(int i = 0; i < numberOfProcesses; i++)
		{			
			
			if(numberOfProcesses > 9)
			{
				System.out.printf("%2d: ",i);
				
			}
			else
			{
			
				System.out.printf("%d: ",i);
			
			}
			
		
			
			for(int j = 0; j < numberOfResourceType; j++)
			{

				System.out.printf("%d ",allocationMatrix[i][j]);

			}

			System.out.printf("\n");
		}

		System.out.printf("\nThe Max Matrix is...\n");	
		System.out.printf("   ");
		
		if(numberOfProcesses > 9)
		{
			System.out.printf(" ");
				
		}
		
		// Call printfColumn function
		printfColumn();
		System.out.printf("\n");

		// Print maxMatrix
		for(int i = 0; i < numberOfProcesses; i++)
		{
			if(numberOfProcesses > 9)
			{
				System.out.printf("%2d: ",i);
				
			}
			else
			{
			
				System.out.printf("%d: ",i);
			
			}
			
		
		
			for(int j = 0; j < numberOfResourceType; j++)
			{
				System.out.printf("%d ",maxMatrix[i][j]);

			}

			System.out.printf("\n");
		}

		
		System.out.printf("\nThe Need Matrix is...");
		System.out.printf("\n");	
		System.out.printf("   ");
		
		if(numberOfProcesses > 9)
		{
			System.out.printf(" ");
				
		}
		
		// Call printfColumn function
		printfColumn();
		System.out.printf("\n");

		// Print needMatrix 
		for(int i = 0; i < numberOfProcesses; i++)
		{
			if(numberOfProcesses > 9)
			{
				System.out.printf("%2d: ",i);
				
			}
			else
			{
			
				System.out.printf("%d: ",i);
			
			}
			
			for(int j = 0; j < numberOfResourceType; j++)
			{
				System.out.printf("%d ",needMatrix[i][j]);
			}

			System.out.printf("\n");
		}


		System.out.printf("\nThe Available Vector is...\n");
		
		// Call printfColumn function
		printfColumn();
		System.out.printf("\n");
		
		// Print available  array
		for(int i = 0; i < numberOfResourceType; i++)
		{

			System.out.printf("%d ",available[i]);

		}
		
		// Call isSafeOne method
		isSafeOne();
		
		// Check initial state whether is safe or not
		if(isSafeState)
		{
			System.out.printf("\n\nTHE SYSTEM IS IN A SAFE STATE!\n");
		}
		else
		{
			System.out.printf("\n\nTHE SYSTEM IS NOT IN A SAFE STATE!\n");

			return 0;
		
		}
		


		System.out.printf("\nThe Request Vector is...\n");
		System.out.printf("  ");
		
		if(process > 9)
		{
			System.out.printf(" ");
		
		}
		
		// Call printfColumn function
		printfColumn();
		System.out.printf("\n");

		System.out.printf("%s",tempString3);
		System.out.printf(":");
		
		// Print available request array
		for(int i = 0; i < numberOfResourceType; i++)
		{
		
				System.out.printf("%d ",request[i]);

		}
		
		// call checkRequest method 
		if(checkRequest())
		{
			// call isSafeOne method 
			isSafeOne();
	
			// Check the whether the system is in a safe state or not
			if(isSafeState)
			{
				System.out.printf("\n\nTHE REQUEST CAN BE GRANTED!\n");
			}
			else
			{
				System.out.printf("\n\nTHE REQUEST CANNOT BE GRANTED!\n");
				
				return 0;
	
			}

	
		}


		System.out.printf("\nThe Available Vector is...\n");
		
		// Call printfColumn function
		printfColumn();
		
		System.out.printf("\n");
		
		// Print available request array
		for(int i = 0; i < numberOfResourceType; i++)
		{

			System.out.printf("%d ",available[i]);

		}

		return 0;
    
	}
 //********************************************************************
// calculateNeedMatrix Method
//
// This Method calculates needMatrix.
//
// Return Value
// ------------
// void                         no return value
//
// Value Parameters
// ----------------
// void							no Value Parameters 
// 
//
// Reference Parameters
// --------------------
// void                         no return value      	
//
// Local Variables
// ---------------
// i			int				Loop Iteration Variable.
// j			int				Loop Iteration Variable.
//
//*******************************************************************   
	public static void calculateNeedMatrix()
	{

		
		// For each process
		for(int i = 0; i < numberOfProcesses; i++)
		{
			//needMatrix[i] = new int[numberOfResourceType];
			
			// For each resource
			for(int j = 0; j < numberOfResourceType; j++)
			{
				// Calculate each needed resource
				needMatrix[i][j] = maxMatrix[i][j] - allocationMatrix[i][j];
			

			}


		}

	}	
//********************************************************************
// printfColumn Method
//
// This Method prints proper A B C ... for resources columns
//
// Return Value
// ------------
// void                         no return value
//
// Value Parameters
// ----------------
// void							no Value Parameters 
// 
//
// Reference Parameters
// --------------------
// void							no Reference Parameters       	
//
// Local Variables
// ---------------
// i			int				Loop Iteration Variable.
// j			int				Loop Iteration Variable.
//
//*******************************************************************	
	public static void printfColumn ()
	{
      int quotient,
	  remainder,
	  temp = 0;
      
		
	  
		//  Print regular A, B, C ... for Columns if resource equal or
		//  less than 26
		if(numberOfResourceType <= 26 )
		{
            
			for(int i = 0; i < numberOfResourceType; i++)
			{
				System.out.printf("%c ",i+65);
			}
		}
		else
		{
		
			// Calculate howmany 26s are there
			quotient  = numberOfResourceType / 26;
			
			// Calculate howmany letters reminded
			remainder = numberOfResourceType % 26;

			// Print regular A, B, C....
			for(int j = 0; j <26; j++)
			{

				System.out.printf("%c ",j+65);
				
			}
		

			// Print A0 B0 C0.... A1, B1, C1 ....
			for(int i = 0; i < quotient-1; i++)
			{
				for(int j = 0; j < 26; j++)
				{

					System.out.printf("%c%d",j+65,i%10);
				
					temp = i; 
				}
			
			}
			// Print reminded part of  A0 B0 C0.... A1, B1, C1 ....
			for(int i = 0; i < remainder; i++)
			{
					
					System.out.printf("%c%d",i+65,temp+1%10);					
			
			}
	

		}
	}   

//********************************************************************
// isSafeOne Method
//
// This Method implements part 1 of safety algorithm
//
// Return Value
// ------------
// void                         no return value
//
// Value Parameters
// ----------------
// void							no Value Parameters 
// 
//
// Reference Parameters
// --------------------
// void							no Reference Parameters   	
//
// Local Variables
// ---------------
// i			int				Loop Iteration Variable.
//
//*******************************************************************    
	public static void isSafeOne()
	{

		// Assign available array to work array
		for(int i = 0; i <numberOfResourceType; i++)
		{
		
			work[i] = available[i];		
		
		}
		
		// Assign false to finish array
		for(int i = 0; i <numberOfProcesses; i++)
		{
		
			finish[i] = false; 
		}

		// Call isSafeTwo function
		isSafeTwo();
      
	}

//********************************************************************
// isSafeTwo Method
//
// This Method implements part 2 of safety algorithm
//
// Return Value
// ------------
// void                         no return value
//
// Value Parameters
// ----------------
// void							no Value Parameters 
// 
//
// Reference Parameters
// --------------------
// void							no Reference Parameters   	
//
// Local Variables
// ---------------
// i			int				Loop Iteration Variable.
// j			int				Loop Iteration Variable.
//*******************************************************************
	public static void isSafeTwo()
	{	
		goSafe3 = true;

		// Check for all processes
		for(int i = 0; i < numberOfProcesses; i++)
		{
			// if it does not finish
			if(finish[i]==false)
			{
				// Check for all resource types need bigger than available
				// if so, assign false to goSafe 
				for(int j=0; j < numberOfResourceType; j++)
				{
					if(needMatrix[i][j] > work[j]) 
					{
						// set goSafe3 varilable false				
						goSafe3 = false;		
										
					}		

				}
				
				// If goSafe3 true 
				if(goSafe3)
				{	
					// call isSafeThree method
					isSafeThree(i);

					 
				
				}
				// set goSafe3 varilable true
				goSafe3 = true;
			}
		
		}
		// call isSafeFour method
		isSafeFour();
		
	}

//********************************************************************
// isSafeThree Method
//
// This Method implements part 3 of safety algorithm
//
// Return Value
// ------------
// void                         no return value
//
// Value Parameters
// ----------------
// i			int				to keep track which process's 
//								allocation row will add to work
//								vector 
//
// Reference Parameters
// --------------------
// void							no Reference Parameters   	
//
// Local Variables
// ---------------
// j			int				Loop Iteration Variable.
//
//*******************************************************************
	public static void isSafeThree(int i)
	{
		// Add all resource to work array for regarding process
		for(int j=0; j < numberOfResourceType; j++)
		{
            work[j] += allocationMatrix[i][j];
		}
		
		// This process is done
		finish[i] = true;

		// call isSafeTwo method
		isSafeTwo();
	
	}
	
//********************************************************************
// isSafeFour Method
//
// This Method implements part 4 of safety algorithm
//
// Return Value
// ------------
// void                         no return value
//
// Value Parameters
// ----------------
// void							no Value Parameters 
// 
//
// Reference Parameters
// --------------------
// void							no Reference Parameters   	
//
// Local Variables
// ---------------
// i			int				Loop Iteration Variable.
//
//*******************************************************************
	public static void isSafeFour()
	{
		// Check whether all processes are done are not 
		for(int i=0; i < numberOfProcesses; i++)
		{
			// If not it is not safe state
			if(finish[i]==false)
			{
				isSafeState = false;
			}
		}
	

	
	}

//********************************************************************
// checkRequest Method
//
// This Method checks new resource allocation request. 
//
// Return Value
// ------------
// boolean                      if request is accepted 
//									return true
//								else	
//									return false
//
// Value Parameters
// ----------------
// void							no Value Parameters 
// 
//
// Reference Parameters
// --------------------
// void							no Reference Parameters   	
//
// Local Variables
// ---------------
// i			int				Loop Iteration Variable.
//
//*******************************************************************
	public static boolean checkRequest()
	{
		boolean checkForNeedRequest      = true,
			    checkForAvailableRequest = true; 
		
		// Check all processes requests whether are bigger than need or not
		for(int i = 0; i < numberOfResourceType; i++)
		{
			// If so, it is not proper request
			if(needMatrix[process][i] < request[i])
			{
				checkForNeedRequest = false;
					
			}
		
		}
		
		// Print error message, if it is not proper request 
		if(checkForNeedRequest == false)
		{
			
			System.out.printf("\n\nERROR! REQUEST CANNOT BE BIGGER THAN NEEDED!\n");

			checkForNeedRequest = true;
			
			return false;
		}
		

		// Check all processes requests whether are bigger than available or not
		for(int i = 0; i < numberOfResourceType; i++)
		{
			// If so, it is not proper request
			if(available[i] < request[i])
			{
				checkForAvailableRequest = false;
					
			}
		
		}
	
		// Print error message, if it is not proper request 
		if(checkForAvailableRequest == false)
		{
		
			System.out.printf("\nERROR! REQUEST CANNOT BE BIGGER THAN  AVAILABLE!\n");

			checkForAvailableRequest = true; 
			
			return false;
		}

		// If request is proper, give these resources regarding process
		for(int i = 0; i < numberOfResourceType; i++)
		{
			// Subtract request from available
			available[i] = available[i] - request[i];	
		
			// Add request to allocationMatrix regarding process
			allocationMatrix[process][i] = allocationMatrix[process][i] + request[i];	

			// Subtract request from needMatrix regarding process
			needMatrix[process][i] = needMatrix[process][i] - request[i];

		}
		
		return true;

	}

	
	
	
}
    
    

