/****************************************************************************
 ** NAME: Cole Masterson                                                   **
 ** CLASS: CSC446 - Compiler Construction                                  **
 ** ASSIGNMENT: 1                                                          **
 ** DATE: 1/31/2024                                                        **
 ** INSTRUCTOR: Dr. George Hamer                                           **
 ****************************************************************************
 ** DESCRIPTION:                                                           **
 **     This program serves as the main entry point for the lexical        **
 **     analyzer of the Oberon-O language. It reads an input file,         **
 **     processes its contents through the lexical analyzer, and displays  **
 **     the lexemes along with their corresponding token types and values. **
 **     The program expects the name of the file to analyze as a command   **
 **     line argument.                                                     **
 ****************************************************************************/

#include "Lexer.h"

using namespace std;


int main(int argc, char* argv[]) 
{
    if(argc !=2)
    {
        cout<<"Usage: source <filename>"<<endl;
        return 1;
    }

    ifstream inFile(argv[1]);

    if(!inFile)
        cout<<"Could not open file: "<< argv[1]<<endl;

    initRevMap();
    string tLine;

    while(getline(inFile, tLine))
        input += tLine + '\n';

    tChar = input[0];

    cout << setw(20) << "LEXEME" 
    << setw(20) << "TOKENTYPE" 
    << setw(20) << "VALUE" 
    << setw(20) << "VALUER" 
    << setw(20) << "LITERAL" << endl << endl;

    while(true)
    {
        token = GetNextToken();

        if(token.type == TokenType::EOFT)
            break;
        
        cout << setw(20) << token.lexeme
        << setw(20) << revTokenMap[token.type]
        << setw(20) << token.value 
        << setw(20) << token.valueR 
        << setw(20) << token.literal << endl; 

    }

    pos = 0;
    tChar = input[0];
    token = GetNextToken();
    return 0;
}