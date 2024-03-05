#include "STEntry.h"

STEntry::STEntry(string lexeme, TokenType token, int depth) {
    // cout<<"lex in sym table entry" <<lexeme<<endl;
    this->lexeme = lexeme;
    this->token = token;
    this->depth = depth;
    this->next = nullptr;
}

void STEntry::print() const {
    cout<<"Lexeme: "<<lexeme<<endl;
    cout<<"Token: "<<revTokenMap[token]<<endl;
    cout<<"Depth: "<<depth<<endl;
    switch (token) {
        case TokenType::IDENTIFIER:
            cout << "Type: " << variable.type << endl;
            cout << "Offset: " << variable.offset << endl;
            cout << "Size: " << variable.size << endl;
            break;
        case TokenType::INTEGER:
            cout << "Value: " << constant.value.intValue << endl;
            break;
        case TokenType::REAL:
            cout << "Value: " << constant.value.realValue << endl;
            break;
        case TokenType::PROCEDURE:
            cout << "Local Variables Size: " << procedure.localVariablesSize << endl;
            cout << "Number of Params: " << procedure.numParams << endl;
            cout << "Param Types: ";
            for (auto type : procedure.paramTypes) {
                cout << type << " ";
            }
            cout << endl;
            cout << "Param Passing Modes: ";
            for (auto mode : procedure.paramPassingModes) {
                cout << mode << " ";
            }
            cout << endl;
            break;
        default:
            break;
    }

}

STEntry::~STEntry() {}