#include <vector>
#include <string>
#include "Lexer.h"

using namespace std;

enum VariableType {INTEGERT, REALT, BOOLEANT};

enum ParameterPassingMode {BY_VALUE,BY_REFERENCE};

class STEntry {
public:
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
            vector<VariableType> paramTypes;
            vector<ParameterPassingMode> paramPassingModes;
        } procedure;
    };

    STEntry(string lexeme, TokenType token, int depth);
    void print() const;
    ~STEntry();
};