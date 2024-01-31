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
