#include "Lexer.h"
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
            {// && currentNode->token == "id") {
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
    int hash = 0;
    for (char c : lexeme) 
    {
        hash = (hash * 31 + c) % size;
    }
    // cout<<"hash for "<<lexeme<<": " << hash<<endl;
    return hash;
}


SymbolTable::SymbolTable(int size) 
{
    this->table.resize(size, nullptr);
    this->size = size;
    // for(int i = 0; i<size; i++){
    //     if(table[i] == nullptr){
    //         cout<<"null pointer when creating at: "<<i<<endl;
    //     }
    // }
}

STEntry* SymbolTable::createTemp(VariableType type,TokenType tokenType, int depth)
{
    string tempName;
    if(depthTempMap[depth])
    {
        tempName = "_t" + (depthTempMap[depth]+1);
        depthTempMap[depth] = ++depthTempMap[depth];
    }
    else
    {
        depthTempMap[depth] = 1;
        tempName = "_t0";
    }
    this->varInsert(tempName, tokenType, depth, type);
    if(this->lookup(tempName) == nullptr)
    {
        cout<<"returning nullptr in createTemp"<<endl;
    }
    // cin.ignore();
    return this->lookup(tempName);
}

void SymbolTable::varInsert(string lexeme, TokenType token, int depth, VariableType type) 
{
    int hash = this->hash(lexeme);
    // TokenType
    // for(int i = 0; i<size; i++){
    //     if(table[i] == nullptr){
    //         cout<<"null pointer when creating at: "<<i<<endl;
    //     }
    // }
    STEntry* newNode = new STEntry(lexeme, token, depth);
    newNode->variable.type = type;
    newNode->variable.offset = this->getOffset(depth);
    
    int tempInt = this->varSize(TokenType::REAL); //EDIT THIS LINE
    
    newNode->variable.size = this->varSize(token);
    
    if (table[hash] == nullptr) 
    {
        // cout<<"this is a null pointer in varinsert"<<endl;
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

void SymbolTable::constInsert(string lexeme, TokenType token, int depth, bool isReal, int intValue, double realValue) 
{
    int hash = this->hash(lexeme);
    cout<<"Const token Type: "<<revTokenMap[token]<<endl;

    STEntry* newNode = new STEntry(lexeme, token, depth);
    newNode->constant.isReal = isReal;

    if (isReal)
        newNode->constant.value.realValue = realValue;
    else 
        newNode->constant.value.intValue = intValue;

    if (table[hash] == nullptr) 
        table[hash] = newNode;
    else 
    {
        STEntry* currentNode = table[hash];
        while (currentNode->next != nullptr) {
            // cout<<"current node lex: " << currentNode->lexeme <<currentNode->depth <<" attempting to insert: "<< lexeme<<endl;
            if(this->multipleDec(lexeme, currentNode->lexeme, depth, currentNode->depth)){
                return;
            }
            currentNode = currentNode->next;
        }
        if(!this->multipleDec(lexeme, currentNode->lexeme, depth, currentNode->depth)){
            currentNode->next = newNode;
        }
    }
}

void SymbolTable::procInsert(string lexeme, TokenType token, int depth, int localVariablesSize, int numParams, vector<VariableType> paramTypes, vector<ParameterPassingMode> paramPassingModes) 
{
    int hash = this->hash(lexeme);
    STEntry* newNode = new STEntry(lexeme, token, depth);

    newNode->procedure.localVariablesSize = localVariablesSize;
    newNode->procedure.numParams = numParams;
    newNode->procedure.paramTypes = paramTypes;
    newNode->procedure.paramPassingModes = paramPassingModes;

    if (table[hash] == nullptr) 
        table[hash] = newNode;
    else 
    {
        STEntry* currentNode = table[hash];

        while (currentNode->next != nullptr)
            currentNode = currentNode->next;

        currentNode->next = newNode;
    }
}

STEntry* SymbolTable::lookup(string lexeme) 
{
    int hash = this->hash(lexeme);
    STEntry* currentNode = table[hash];
    // for(int i = 0; i<size; i++){
    //     if(table[i] == nullptr){
    //         cout<<"null pointer when looking up at: "<<i<<endl;
    //     }
    //     else{
    //         cout<<table[i]->lexeme<<endl;
    //     }
    // }
    while (currentNode != nullptr) 
    {
        if (currentNode->lexeme == lexeme) 
        {
            return currentNode;
        }
        currentNode = currentNode->next;
    }
    // cout<<"returning NULLPTR for "<<lexeme<<endl;
    // cin.ignore();
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
                {
                    table[i] = currentNode->next;
                }
                else 
                {
                    prevNode->next = currentNode->next;
                }
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
}

void SymbolTable::writeTable(int depth) {
    // for(int i = 0; i<size; i++){
    //     if(table[i] == nullptr){
    //         cout<<"null pointer when writing at: "<<i<<endl;
    //     }
    //     if(i==7){
    //         cout<<table[i]->lexeme<<endl;
    //     }
    // }
    cout << "Variables in symbol table at depth " << depth << ":" << endl;
    STEntry* currentNode = nullptr;
    for (int i = 0; i < size; i++) {
        currentNode = nullptr;
        if(table[i] != nullptr){
            currentNode = table[i];
        }
        // cout<<"for loop i:"<<i<<endl;
        while (currentNode != nullptr) {
            // cout<<"while loop i:"<<i<<endl;
            // cout<< currentNode->depth<<endl;
            if (currentNode->depth == depth){// && currentNode->token == "id") {
                //cout << currentNode->lexeme << endl;
                currentNode->print();
            }
            currentNode = currentNode->next;
        }
    }
}
void SymbolTable::writeTableTest() {
    cout << "Variables in symbol table at all depth:" << endl;
    cout<<size<<endl;
    for (int i = 7; i < size; i++) {
        // cout<<"here1"<<endl;
        // cin.ignore();
        STEntry* currentNode = table[i];
        // cout<<"here2"<<endl;
        // cin.ignore();
        if(currentNode == nullptr){//|| currentNode->lexeme == ""){
            cout<<"this node is a nullptr"<<endl;
        }
        cout<<"here3: " << currentNode<<endl;
        //cout<< currentNode->lexeme<<endl; //breaking here
        while (currentNode != nullptr) {
            cout<<"in while loop?"<<currentNode->depth<<endl;
            if(currentNode->token == TokenType::PROCEDURE){
                cout << "LEX: " << currentNode->lexeme<< " DEPTH: "<<currentNode->depth << " TYPE: "<< currentNode->token << " LOCAL VAR SIZE: "<<currentNode->procedure.localVariablesSize <<endl;
            }
            else if(currentNode->token == TokenType::INTEGER || currentNode->token == TokenType::REAL){
                cout << "LEX: " << currentNode->lexeme<< " DEPTH: "<<currentNode->depth << " TYPE: "<< varTypeList[currentNode->variable.type] << " SIZE: "<<currentNode->variable.size << " OFFSET: " << currentNode->variable.offset<<endl;
            }
            else{
                cout<<currentNode->lexeme<<endl;
            }
            currentNode = currentNode->next;
        }
    }
}
SymbolTable::~SymbolTable() {
    for (int i = 0; i < size; i++) {
        STEntry* currentNode = table[i];
        while (currentNode != nullptr) {
            STEntry* temp = currentNode;
            currentNode = currentNode->next;
            delete temp;
        }
    }
}