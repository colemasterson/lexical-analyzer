/*
  TODO:
    1. Change up comments 
    2. Testing
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
#include "Lexer.h"

using namespace std;

//Globals
int pos = 0;
char tChar;
string input;
Token token;

map<string, TokenType> tokenMap = {
    {"MODULE", MODULE},
    {"PROCEDURE", PROCEDURE},
    {"VAR", VAR},
    {"BEGIN", BEGIN},
    {"END", END},
    {"IF", IF},
    {"THEN", THEN},
    {"ELSE", ELSE},
    {"ELSIF", ELSIF},
    {"WHILE", WHILE},
    {"DO", DO},
    {"ARRAY", ARRAY},
    {"RECORD", RECORD},
    {"CONST", CONST},
    {"TYPE", TYPE},
    {"INTEGER", INTEGER},
    {"REAL", REAL},

    {"COMMA", COMMA},
    {"PERIOD", PERIOD},
    {"SEMICOLON", SEMICOLON},
    {"LPAREN", LPAREN},
    {"RPAREN", RPAREN},
    {"COLON", COLON},

    // Broken 
    {"LCURL", LCURL},
    {"RCURL", RCURL},
    {"LSQUARE", LSQUARE},
    {"RSQUARE", RSQUARE},
    {"APOST", APOST},
    {"QUOTE", QUOTE},
    {"TILDE", TILDE},

    {"IDENTIFIER", IDENTIFIER},
    {"NUMBER", NUMBER},
    {"STRING", STRING},
    {"RELOP", RELOP},
    {"ADDOP", ADDOP},
    {"MULOP", MULOP},
    {"ASSIGNOP", ASSIGNOP},

    {"OR", OR},
    {"DIV", DIV},
    {"MOD", MOD},


    {"ERRORT", ERRORT},
    {"EOFT", EOFT},

};

map<TokenType, string> revTokenMap;

extern void initRevMap()
{
    // initialize the reverse token map for easier token display
    for (const auto &pair : tokenMap) 
        revTokenMap[pair.second] = pair.first;
}
  
extern Token GetNextToken() 
{
    while (tChar != '\0') {
        if (isspace(tChar)) {//Check if the current character is a space then skip over
        skip_whitespace();
        continue;
        }

        if (isalpha(tChar)) { //Check if the current character is in the alphabet
        return identifier();
        }

        if (isdigit(tChar)) { //check if the current character is a digit
        return number();
        }

        switch (tChar) { //Specific cases for allowed symbols
        case '+':
            advance();
            return Token{TokenType::ADDOP, "+"};
        case ',':
            advance();
            return Token{TokenType::COMMA, ","};
        case '.':
            advance();
            return Token{TokenType::PERIOD, "."};
        case '-':
            advance();
            return Token{TokenType::ADDOP, "-"};
        case '*':
            advance();
            return Token{TokenType::MULOP, "*"};
        case '/':
            advance();
            return Token{TokenType::MULOP, "/"};
        case '&':
            advance();
            return Token{TokenType::MULOP, "&"};
        case ';':
            advance();
            return Token{TokenType::SEMICOLON, ";"};

        case '(':
            if (peek() == '*') 
            {  // Check for comment start sequence '(*'
                comment();
                continue;         
            } 
            else 
            {
                advance();
                return Token{TokenType::LPAREN, "("};
            }

        case ')':
            advance();
            return Token{TokenType::RPAREN, ")"};
        case ':':
            return assignop();
        case '{':
            advance();
            return Token{TokenType::LCURL, "{"};
        case '}':
            advance();
            return Token{TokenType::RCURL, "}"};
        case '[':
            advance();
            return Token{TokenType::LSQUARE, "["};
        case ']':
            advance();
            return Token{TokenType::RSQUARE, "]"};
        case '"':
            return literal();
        case '~':
            advance();
            return Token{TokenType::TILDE, "~"};

        case '\'':
            return literal();
        case '<':
        case '>':
        case '=':
            return relop();
        default:
            advance();
            return Token{TokenType::ERRORT, ""};
        }
    }
    return Token{TokenType::EOFT, ""};
}

void advance() {
  pos++;
  if (pos > input.length() - 1) {
    tChar = '\0';
  } 
  else {
    tChar = input[pos];
  }
  return;
}

void skip_whitespace() {
  while (tChar != '\0' && isspace(tChar)) {
    advance();
  }
  return;
}

void comment() {
    if (tChar == '(' && peek() == '*') {  // Check for opening of comment
        advance();  // Consume '*'
        while (true) {
            advance();
            if (tChar == '\0') {  // End of file reached
                return;
            }
            if (tChar == '*' && peek() == ')') {  // Check for closing of comment
                advance();  // Consume ')'
                advance();  // Move past the closing ')'
                break;
            }
        }
    }
}


char peek() {
    // Check if we're at the end of the input
    if (pos + 1 >= input.length()) {
        return '\0'; // Return null character if there are no more characters
    }
    return input[pos + 1]; // Return the next character without advancing pos
}

Token literal() {
    string result;
    char quoteType = tChar; // Save the quote type (' or ")
    advance();

    while (tChar != quoteType) { // Continue until the matching quote is found
        if (tChar == '\0' || tChar == '\n') {
            cout << "Unterminated literal" << endl;
            return Token{TokenType::ERRORT, result}; // Use ERRORT for error tokens
        }
        result += tChar;
        advance();
    }
    advance(); // Skip the closing quote

    if (quoteType == '\'') {
        // Handle character literal
        return Token{TokenType::STRING, result, 0, 0.0, result};
    } else {
        // Handle string literal
        return Token{TokenType::STRING, result, 0, 0.0, result};
    }
}

Token relop()
{
    string result;
    result += tChar;
    advance();

    if(tChar == '=' || tChar == '>' || tChar == '<')
    {
        result += tChar;
        advance();
        return Token{TokenType::RELOP, result};
    }
    else
        return Token{TokenType::RELOP, result};
}

Token assignop()
{
    string result;
    result += tChar;
    advance();
    if(tChar == '=')
    {
        result += tChar;
        advance();
        return Token{TokenType::ASSIGNOP, result};
    }
    else 
    {
        return Token{TokenType::COLON, result};
    }
}

Token identifier() 
{
    string result;

    while (tChar != '\0' && isalnum(tChar)) 
    {
        result += tChar;
        advance();
    }

    if(result.length() > 17)
        result = result.substr(0,17);

    //add case to catch OR, DIV, MOD and assign as addop/mulop
    if(tokenMap[result] == TokenType::OR)  
        return Token{TokenType::ADDOP, result};

    if(tokenMap[result] == TokenType::DIV || tokenMap[result] == TokenType::MOD)
        return Token{TokenType::MULOP, result}; 

    if (tokenMap.count(result))
        return Token{tokenMap[result], result};
    else
      return Token{TokenType::IDENTIFIER, result};
}

Token real(string temp)
{
    advance();
    while (tChar != '\0' && isdigit(tChar)) 
    {
        temp += tChar;
        advance();
    }

    return Token{TokenType::REAL, temp, 0, stof(temp)};
}

Token number() 
{
    string result;
    while (tChar != '\0' && isdigit(tChar)) 
    {
        result += tChar;
        advance();
    }

    if(tChar == '.')
    {
        result += tChar;
        return real(result);
    }

    return Token{TokenType::INTEGER, result,stoi(result)};
}