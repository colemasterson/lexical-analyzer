/****************************************************************************
 ** NAME: Cole Masterson                                                   **
 ** CLASS: CSC 446                                                         **
 ** ASSIGNMENT: 3                                                          **
 ** DUE DATE: 2/21/23                                                      **
 ** INSTRUCTOR: Dr. Hamer                                                  **
 ***************************************************************************/

#include "parser.h"
#include "Lexer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>



using namespace std;

/****************************************************************************
 ** FUNCTION: match(TokenType desired)                                     **
 ****************************************************************************
 ** DESCRIPTION: if the current token matches the desired token type then  **
 **              get the next token otherwise throw an error               **
 ** INPUT ARGS: TokenType                                                  **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void match(TokenType desired)
{
    if(token.type == desired)
        token = GetNextToken();
    else
    {
        cout<<"Parse Error. Token: " <<revTokenMap[token.type]<<" Desired Type: "<<revTokenMap[desired]<<endl;
        cin.ignore();
    }

    return;
}

/****************************************************************************
 ** FUNCTION: PROG()                                                       **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for PROG                     **
 **          PROG  ->	modulet idt; DeclarativePart StatementPart endt idt. **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void PROG()
{
    match(TokenType::MODULE);
    match(TokenType::IDENTIFIER);
    match(TokenType::SEMICOLON);
    DECLPART();
    STATEPART();
    match(TokenType::END);
    match(TokenType::IDENTIFIER);
    match(TokenType::PERIOD);
    return;
}

/****************************************************************************
 ** FUNCTION: DECLPART()                                                   **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for DECLPART                 **
 **              DECLPART	->	CONSTPART VARPART PROCPART                 **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void DECLPART()
{
	CONSTPART();
	VARPART();
	PROCPART();
	return;
}


/****************************************************************************
 ** FUNCTION: CONSTPART()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for CONSTPART                **
 **					ConstPart -> constt ConstTail | lambda				   **e
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void CONSTPART()
{
	if(token.type == TokenType::CONST)
	{
		match(TokenType::CONST);
		CONSTTAIL();
	}
	return;
}

/****************************************************************************
 ** FUNCTION: CONSTTAIL()	                                               **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for STAT_LIST                **
 **              ConstTail -> idt = Value ; ConstTail | lambda			   **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void CONSTTAIL()
{
	if(token.type == TokenType::IDENTIFIER)
	{
		match(TokenType::IDENTIFIER);
		match(TokenType::RELOP);
		VALUE();
		match(TokenType::SEMICOLON);
		CONSTTAIL();
	}
	return;
}


/****************************************************************************
 ** FUNCTION: VARPART()                                                    **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for VARS    		           **
 **              VarPart -> vart VarTail | lambda		                   **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void VARPART() 
{
	if(token.type == TokenType::VAR) 
	{
		match(TokenType::VAR);
		VARTAIL();
	}
	return;  
}

/****************************************************************************
 ** FUNCTION: VARTAIL()                                                    **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for SUB_DECL                 **
 ** 			 VarTail -> IdentifierList : TypeMark ; VarTail | lambda   **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void VARTAIL() 
{
	if(token.type == TokenType::IDENTIFIER)
	{
		ID_LIST();
		match(TokenType::COLON);
		TYPEMARK();
		match(TokenType::SEMICOLON);
		VARTAIL();
	}
	return;
}

/****************************************************************************
 ** FUNCTION: ID_LIST()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for ID_LIST                **
 **              ID_LIST		-> 	idt | ID_LIST , idt                   **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void ID_LIST()
{
	match(TokenType::IDENTIFIER);
	if(token.type == TokenType::COMMA)
	{
		match(TokenType::COMMA);
		ID_LIST();
	}
	return;
}

/****************************************************************************
 ** FUNCTION: TYPEMARK()                                                       **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for TYPE                     **
 **              TypeMark -> integert | realt | chart                      **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void TYPEMARK()
{
	if(token.type == TokenType::INTEGER)
		match(TokenType::INTEGER);
	else if(token.type == TokenType::REAL)
		match(TokenType::REAL);
	else
		match(TokenType::CHAR);

	return;
}

/****************************************************************************
 ** FUNCTION: VALUE()                                                      **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for VALUE                    **
 **              Value -> NumericalLiteral			                       **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void VALUE()
{
	match(TokenType::NUMBER);
	return;
}


/****************************************************************************
 ** FUNCTION: PROCPART()                                                   **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for PROCPART                 **
 **              ProcPart -> ProcedureDecl ProcPart | lambda               **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void PROCPART()
{
	if(token.type == TokenType::PROCEDURE)
	{
		PROCDECL();
		PROCPART();
	}
 	return;
}

/****************************************************************************
 ** FUNCTION: PROCDECL()                                                   **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for PROCDECL                 **
 **             ProcDecl -> ProcHeading ; ProcBody idt ;              	   **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void PROCDECL()
{
	PROCHEAD();
	match(TokenType::SEMICOLON);
	PROCBODY();
	match(TokenType::IDENTIFIER);
	match(TokenType::SEMICOLON);
	return;
}


/****************************************************************************
 ** FUNCTION: PROCHEAD()                                                   **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for PROCHEAD                 **
 **              ProcHeading -> proct idt Args			                   **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void PROCHEAD()
{
	match(TokenType::PROCEDURE);
	match(TokenType::IDENTIFIER);
	ARGS();
	return;
}

/****************************************************************************
 ** FUNCTION: PROCBODY()                                                   **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for PROCBODY                 **
 **              ProcBody -> DeclarativePart StatementPart endt            **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void PROCBODY()
{
	DECLPART();
	STATEPART();
	match(TokenType::END);
	return;
}

/****************************************************************************
 ** FUNCTION: ARGS()                                         	           **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for ARGS       	           **
 **              Args -> ( ArgList ) | lambda			                   **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void ARGS() 
{
	if(token.type == TokenType::LPAREN)
	{
		match(TokenType::LPAREN);
		ARG_LIST();
		match(TokenType::RPAREN);
	}
	return;
}

/****************************************************************************
 ** FUNCTION: ARG_LIST()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for ARG_LIST                 **
 **              ArgList -> Mode IdentifierList : TypeMark MoreArgs        **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void ARG_LIST()
{
	MODE();
	ID_LIST();
	match(TokenType::COLON);
	TYPEMARK();
	MORE_ARGS();
	return;
}

/****************************************************************************
 ** FUNCTION: MORE_ARGS()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for MORE_ARGS                **
 **              ArgList -> Mode IdentifierList : TypeMark MoreArgs        **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void MORE_ARGS()
{
	if(token.type == TokenType::SEMICOLON) 
	{
		match(TokenType::SEMICOLON);
		ARG_LIST();
	}
	return;
}

/****************************************************************************
 ** FUNCTION: MODE()                                                       **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for MODE		               **
 **              Mode -> vart | lambda							           **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void MODE() 
{
	if(token.type == TokenType::VAR)
		match(TokenType::VAR);

	return;
}

/****************************************************************************
 ** FUNCTION: STATEPART()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for STATEPART		           **
 **              StatmentPart -> begint SeqOfStatements | lambda           **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void STATEPART() 
{
	if(token.type == TokenType::BEGIN)
	{
		match(TokenType::BEGIN);
		SEQ_STATE();
	}
	return;
}

/****************************************************************************
 ** FUNCTION: SEQ_STATE()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for SEQ_STATE		           **
 **              SeqOfStatements -> lambda						           **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/
void SEQ_STATE() 
{
	return;
}


