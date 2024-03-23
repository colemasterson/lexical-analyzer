//#include "Lexer.h"
#include "SymbolTable.h"

using namespace std;

map<int,int> depthTempMap;

string varTypeList[3] = {"CHARACTER","INTEGER", "REAL"};

int SymbolTable::getOffset(int depth)
{
    int offset = 0;
    for (int i = 0; i < size; i++) 
    {
        STEntry* currentNode = table[i];
        while (currentNode != nullptr) 
        {
            if (currentNode->depth == depth)
            {
                offset += currentNode->variable.size;
            }
            currentNode = currentNode->next;
        }
    }
    return offset;
}

int SymbolTable::varSize(TokenType varType)
{
    switch (varType)
    {
        case TokenType::CHAR:
            return 1;
        case TokenType::INTEGER:
            return 2;
        case TokenType::REAL:
            return 4;
        default:
            cout<<"ERROR with variable size "<<revTokenMap[varType] <<endl;
            return 0;
            break;
    }
    return 0;
}



bool SymbolTable::multipleDec(string lexeme, string symTabLexeme, int depth, int symTabDepth)
{
    if(lexeme == symTabLexeme && depth == symTabDepth)
    {
        cout<<"Multiple declarations of the name: "<<lexeme << " at depth: "<< depth<<endl;
        return true;
    }
    return false;
}

int SymbolTable::hash(string lexeme) 
{
    unsigned long hash = 0;
    unsigned long high;

    for (char c : lexeme) 
    {
        hash = (hash << 4) + c;
        high = hash & 0xF0000000;
        
        if (high != 0)
            hash ^= high >> 24;
        
        hash &= ~high;
    }

    return hash % size;
}


SymbolTable::SymbolTable(int size) 
{
    this->table.resize(size, nullptr);
    this->size = size;
    initRevMap();
}

void SymbolTable::insert(string lexeme, TokenType token, int depth) 
{
    int hash = this->hash(lexeme);
    STEntry * newNode = new STEntry{lexeme, token, depth};

    newNode->variable.type = VariableType::INTEGERT;
    newNode->variable.offset = this->getOffset(depth);
    
    int tempInt = this->varSize(TokenType::INTEGER);
    
    newNode->variable.size = this->varSize(token);
    
    if (table[hash] == nullptr) 
    {
        table[hash] = newNode;
    }
    else {
        STEntry* currentNode = table[hash];
        while (currentNode->next != nullptr) 
        {
            currentNode = currentNode->next;
        }
        currentNode->next = newNode;
    }
}


STEntry* SymbolTable::lookup(string lexeme) 
{
    int hash = this->hash(lexeme);
    STEntry* currentNode = table[hash];

    while (currentNode != nullptr) 
    {
        if (currentNode->lexeme == lexeme) 
        {
            cout << lexeme << " found at depth: " << currentNode->depth << endl;
            return currentNode;
        }
        currentNode = currentNode->next;
    }

    cout << lexeme << " was not found." << endl;
    return nullptr;
}

void SymbolTable::deleteDepth(int depth) 
{
    for (int i = 0; i < size; i++) 
    {
        STEntry* currentNode = table[i];
        STEntry* prevNode = nullptr;
        while (currentNode != nullptr) 
        {
            if (currentNode->depth == depth) 
            {
                if (prevNode == nullptr) 
                    table[i] = currentNode->next;
                else 
                    prevNode->next = currentNode->next;
                
                STEntry* temp = currentNode;
                currentNode = currentNode->next;
                delete temp;
            }
            else 
            {
                prevNode = currentNode;
                currentNode = currentNode->next;
            }
        }
    }

    cout << "Table contents at depth " << depth << " deleted." << endl;
}

void SymbolTable::writeTable(int depth) 
{
    cout << "Contents of table at depth " << depth << ":" << endl;
    cout << setw(20) << "LEXEME" << setw(20) << "TOKEN" << setw(20) << "DEPTH" << setw(20) << "TYPE" << setw(20) << "OFFSET"<< setw(20) << "SIZE" << endl; 

    STEntry* currentNode = nullptr;
    for (int i = 0; i < size; i++) 
    {
        currentNode = nullptr;
        if(table[i] != nullptr)
            currentNode = table[i];
        
        while (currentNode != nullptr) 
        {
            if (currentNode->depth == depth)
                currentNode->print();
            
            currentNode = currentNode->next;
        }
    }
}

SymbolTable::~SymbolTable() 
{
    for (int i = 0; i < size; i++) 
    {   
        STEntry* currentNode = table[i];
        while (currentNode != nullptr) 
        {
            STEntry* temp = currentNode;
            currentNode = currentNode->next;
            delete temp;
        }
    }
}