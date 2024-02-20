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

void match(TokenType desired){
  if(token.type == desired){
    token = GetNextToken();
  }
  else{
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

void PROG(){
  match(TokenType::MODULE);
  match(TokenType::IDENTIFIER);
  match(TokenType::SEMICOLON);
  DECLS();
  SUB_PROGS();
  COMP_STAT();
  match(TokenType::PERIOD);
  return;
}

/****************************************************************************
 ** FUNCTION: DECLS()                                                      **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for DECLS                    **
 **              DECLS		->	CONSTPART VARPART PROCPART                   **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void DECLS(){
  // cout<<"DECLS"<<endl;
  CONSTS();
  VARS();
  return;
}

/****************************************************************************
 ** FUNCTION: SUB_PROGS()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for SUB_PROGS                **
 **              SUB_PROGS	->	SUB_PROGS SUB_DECL | lambda                **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void SUB_PROGS(){
  if(token.type == TokenType::PROCEDURE){
    SUB_DECL();
    SUB_PROGS();
  }
  else{
    return;
  }
  return;
}

/****************************************************************************
 ** FUNCTION: COMP_STAT()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for COMP_STAT                **
 **              COMP_STAT	->	begint  STAT_LIST	endt                   **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void COMP_STAT(){
  match(TokenType::BEGIN);
  STAT_LIST();
  match(TokenType::END);
  return;
}

/****************************************************************************
 ** FUNCTION: STAT_LIST()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for STAT_LIST                **
 **              STAT_LIST	->	lambda                                     **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void STAT_LIST(){
  return;
}

/****************************************************************************
 ** FUNCTION: CONSTS()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for CONSTS                **
 **              CONSTS		->	consts CONST_DECL | lambda                  **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void CONSTS(){
  // cout<<"consts"<<endl;
  if(token.type == TokenType::CONST){
    match(TokenType::CONST);
    CONSTS_DECL();
    CONSTS();
  }
  return;
}

/****************************************************************************
 ** FUNCTION: VARS()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for VARS                **
 **              VARS		-> 	vart VAR_DECL | lambda                   **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void VARS(){
  // cout<<"Vars"<<endl;
  if(token.type == TokenType::VAR){
    match(TokenType::VAR);
    VAR_DECL();
  }
  return;  
}

/****************************************************************************
 ** FUNCTION: DUB_DECL()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for SUB_DECL                **
 **              SUB_DECL	->	proceduret  idt ARGS ; DECLS COMP_STAT ;   **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void SUB_DECL(){
  match(TokenType::PROCEDURE);
  match(TokenType::IDENTIFIER);
  ARGS();
  match(TokenType::SEMICOLON);
  DECLS();
  // cout<<"here"<<endl;
  COMP_STAT();
  match(TokenType::SEMICOLON);
  return;
}

/****************************************************************************
 ** FUNCTION: CONST_DECL()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for CONST_DECL                **
 **              CONST_DECL	-> 	idt relop numt ; CONST_DECL|lambda                   **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void CONSTS_DECL(){
  // cout<<"consts_decl"<<endl;
  if(token.type == TokenType::IDENTIFIER){
    match(TokenType::IDENTIFIER);
    match(TokenType::RELOP);
    // cout<<TokenList[token.type]<<endl;
    match(TokenType::NUMBER);
    match(TokenType::SEMICOLON);
    CONSTS_DECL();
  }
  return;
}

/****************************************************************************
 ** FUNCTION: VAR_DECL()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for COMP_STAT                **
 **              VAR_DECL	-> 	ID_LIST : TYPE ; VAR_DECL | lambda                  **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void VAR_DECL(){
  if(token.type == TokenType::IDENTIFIER){
    ID_LIST();
    match(TokenType::COLON);
    TYPE();
    match(TokenType::SEMICOLON);
    VAR_DECL();
  }
  return;
}

/****************************************************************************
 ** FUNCTION: ARGS()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for ARGS                **
 **              ARGS		->	( ARG_LIST ) | lambda                   **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void ARGS(){
  if(token.type == TokenType::LPAREN){
    match(TokenType::LPAREN);
    ARG_LIST();
    match(TokenType::RPAREN);
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

void ID_LIST(){
  match(TokenType::IDENTIFIER);\
  if(token.type == TokenType::COMMA){
    match(TokenType::COMMA);
    ID_LIST();
  }
  return;
}

/****************************************************************************
 ** FUNCTION: TYPE()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for TYPE                **
 **              TYPE		->	integer | real                   **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void TYPE(){
  if(token.type == TokenType::INTEGER)
  {
    match(TokenType::INTEGER);
  }
  else if(token.type == TokenType::REAL)
  {
    match(TokenType::REAL);
  }
  else
  {
    match(TokenType::CHAR);
  }

  return;
}

/****************************************************************************
 ** FUNCTION: ARG_LIST()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for ARG_LIST                **
 **              ARG_LIST	-> 	MODE ID_LIST : TYPE MORE_ARGS                  **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void ARG_LIST(){
  MODE();
  ID_LIST();
  match(TokenType::COLON);
  TYPE();
  MORE_ARGS();
  return;
}

/****************************************************************************
 ** FUNCTION: MODE()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for MODE                **
 **              MODE		->	vart | lambda                  **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void MODE(){
  if(token.type == TokenType::VAR){
    match(TokenType::VAR);
  }
  return;
}

/****************************************************************************
 ** FUNCTION: MORE_ARGS()                                                  **
 ****************************************************************************
 ** DESCRIPTION: follows the production rules for MORE_ARGS                **
 **              MORE_ARGS	-> 	; ARG_LIST | lambda                  **
 ** INPUT ARGS: None                                                       **
 ** OUTPUT ARGS: None                                                      **
 ** IN/OUT ARGS: None                                                      ** 
 ****************************************************************************/

void MORE_ARGS(){
  if(token.type == TokenType::SEMICOLON){
    match(TokenType::SEMICOLON);
    ARG_LIST();
  }
  return;
}
