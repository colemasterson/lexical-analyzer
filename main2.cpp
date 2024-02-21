#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
#include "Lexer.h"
#include "Parser.h"

using namespace std;

int main(int argc, char* argv[]) 
{
	if(argc !=2)
	{
		cout<<"Usage: source <filename>"<<endl;
		return 1;
	}

	ifstream file(argv[1]);

	if(!file.is_open())
		cout<<"Coule not open file: "<< argv[1]<<endl;
	
	string line;
	while(getline(file, line))
	{
		input += line + '\n';
	}

	tChar = input[0];
	while(true)
	{
		token = GetNextToken();
		if(token.type == TokenType::EOFT)
			break;

		cout << "Token Type: " << revTokenMap[token.type] << ", Lexeme: " << token.lexeme << ", Value: "<< token.value << ", ValueR: "<<token.valueR<<", Literal: "<<token.literal << endl;
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
	
	return 0;
}