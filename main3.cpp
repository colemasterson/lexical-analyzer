/*****************************************************************************
 ** NAME: Cole Masterson                                                   	**
 ** CLASS: CSC446 - Compiler Construction                                  	**
 ** ASSIGNMENT: 4                                                          	**
 ** DATE: 1/31/2024                                                        	**
 ** INSTRUCTOR: Dr. George Hamer                                           	**
 *****************************************************************************
 ** DESCRIPTION:                                                           	**
 **   This program serves as the main entry point for the lexical        	**
 **   analyzer and parser of the Oberon-O language. It reads an input file, **
 **   processes its contents through the lexical analyzer, and displays  	**
 **   the lexemes along with their corresponding token types and values. 	**
 **   The program expects the name of the file to analyze as a command   	**
 **   line argument.                                                     	**
 ****************************************************************************/
#include "SymbolTable.h"

int main() {
    // Create a SymbolTable instance
    SymbolTable symTable(10); // Assuming a hash table size of 10 for simplicity

    // Insert some variables at different depths
    symTable.insert("apple", TokenType::INTEGER, 1);
    symTable.insert("orange", TokenType::INTEGER, 1);
    symTable.insert("banana", TokenType::INTEGER, 2);
    symTable.insert("grape", TokenType::INTEGER, 2);

    // Try to look up some variables
    if (symTable.lookup("apple")) {
        cout << "Found 'apple'" << endl;
    } else {
        cout << "'apple' not found" << endl;
    }

    if (symTable.lookup("mango")) {
        cout << "Found 'mango'" << endl;
    } else {
        cout << "'mango' not found" << endl;
    }

    // Write the table for depth 1 and 2 to see the current state
    cout << "Table content at depth 1:" << endl;
    symTable.writeTable(1);

    cout << "\nTable content at depth 2:" << endl;
    symTable.writeTable(2);

    // Delete all variables at depth 1
    symTable.deleteDepth(1);

    // Display the table again to verify deletion
    cout << "\nTable after deleting depth 1:" << endl;
    symTable.writeTableTest(); // Assuming writeTableTest shows all entries regardless of depth

    return 0;
}
