//********************************************************************
//
// Erman Gurses
// Foundation of Computer System Software COSC 5331
// Project #4: Implementation of Bankers Algorithm
//
// September 28, 2012 
// Instructor: Dr. Ajay K. Katangur
//********************************************************************

//********************************************************************
//
// Includes and Defines
//
//********************************************************************
#include <iostream>
#include <fstream>
#include <string>    
#include <cstring>
#include <ctype.h>


using namespace std;

//********************************************************************
//
// Function Prototypes
//
//********************************************************************
bool openInputFile(ifstream & inFile, char *argv[]); 
void readInputFile(ifstream & inFile);
void calculateNeedMatrix();
void printAll();
void isSafe1();
void isSafe2();
void isSafe3(int i);
void isSafe4();
void printfColumn();
bool checkRequest();

//********************************************************************
//
// Global Variables
//
//********************************************************************
int ** allocationMatrix = NULL,
    ** maxMatrix        = NULL,
    ** needMatrix       = NULL,
    * available         = NULL,
    * request           = NULL,
    * work              = NULL,
    * finish            = NULL,    
    numberOfProcesses,
    numberOfResourceType;
    
bool isSafeState = true,
     goSafe3     = true;

//********************************************************************
//
// main Function
//
// This function gets sequence number from the user then created a 
// child thread to calculate fibonacci sequence. After child 
// finishes its job it prints the results. 
//
// Return Value
// ------------
// 
//
// Value Parameters
// ----------------
// int         argc            It counts how many argument is entered 
//                               in command line  
//
// Reference Parameters
// --------------------
//    char     *argv[]         Pointers of Arrays that keeps command 
//                               line parameters 
//
// Local Variables
// ---------------
// threadID1    pthread_t      Keeps thread ID.
// threadID2    pthread_t      Keeps thread ID.
// rc           int            Control flag to check creation of 
//                               thread succesful or not. 
// i            int            Loop Iteration Variable.
// t            int            Visual ID of thread.
//
//********************************************************************

int main(int argc, char *argv[]){
    ifstream inFile;
    openInputFile(inFile, argv);
    readInputFile(inFile);
    calculateNeedMatrix();
    printAll();
    printf("\n");
  return 0;
}

//********************************************************************
// openInputFile Function
//
// This function belongs to cons thread it prints vowel words
//
// Return Value
// ------------
// void                         No return value
//
// Value Parameters
// ----------------
//                              No Value Parameters 
// 
//
// Reference Parameters
// --------------------
// t            char            Visual ID of thread.           
//
// Local Variables
// ---------------
// vowTemp      char            Keeps current word
//
//********************************************************************
bool openInputFile(ifstream & inFile, char *argv[]){
   inFile.open(argv[1]);
   if (!inFile){
      cout <<"The input File cannot been opened!!!\n";
      return false;
   }
   return true;
}

//********************************************************************
// readInputFile Function
//
// This function belongs to cons thread it prints vowel words
//
// Return Value
// ------------
// void                     No return value
//
// Value Parameters
// ----------------
//                          No Value Parameters 
// 
//
// Reference Parameters
// --------------------
// t            char        Visual ID of thread.           
//
// Local Variables
// ---------------
// vowTemp      char        Keeps current word
//
//********************************************************************
void readInputFile(ifstream & inFile){
    
    string tempString1,
           tempString2;
    getline(inFile, tempString1); 
    numberOfProcesses = atoi(tempString1.data());
    allocationMatrix  = new int*[numberOfProcesses];
    maxMatrix         = new int*[numberOfProcesses];
    getline(inFile, tempString1); 
    getline(inFile, tempString1); 
    numberOfResourceType = atoi(tempString1.data());
    available    =  new int[numberOfResourceType]; 
    request      =  new int[numberOfResourceType+1]; 
    getline(inFile, tempString1); 
    for(int i = 0; i < numberOfProcesses; i++){
    	allocationMatrix[i] = new int[numberOfResourceType]; 
        getline(inFile, tempString1); 
        size_t index = 0;

        for(int j = 0; j < numberOfResourceType; j++){
            
            tempString2 = tempString1.at(index);
            allocationMatrix[i][j] =atoi(tempString2.data()); 
            index += 2;
        }
    }
    getline(inFile, tempString1);
    for(int i = 0; i < numberOfProcesses; i++){
        maxMatrix[i] = new int[numberOfResourceType];
        getline(inFile, tempString1); 
        size_t index = 0;
        for(int j = 0; j < numberOfResourceType; j++){
            tempString2 = tempString1.at(index);
            maxMatrix[i][j] = atoi(tempString2.data()); 
            index+=2;
        }
    }
    getline(inFile, tempString1); 
    getline(inFile, tempString1); 
    size_t index = 0;
    for(int i = 0; i < numberOfResourceType; i++){

        tempString2 = tempString1.at(index);
        available[i] =  atoi(tempString2.data()); 
        index+=2;
    }
    getline(inFile, tempString1); 
    getline(inFile, tempString1);
    index = 0; 
    for(int i = 0; i < numberOfResourceType+1; i++){
    	tempString2 = tempString1.at(index);
        request[i] =  atoi(tempString2.data()); 
        index+=2;
    }
}
//********************************************************************
// calculateNeedMatrix Function
//
// This function belongs to cons thread it prints vowel words
//
// Return Value
// ------------
// void                     No return value
//
// Value Parameters
// ----------------
//                          No Value Parameters 
// 
//
// Reference Parameters
// --------------------
// needMatrix   int         maxMatrix - allocationMatrix     
//
// Local Variables
// ---------------
// vowTemp      char        Keeps current word
//
//********************************************************************
void calculateNeedMatrix(){
    needMatrix = new int*[numberOfProcesses];
    for(int i = 0; i < numberOfProcesses; i++){
        needMatrix[i] = new int[numberOfResourceType];
        for(int j = 0; j < numberOfResourceType; j++){
            needMatrix[i][j] = maxMatrix[i][j] - allocationMatrix[i][j];
        }
    }
}

