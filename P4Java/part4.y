%{

#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *str);

extern lineno;

extern * yytext;

#define INTGR 1
#define FLT 2

%}

%start   _start
%token   ASSIGNMENT     	
%token   MINUS         
%token   PLUS          
%token   MULT  		   
%token   DIV  		   
%token   MOD  		   
%token   NOT_EQUAL 	   
%token   GREATER  	   
%token   LESS  		   
%token   LESS_EQUAL    
%token   GREATER_EQUAL 
%token   EQUAL  	   


%token  LEFT_P   	   		 
%token  RIGHT_P   	   
%token  SEMICOLON     
%token  COMMA         
%token  LEFT_B        
%token  RIGHT_B       


%token  _BEGIN  	   	   
%token  DO      	   
%token  ELSE    	   
%token  END     	   
%token  FLOAT        
%token  FOR          
%token  IF           
%token  INTEGER      
%token  THEN         
%token  PRINT        
%token  PROGRAM      
%token  UNTIL        
%token  WHILE        
%token  AND          
%token  OR           
%token  RETURN       

%token  <intVal>  	FCONST
%token  <floatVal>  ICONST      
%token  <stringVal> IDENTIFIER   
%token  STRING       





%union {

	int intVal;

	float floatVal;
	
	char *stringVal; 

}


%%  

	_start					: PROGRAM declarations  _BEGIN statements END SEMICOLON;
	
	declarations 			:  /* empty */
				
							|  declarations declaration 
				
							;
	
	declaration				: type variable_list SEMICOLON {printf("Valid Declaration\n");};
	
							| type error SEMICOLON {printf("Invalid Declaration\n");};
	
						
	
	type		 			: INTEGER  
			
							| FLOAT	   
							
							;	
					
	variable_list 			: variable_list  COMMA variable 
				 
							| variable; 
							
						
				 
	statements  			: /* empty */
	
							| statements  assingment_stmt
					
							| statements if_stmt
					
							| statements do_until_stmt
					
							| statements for_stmt
					
							| statements while_stmt
					
							| statements print_statement
						
							;
					
				
	assingment_stmt			: variable ASSIGNMENT math_expression SEMICOLON {printf("Valid Assignment\n");};
	
							| variable  error SEMICOLON  {printf("Invalid Assignment\n");}
							
							;
	
	
	if_stmt					: IF relational_expression THEN statements if_stmt_rest 
	
							| IF error if_stmt_rest { printf("Invalid If\n");}
							
							;
				
			
	
	
	if_stmt_rest 			: ELSE statements END IF SEMICOLON {printf("Valid If\n");};
						
							| ELSE error SEMICOLON  {printf("Invalid If\n");};
			 
							| END IF SEMICOLON {printf("Valid If\n");};
		
							| END error SEMICOLON { printf("Invalid If\n");}
		
							;
	
	
	do_until_stmt 			: DO statements UNTIL relational_expression SEMICOLON {printf("Valid Do-Until\n");};
	
							| DO error SEMICOLON {printf("Invalid Do-Until\n");}
							
							;
						
						
	
	for_stmt				: FOR variable ASSIGNMENT variable COMMA variable for_stmt_rest 
		
							| FOR error for_stmt_rest  { printf("Invalid For\n");}
							
							;
		
	
	for_stmt_rest 			: COMMA variable statements END FOR SEMICOLON  {printf("Valid For\n");};
		
							| COMMA error SEMICOLON   {printf("Invalid For\n");};
				 
							| statements END FOR SEMICOLON {printf("Valid For\n");};
					
							| statements error SEMICOLON  {printf("Invalid For\n");}
				 
							;

	while_stmt				: WHILE relational_expression _BEGIN statements  END WHILE SEMICOLON {printf("Valid While\n");};
				
							| WHILE error SEMICOLON  {printf("Invalid While\n");}
							
							;
					

	print_statement			: PRINT LEFT_P argument RIGHT_P SEMICOLON  {printf("Valid Print\n");};
					 
							| PRINT error SEMICOLON {printf("Invalid Print\n");}
							
							;
	
	
	
	math_expression 	    : unary_operator factor
							
							| factor  math_expression_rest
							
							;
							
						
						
						
	math_expression_rest	: binary_operator factor
							
							| factor
							
							;
							
							
	relational_expression	: factor relational_operator factor

							;
				
				
	factor					: variable 
	
							| ICONST
								
							| FCONST
			
							;
	
	argument   				: STRING 
			  
							| variable
			   
							;
		
	variable				: IDENTIFIER variable_rest1 {printf("Identifier: %s\n",$1);};
									
									
	
	variable_rest1 			:  /* empty */
	
							|  LEFT_B  factor variable_rest2 RIGHT_B
				 
							;
	
	variable_rest2  		: /* empty */
		
							| variable_rest2 COMMA factor
					
							;
	 
	binary_operator			: MULT 
								
							| DIV 
								
							| MOD
								
							| PLUS 
						 
							| MINUS

							;
	 
	relational_operator  	: LESS 
								
							| LESS_EQUAL 
								
							| GREATER 
								
							| GREATER_EQUAL 
								
							| EQUAL 
								
							| NOT_EQUAL
							
							;
						 
	unary_operator			: MINUS

							;
	
	 
%%
//********************************************************************
//
// main Function
//
// This function calls yyparse function
// 
//
// 
//
//*******************************************************************


int main() 
{

	// if yyparse return different than 0 Compilation was unsuccessful
    if (yyparse())
	{
        
		printf("Compilation was unsuccessful.\n");
	
	}		
  
	

}

//********************************************************************
//
// yyerror Function
//
// This function is called when syntax error occured. 
//	- It prints in which line the syntax error occured 
//  - What token cause the syntax error 
//
// 
//
//*******************************************************************

void yyerror(const char *str)
{
        printf("%d: %s at '%s'\n",lineno,str,yytext);
}
