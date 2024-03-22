#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Lexer.h"
#include "Parser.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

 enum VariableType { INTEGERT, REALT, CHART };
// enum ParameterPassingMode { BY_VALUE, BY_REFERENCE };

struct STEntry
{
    string lexeme;
    TokenType token;
    int depth;
    STEntry* next;

    union {
        struct {
            VariableType type;
            int offset;
            int size;
        } variable;

        struct {
            bool isReal;
            union {
                int intValue;
                double realValue;
            } value;
        } constant;

        struct {
            int localVariablesSize;
            int numParams;
            //vector<VariableType> paramTypes;
            //vector<ParameterPassingMode> paramPassingModes;
        } procedure;
    };

    void print() const 
    {
        cout<<"Lexeme: "<<lexeme<<endl;
        cout<<"Token: "<<revTokenMap[token]<<endl;
        cout<<"Depth: "<<depth<<endl;

        cout << "Type: " << variable.type << endl;
        cout << "Offset: " << variable.offset << endl;
        cout << "Size: " << variable.size << endl;
    };

};

class SymbolTable {
private:
    vector<STEntry*> table;
    int size;
    bool multipleDec(string lexeme, string symTabLexeme, int depth, int symTabDepth);
    int hash(string lexeme);
    int varSize(TokenType varType);
    int getOffset(int depth);

public:
    SymbolTable(int size = 10);
    //Variable insert
    void insert(string lexeme, TokenType token, int depth);

    STEntry* lookup(string lexeme);
    STEntry* createTemp(VariableType type, TokenType tokenType,int depth);
    void deleteDepth(int depth);
    void writeTable(int depth);
    void writeTableTest();
    ~SymbolTable();
};

#endif