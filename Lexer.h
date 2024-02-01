/****************************************************************************
 ** NAME: Cole Masterson                                                   **
 ** CLASS: CSC446 - Compiler Construction                                  **
 ** ASSIGNMENT: 1                                                          **
 ** DATE: 1/31/2024                                                        **
 ** INSTRUCTOR: Dr. George Hamer                                           **
 **************************************************************************** 
 ** DESCRIPTION:  Header file for the lexical analyzer of the Oberon-O     **
 ** language. This file defines the token types, the Token structure,      **
 ** global variables, and function prototypes necessary for lexical        **
 ** analysis. It includes the necessary standard libraries and sets up     **
 ** the structure for the lexer to identify various token types like       **
 ** identifiers, numbers, strings, operators, and reserved keywords.       **
*****************************************************************************/
#ifndef LEXER_H
#define LEXER_H

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <map>

using namespace std;

enum TokenType {
    // Reserved token types
    MODULE, PROCEDURE, VAR, BEGIN, END, IF, THEN, ELSE, ELSIF, WHILE, DO,
    ARRAY, RECORD, CONST, TYPE, INTEGER, REAL,

    // Symbol token types
    COMMA, PERIOD, SEMICOLON, LPAREN, RPAREN, COLON,

    //Broken symbols
    LCURL, RCURL, 
    LSQUARE, RSQUARE, 
    APOST, 
    QUOTE, 
    TILDE,

    // Other token types
    IDENTIFIER, NUMBER, STRING,
    RELOP, ADDOP, MULOP, ASSIGNOP,

    //Alphanum operation types
    OR, DIV, MOD,

    // Error Token for testing
    ERRORT,
    EOFT
};

struct Token
{
  TokenType type;
  string lexeme;
  int value;
  float valueR;
  string literal;
};

//Globals
extern int pos;
extern char tChar;
extern string input;
extern Token token;

extern map<string, TokenType> tokenMap;
extern map<TokenType, string> revTokenMap;

//Functions
extern Token GetNextToken();
extern void initRevMap();
void advance();
void skip_whitespace();
void comment();
char peek();

Token identifier();
Token literal();
Token number();
Token relop();
Token assignop();
Token real(string temp);

#endif
