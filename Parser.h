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
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>

using namespace std;

void match(TokenType desired);
void PROG();
void DECLS();
void SUB_PROGS();
void COMP_STAT();
void STAT_LIST();
void CONSTS();
void VARS();
void SUB_DECL();
void CONSTS_DECL();
void VAR_DECL();
void ARGS();
void ID_LIST();
void TYPE();
void ARG_LIST();
void MODE();
void MORE_ARGS();
#endif