//********************************************************************
// printfColumn Function
//
// This function belongs to cons thread it prints vowel words
//
// Return Value
// ------------
// void                       No return value
//
// Value Parameters
// ----------------
//                            No Value Parameters 
// 
//
// Reference Parameters
// --------------------
// t            char          Visual ID of thread.           
//
// Local Variables
// ---------------
// vowTemp      char          Keeps current word
//
//*******************************************************************
void printfColumn(){   
    if(numberOfResourceType <= 26 ){
        for(int i = 0; i < numberOfResourceType; i++){
            printf("%c ",i+65);
        }
    }else{
        int quotient,
            remainder,
            temp;
        quotient  = numberOfResourceType / 26;
        remainder = numberOfResourceType % 26;

        for(int j = 0; j <26; j++){
            printf("%c ",j+65);
        }
        for(int i = 0; i < quotient-1; i++){
            for(int j = 0; j < 26; j++){
                printf("%c%d",j+65,i%10);
                temp = i; 
            } 
        }
        for(int i = 0; i < remainder; i++){    
            printf("%c%d",i+65,temp+1%10);        
        }
    }
}

//********************************************************************
// printAll Function
//
// This function belongs to cons thread it prints vowel words
//
// Return Value
// ------------
// Void                      No return value
//
// Value Parameters
// ----------------
//                           No Value Parameters 
// 
//
// Reference Parameters
// --------------------
// t            char         Visual ID of thread.           
//
// Local Variables
// ---------------
// vowTemp      char         Keeps current word
//
//*******************************************************************
void printAll(){ 
    printf("There are %d processes in the system\n\n",numberOfProcesses);
    printf("There are %d resource types\n\n",numberOfResourceType);
    printf("The Allocation Matrix is...\n");
    printf("   ");
    printfColumn();
    printf("\n");
    for(int i = 0; i < numberOfProcesses; i++){    
    
        printf("%d: ",i);
        for(int j = 0; j < numberOfResourceType; j++){
            printf("%d ",allocationMatrix[i][j]);
        }
        printf("\n");
    }
    printf("\nThe Max Matrix is...\n");    
    printf("   ");
    printfColumn();
    printf("\n");

    for(int i = 0; i < numberOfProcesses; i++){
        printf("%d: ",i);
        for(int j = 0; j < numberOfResourceType; j++){
            printf("%d ",maxMatrix[i][j]);
        }
        printf("\n");
    }
    printf("\nThe Need Matrix is...");
    printf("\n");    
    printf("   ");
    printfColumn();
    printf("\n");

    for(int i = 0; i < numberOfProcesses; i++){
        printf("%d: ",i);
        for(int j = 0; j < numberOfResourceType; j++){
            printf("%d ",needMatrix[i][j]);
        }
        printf("\n");
    }
    printf("\nThe Available Vector is...\n");
    printfColumn();
    printf("\n");
    for(int i = 0; i < numberOfResourceType; i++){
        printf("%d ",available[i]);
    }
    isSafe1();
    if(isSafeState){
        printf("\n\nTHE SYSTEM IS IN A SAFE STATE!\n");
    }else{
        printf("\n\nTHE SYSTEM IS NOT IN A SAFE STATE!\n");
        return;
    }
    printf("\nThe Request Vector is...\n");
    printf("  ");
    printfColumn();
    printf("\n");

    for(int i = 0; i < numberOfResourceType+1; i++){
        if(i == 0){
            printf("%d",request[i]);
            printf(":");
        }else{
            printf("%d ",request[i]); 
        }
    }
    if(checkRequest()){
        isSafe1();
        if(isSafeState){
            printf("\n\nTHE REQUEST CAN BE GRANTED!\n");
        }else{
            printf("\n\nTHE REQUEST CANNOT BE GRANTED!\n");
        }
    }
    printf("\nThe Available Vector is...\n");
    printfColumn();
    printf("\n");
    for(int i = 0; i < numberOfResourceType; i++){
        printf("%d ",available[i]);
    }
}

