/****************************************************************************
 ** NAME: Cole Masterson                                                   **
 ** CLASS: CSC446 - Compiler Construction                                  **
 ** ASSIGNMENT: 1                                                          **
 ** DATE: 1/31/2024                                                        **
 ** INSTRUCTOR: Dr. George Hamer                                           **
 ****************************************************************************  
 ** DESCRIPTION:  This file implements the lexical analyzer for the        **
 ** Oberon-O language, as defined in Lexer.h. It includes the core         **
 ** functionalities for analyzing an input string and breaking it down     **
 ** into tokens. The implementation covers various aspects such as         **
 ** handling whitespace, identifying different types of tokens (like       **
 ** identifiers, numbers, operators), and managing comments and errors.    **
 ** The file initializes global variables and token maps, and defines      **
 ** necessary functions for the lexical analysis process.                  **
 ****************************************************************************/

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

//Doesn't really need a comment block, just creates a reverse tokenMap with a for loop
extern void initRevMap()
{
    for (const auto &pair : tokenMap) 
        revTokenMap[pair.second] = pair.first;
}

/****************************************************************************
 * FUNCTION: GetNextToken                                                   *
 * DESCRIPTION:                                                             *
 *     Scans the input string and identifies the next token in the sequence.*
 * INPUTS:                                                                  *
 *     - None                                                               *
 * OUTPUTS:                                                                 *
 *     - Token                                                              *
 ****************************************************************************/
