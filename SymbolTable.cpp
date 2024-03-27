#include "SymbolTable.h"

using namespace std;

map<int,int> depthTempMap;

string varTypeList[3] = {"CHARACTER","INTEGER", "REAL"};

SymbolTable::SymbolTable(int size) 
{
    this->table.resize(size, nullptr);
    this->size = size;
    initRevMap();
}

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

bool SymbolTable::multipleDec(string lexeme, string symTabLexeme, int depth, int symTabDepth)
{
    if(lexeme == symTabLexeme && depth == symTabDepth)
    {
        cout<<"Multiple declarations of: \""<< lexeme <<"\" at depth: "<< depth<<endl;
        return true;
    }

    return false;
}

void SymbolTable::insertVar(string lexeme, TokenType token, int depth, VariableType type) 
{
    int hash = this->hash(lexeme);

    STEntry * newNode = new STEntry{lexeme, token, depth};
    newNode->typeofEntry = varType;

    newNode->variable.type = type;
    newNode->variable.offset = this->getOffset(depth);    
    newNode->variable.size = this->varSize(token);
    
    if (table[hash] == nullptr) 
    {
        table[hash] = newNode;
    }
    else 
    {
        STEntry* currentNode = table[hash];
        while (currentNode->next != nullptr) 
        {
            currentNode = currentNode->next;
        }
        currentNode->next = newNode;
    }
}


int SymbolTable::getParamListSize(ListNode<ParameterPassingMode>* head)
{
    int count = 0;
    ListNode<ParameterPassingMode>* current = head;
    while (current != nullptr) 
    {
        count++;
        current = current->next;
    }
    return count;
}

int SymbolTable::getVarListSize(ListNode<VariableType>* head)
{
    int count = 0;
    ListNode<VariableType>* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}


void SymbolTable::insertConst(string lexeme, TokenType token, int depth, bool isReal, int intValue, float realValue)
{
    // Create a hash value and STEntry node pointer
    int hashValue = hash(lexeme);
    cout << "Const token type: " << revTokenMap[token] << endl;

    STEntry * newNode = new STEntry{lexeme, token, depth};
    newNode->typeofEntry = constType;
    newNode->constant.isReal = isReal;

    // set int or float value type
    if(isReal)
        newNode->constant.value.realValue = realValue;
    else
        newNode->constant.value.intValue = intValue;

    // insert const into the table
    if (table[hashValue] == nullptr) 
    {
        table[hashValue] = newNode;
    }
    else
    {
        // if there's a collision, iterate to end of list and insert
        cout << "Collision for const entry: "<< lexeme << endl;
        STEntry* current = table[hashValue];

        while (current->next != nullptr)
        {
            // check for multiple declarations of the constant
            if(multipleDec(lexeme, current->lexeme, depth, current->depth))
                return;
            current = current->next;
        }

        current->next = newNode;
    }
}


void SymbolTable::insertProc(string lexeme, TokenType token, int depth, int localVariablesSize, int numParams, ListNode<VariableType>* paramTypes, ListNode<ParameterPassingMode>* paramPassingModes) 
{
    // Create a hash value and STEntry node pointer
    int hashValue = hash(lexeme);

    STEntry * newNode = new STEntry{lexeme, token, depth};
    newNode->typeofEntry = procType;
    
    // set the procedure specific values
    newNode->procedure.localVariablesSize = localVariablesSize;
    newNode->procedure.numParams = numParams;
    newNode->procedure.paramTypes = paramTypes; // Directly assign the linked list for parameter types.
    newNode->procedure.paramPassingModes = paramPassingModes; // Directly assign the linked list for parameter passing modes.
    
    // insert entry into symbol table
    if (table[hashValue] == nullptr) 
    {
        table[hashValue] = newNode;
    } 
    else 
    {
        // if there's a collision, iterate to end of list and insert
        cout << "Collision for procedure entry: "<< lexeme << endl;
        STEntry* current = table[hashValue];
        while (current->next != nullptr) 
        {
            current = current->next;
        }
        current->next = newNode;
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

/*SymbolTable::~SymbolTable() 
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
}*/
SymbolTable::~SymbolTable() 
{
    for (int i = 0; i < size; i++) 
    {   
        STEntry* currentNode = table[i];
        while (currentNode != nullptr) 
        {
            // check if entry is a procedure type and delete the linked lists
            if (currentNode->typeofEntry == procType) 
            {
                // delete the paramTypes list
                ListNode<VariableType>* currentParamType = currentNode->procedure.paramTypes;
                while (currentParamType != nullptr)
                {
                    ListNode<VariableType>* tempType = currentParamType;
                    currentParamType = currentParamType->next;
                    delete tempType;
                }

                // delete the paramPassingModes list
                ListNode<ParameterPassingMode>* currentParamMode = currentNode->procedure.paramPassingModes;
                while (currentParamMode != nullptr)
                {
                    ListNode<ParameterPassingMode>* tempMode = currentParamMode;
                    currentParamMode = currentParamMode->next;
                    delete tempMode;
                }
            }

            // delete the entry
            STEntry* temp = currentNode;
            currentNode = currentNode->next;
            delete temp;
        }
    }
}