//********************************************************************
// isSafe1 Function
//
// This function belongs to cons thread it prints vowel words
//
// Return Value
// ------------
// void                     No return value
//
// Value Parameters
// ----------------
//                          No Value Parameters 
// 
//
// Reference Parameters
// --------------------
// t            char        Visual ID of thread.           
//
// Local Variables
// ---------------
// vowTemp      char        Keeps current word
//
//********************************************************************
void isSafe1(){
    work    =  new int[numberOfResourceType]; 
    finish  =  new int[numberOfProcesses];
    for(int i = 0; i <numberOfResourceType; i++){ 
    	work[i] = available[i];    
    }
    for(int i = 0; i <numberOfProcesses; i++){
        finish[i] = false; 
    }
    isSafe2();   
}

//********************************************************************
// isSafe2 Function
//
// This function belongs to cons thread it prints vowel words
//
// Return Value
// ------------
// void                     no return value
//
// Value Parameters
// ----------------
//                          no Value Parameters 
// 
//
// Reference Parameters
// --------------------
// t            char        Visual ID of thread.           
//
// Local Variables
// ---------------
// vowTemp      char        Keeps current word
//
//********************************************************************
void isSafe2(){
    goSafe3 = true;
    for(int i = 0; i < numberOfProcesses; i++){
        if(finish[i]==false){
            for(int j=0; j < numberOfResourceType; j++){
                if(needMatrix[i][j] > work[j]){                    
                    goSafe3 = false;                             
                 }        
            }
            if(goSafe3){
                 isSafe3(i);
            }
            goSafe3 = true;
        }
    }
    isSafe4();    
}

//********************************************************************
// isSafe3 Function
//
// This function belongs to cons thread it prints vowel words
//
// Return Value
// ------------
// void                     No return value
//
// Value Parameters
// ----------------
//                          No Value Parameters 
// 
//
// Reference Parameters
// --------------------
// t            char        Visual ID of thread.           
//
// Local Variables
// ---------------
// vowTemp      char        Keeps current word
//
//********************************************************************
void isSafe3(int i){
    for(int j = 0; j < numberOfResourceType; j++){
     	work[j] += allocationMatrix[i][j];
    } 
    finish[i] = true;
    isSafe2(); 
}
//********************************************************************
// isSafe4 Function
//
// This function belongs to cons thread it prints vowel words
//
// Return Value
// ------------
// void                     No return value
//
// Value Parameters
// ----------------
//                          No Value Parameters 
// 
//
// Reference Parameters
// --------------------
// t            char        Visual ID of thread.           
//
// Local Variables
// ---------------
// vowTemp      char        Keeps current word
//
//********************************************************************
void isSafe4(){
    for(int i = 0; i < numberOfProcesses; i++){
        if(finish[i]==false){
            isSafeState = false;
        }
    }
}

//********************************************************************
// checkRequest Function
//
// This function belongs to cons thread it prints vowel words
//
// Return Value
// ------------
// void                     No return value
//
// Value Parameters
// ----------------
//                          No Value Parameters 
// 
//
// Reference Parameters
// --------------------
// t            char        Visual ID of thread.           
//
// Local Variables
// ---------------
// vowTemp      char        Keeps current word
//
//********************************************************************
bool checkRequest(){
    bool checkForNeedRequest      = true,
         checkForAvailableRequest = true; 
    int  process;     
    process = request[0];
    
    for(int i = 0; i < numberOfResourceType; i++){
        if(needMatrix[process][i] < request[i+1]){
            checkForNeedRequest = false;
        }
    }
    if(checkForNeedRequest == false){
        printf("\n\nERROR! REQUEST CANNOT BE BIGGER THAN NEEDED!\n");
        return false;
    }
    for(int i = 0; i < numberOfResourceType; i++){
    
        if(available[i] < request[i+1]){
            checkForAvailableRequest = false;
        }
    }
    if(checkForAvailableRequest == false){
        printf("\nERROR! REQUEST CANNOT BE BIGGER THAN NEEDED AVAILABLE!\n");
        return false;
    }
    for(int i = 0; i < numberOfResourceType; i++){
        available[i] -= request[i+1];    
        allocationMatrix[process][i] += request[i+1];   
        needMatrix[process][i] -= request[i+1];
    }
    return true;
}

