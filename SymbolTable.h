#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Lexer.h"
#include "Parser.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

const int TableSize = 211;

enum VariableType { INTEGERT, REALT, CHART };
enum ParameterPassingMode { BY_VALUE, BY_REFERENCE };
enum EntryType {varType, constType, procType};
/* 
struct ParamNode;
typedef ParamNode * ParamPtr;

struct ParamNode 
{
  VariableType paramType;
  ParamPtr next;
}; */

template<typename T> struct ListNode
{
	T data;
	ListNode* next;

	ListNode(T data, ListNode* next = nullptr) : data(data), next(next) {}
};

struct VariableInfo
{
    VariableType type;
    int offset;
    int size;
};

struct ConstantInfo
{
    bool isReal;
    union {
        int intValue;
        float realValue;
    } value;
};

struct ProcedureInfo
{
    int localVariablesSize;
    int numParams;
    ListNode<VariableType>* paramTypes;
    ListNode<ParameterPassingMode>* paramPassingModes;
};


struct STEntry
{
    string lexeme;
    TokenType token;
    int depth;
    STEntry* next;
	EntryType typeofEntry;

    union 
    {
        VariableInfo variable;
        ConstantInfo constant;
        ProcedureInfo procedure;
    };

    void print() const 
    {
		cout << setw(20) << lexeme 
		<< setw(20) << revTokenMap[token] 
		<< setw(20) << depth 
		<< setw(20) << variable.type 
		<< setw(20) << variable.offset
		<< setw(20) << variable.size << endl;

/*         cout<<"Lexeme: "<<lexeme<<endl;
        cout<<"Token: "<<revTokenMap[token]<<endl;
        cout<<"Depth: "<<depth<<endl;

        cout << "Type: " << variable.type << endl;
        cout << "Offset: " << variable.offset << endl;
        cout << "Size: " << variable.size << endl; */
    };

};

class SymbolTable 
{
	private:
		vector<STEntry*> table;
		int size;
		bool multipleDec(string lexeme, string symTabLexeme, int depth, int symTabDepth);
		int hash(string lexeme);
		int varSize(TokenType varType);
		int getOffset(int depth);

	public:
		SymbolTable(int size = TableSize);

		void insert(string lexeme, TokenType token, int depth);
		STEntry* lookup(string lexeme);
		void deleteDepth(int depth);
		void writeTable(int depth);
		void writeTableTest();
		~SymbolTable();
};

#endif