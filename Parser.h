/****************************************************************************
 ** NAME: Cole Masterson                                                   **
 ** CLASS: CSC 446                                                         **
 ** ASSIGNMENT: 3                                                          **
 ** DUE DATE: 2/21/23                                                      **
 ** INSTRUCTOR: Dr. Hamer                                                  **
 **************************************************************************** 
 ** DESCRIPTION:  Parser for the Oberon-O language that determines if the  **
 **               program contains legal syntax.                           **
 ****************************************************************************/

#ifndef PARSER_H
#define PARSER_H
#include "Lexer.h"
#include "SymbolTable.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stack>
#include <list>
#include <cctype>

using namespace std;

extern SymbolTable symTable;
extern stack<Token> tokenStack;
extern int maxDepth;

void match(TokenType desired);
void PROG();        //
void DECLPART();    //
void CONSTPART();   //
void CONSTTAIL();   //
void VARPART();     //
void VARTAIL();     //
void ID_LIST();     //
void TYPEMARK();    //
void VALUE();       //
void PROCPART();    //
void PROCDECL();    //
void PROCHEAD();    //
void PROCBODY();    //
void ARGS();        //
void ARG_LIST();    //
void MORE_ARGS();   //
void MODE();        //
void STATEPART();   //
// Assignment 6 Addons
void SEQ_STATE();
void STATEMENT();
void STATTAIL();
void ASSIGNSTAT();
void IOSTAT();
void EXPR();
void RELATION();
void SIMPLEEXPR();
void MORETERM();
void TERM();
void MOREFACTOR();
void FACTOR();
void ADDOPF();
void MULOPF();
void SIGNOPF();

#endif