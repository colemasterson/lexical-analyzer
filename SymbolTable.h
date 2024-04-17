#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Lexer.h"
#include <vector>

using namespace std;

const int TableSize = 223;

enum VariableType { INTEGERT, REALT, CHART };
enum ParameterPassingMode { BY_VALUE, BY_REFERENCE };
enum EntryType {varType, constType, procType};

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
		<< setw(20) << variable.size 
        << setw(20) << typeofEntry << endl;

    };

};

class SymbolTable 
{
	private:
		vector<STEntry*> table;
		int size;
		int hash(string lexeme);
		int varSize(TokenType varType);
		int getOffset(int depth);
        bool multipleDec(string lexeme, string symTabLexeme, int depth, int symTabDepth);
	public:
		SymbolTable(int size = TableSize);
		void insertVar(string lexeme, TokenType token, int depth, VariableType type);
        void insertConst(string lexeme, TokenType token, int depth, bool isReal, int intValue, float realValue);
        void insertProc(string lexeme, TokenType token, int depth, int localVariablesSize, int numParams, ListNode<VariableType>* paramTypes, ListNode<ParameterPassingMode>* paramPassingModes);

        int getParamListSize(ListNode<ParameterPassingMode>* elements);
        int getVarListSize(ListNode<VariableType>* elements);

		STEntry* lookup(string lexeme);
		void deleteDepth(int depth);
		void writeTable(int depth);
		~SymbolTable();
};

#endif