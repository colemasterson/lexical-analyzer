#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Lexer.h"
#include "STEntry.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// enum VariableType { INTEGER, REAL, BOOLEAN };
// enum ParameterPassingMode { BY_VALUE, BY_REFERENCE };

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
    void varInsert(string lexeme, TokenType token, int depth, VariableType type);
    //const insert
    void constInsert(string lexeme, TokenType token, int depth, bool isReal, int intValue, double realValue);
    //procedure insert
    void procInsert(string lexeme, TokenType token, int depth, int localVariablesSize, int numParams, vector<VariableType> paramTypes, vector<ParameterPassingMode> paramPassingModes);
    STEntry* lookup(string lexeme);
    STEntry* createTemp(VariableType type, TokenType tokenType,int depth);
    void deleteDepth(int depth);
    void writeTable(int depth);
    void writeTableTest();
    ~SymbolTable();
};

#endif