extern Token GetNextToken() 
{
    while (tChar != '\0') 
    {
        if (isspace(tChar)) 
        {
            skip_whitespace();
            continue;
        }

        if (isalpha(tChar))
            return identifier();

        if (isdigit(tChar)) 
            return number();
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

/****************************************************************************
 * FUNCTION: advance                                                        *
 * DESCRIPTION:                                                             *
 *     Advances the current position in the input string and updates the    *
 *     current character being analyzed (tChar).                            *
 * INPUTS:                                                                  *
 *     - None                                                               *
 * OUTPUTS:                                                                 *
 *     - Updates global variable char tChar                                 *
 ****************************************************************************/

void advance() 
{
    pos++;

    if(pos > input.length() - 1) 
        tChar = '\0';        
    else
        tChar = input[pos];

    return;
}

/****************************************************************************
 * FUNCTION: skip_whitespace                                                *
 * DESCRIPTION:                                                             *
 *     Skips over any whitespace characters in the input string.            *
 * INPUTS:                                                                  *
 *     None                                                                 *
 * OUTPUTS:                                                                 *
 *     - Advances the global variable 'pos' to the next non-whitespace      *
 *       character in 'input' and updates 'tChar' accordingly.              *
 ****************************************************************************/

void skip_whitespace() 
{
    while (tChar != '\0' && isspace(tChar))
        advance();
    return;
}

/****************************************************************************
 * FUNCTION: comment                                                        *
 * DESCRIPTION:                                                             *
 *     Detects and skips over comments in the input string. Comments are    *
 *     expected to start with '(*' and end with '*)'.                       *
 * INPUTS:                                                                  *
 *     None                                                                 *
 * OUTPUTS:                                                                 *
 *     - Advances the global variable 'pos' until the end of the comment or *
 *       end of the file is reached. Updates 'tChar' accordingly.           *
 ****************************************************************************/

void comment() 
{
    if (tChar == '(' && peek() == '*') 
    {
        advance();
        while (true) 
        {
            advance();

            if (tChar == '\0') 
                return;

            if (tChar == '*' && peek() == ')') 
            {
                advance();  // Consume ')'
                advance();  // Move past the closing ')'
                break;
            }
        }
    }
}

/****************************************************************************
 * FUNCTION: peek                                                           *
 * DESCRIPTION:                                                             *
 *     Returns the character following the current position in the input    *
 *     string without advancing the position.                               *
 * INPUTS:                                                                  *
 *     None                                                                 *
 * OUTPUTS:                                                                 *
 *     - Returns a char: the next character in 'input', or '\0' if the end  *
 *       of the input is reached.                                           *
 ****************************************************************************/
char peek() 
{
    // Check if we're at the end of the input
    if (pos + 1 >= input.length()) 
        return '\0';
    
    return input[pos + 1];
}

/****************************************************************************
 * FUNCTION: literal                                                        *
 * DESCRIPTION:                                                             *
 *     Processes string or character literals in the input, handling        *
 *     unterminated literals as errors.                                     *
 * INPUTS:                                                                  *
 *     None                                                                 *
 * OUTPUTS:                                                                 *
 *     - Returns a Token of type STRING with the extracted literal. In case *
 *       of an unterminated literal, returns an error token (ERRORT).       *
 ****************************************************************************/

Token literal() 
{
    string result;
    char quoteType = tChar; // Save the quote type (' or ")
    advance();

    while (tChar != quoteType) 
    { // Continue until the matching quote is found
        if (tChar == '\0' || tChar == '\n') 
        {
            cout << "Unterminated literal" << endl;
            return Token{TokenType::ERRORT, result}; // Use ERRORT for error tokens
        }
        result += tChar;
        advance();
    }
    advance(); // Skip the closing quote

    if (quoteType == '\'') 
        return Token{TokenType::STRING, result, 0, 0.0, result}; 
    else 
        return Token{TokenType::STRING, result, 0, 0.0, result};
}

/****************************************************************************
 * FUNCTION: relop                                                          *
 * DESCRIPTION:                                                             *
 *     Identifies and processes relational operators in the input string.   *
 * INPUTS:                                                                  *
 *     None                                                                 *
 * OUTPUTS:                                                                 *
 *     - Returns a Token of type RELOP with the relational operator         *
 *       identified in the input string.                                    *
 ****************************************************************************/

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

/****************************************************************************
 * FUNCTION: assignop                                                       *
 * DESCRIPTION:                                                             *
 *     Processes the assignment operator (:=) or a standalone colon in the  *
 *     input string.                                                        *
 * INPUTS:                                                                  *
 *     None                                                                 *
 * OUTPUTS:                                                                 *
 *     - Returns a Token of type ASSIGNOP for the assignment operator or    *
 *       COLON for a standalone colon.                                      *
 ****************************************************************************/

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

/****************************************************************************
 * FUNCTION: identifier                                                     *
 * DESCRIPTION:                                                             *
 *     Processes alphanumeric sequences in the input string as identifiers. *
 *     Identifiers are truncated to a maximum of 17 characters. Special    *
 *     cases like OR, DIV, and MOD are treated as operators.                *
 * INPUTS:                                                                  *
 *     None                                                                 *
 * OUTPUTS:                                                                 *
 *     - Returns a Token of type IDENTIFIER, ADDOP, or MULOP, depending on  *
 *       the identifier or operator identified in the input string.         *
 ****************************************************************************/

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

/****************************************************************************
 * FUNCTION: real                                                           *
 * DESCRIPTION:                                                             *
 *     Processes the fractional part of a real number, building upon a      *
 *     previously collected integer part.                                   *
 * INPUTS:                                                                  *
 *     - string temp: The integer part of the real number already collected *
 * OUTPUTS:                                                                 *
 *     - Returns a Token of type REAL with the complete real number.        *
 ****************************************************************************/

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

/****************************************************************************
 * FUNCTION: number                                                         *
 * DESCRIPTION:                                                             *
 *     Identifies and processes numeric literals in the input string. If a  *
 *     decimal point is encountered, it hands off processing to the real    *
 *     function to handle real numbers. Otherwise, it treats the sequence   *
 *     as an integer.                                                       *
 * INPUTS:                                                                  *
 *     None                                                                 *
 * OUTPUTS:                                                                 *
 *     - Returns a Token of type INTEGER for integers or delegates to real  *
 *       function for real numbers.                                         *
 ****************************************************************************/

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