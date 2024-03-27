/*****************************************************************************
 ** NAME: Cole Masterson                                                   	**
 ** CLASS: CSC446 - Compiler Construction                                  	**
 ** ASSIGNMENT: 5                                                          	**
 ** DATE: 1/31/2024                                                        	**
 ** INSTRUCTOR: Dr. George Hamer                                           	**
 *****************************************************************************
 ** DESCRIPTION:                                                           	**
 **   This program serves as the main entry point for the lexical        	**
 **   analyzer, parser, and symbol table of the Oberon-O language. It reads **
 **   an input file, processes its contents through the lexical analyzer,   **
 **   determines the structure with the parser and places the appropriate   **
 **   record values in the symbol table. Then, the symbol table with print  **
 **   its contents along with their corresponding token types and values. 	**
 **   The program expects the name of the file to analyze as a command   	**
 **   line argument.                                                     	**
 ****************************************************************************/

#include "Lexer.h"
#include "Parser.h"
#include "SymbolTable.h"

using namespace std;

int main(int argc, char* argv[]) 
{
	if(argc !=2)
	{
		cout<<"Usage: `./output <filename>` "<<endl;
		return 1;
	}

	ifstream file(argv[1]);

	if(!file)
		cout<<"Could not open file: "<< argv[1] << endl;
	
	initRevMap();
	string line;

	while(getline(file, line))
		input += line + '\n';

	tChar = input[0];

    cout << setw(20) << "LEXEME" << setw(20) << "TOKENTYPE" << setw(20) << "VALUE" << setw(20) << "VALUER" << setw(20) << "LITERAL" << endl << endl;

	while(true)
	{
		token = GetNextToken();

		if(token.type == TokenType::EOFT)
			break;

        cout << setw(20) << token.lexeme << setw(20) << revTokenMap[token.type] << setw(20) << token.value << setw(20) << token.valueR << setw(20) << token.literal << endl; 
	}

	pos = 0;
	tChar = input[0];
	token = GetNextToken();

	PROG();

	if(token.type == TokenType::EOFT)
	{
		cout<<"COMPLETE"<<endl;
	}
	else
	{
		cout<<"ERROR: parsing complete but EOF token not found"<<endl;
	}

	for(int i = 0; i <= maxDepth; i++)
	{
		symTable.writeTable(i);
	}
	
	return 0;
}