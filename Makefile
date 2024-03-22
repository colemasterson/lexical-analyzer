output: main3.o Lexer.o Parser.o SymbolTable.o
	g++ -o output main3.o Lexer.o Parser.o SymbolTable.o -std=c++11
	
main3.o: main3.cpp
	g++ -c main3.cpp -std=c++11

Lexer.o: Lexer.h Lexer.cpp
	g++ -c Lexer.cpp -std=c++11

Parser.o: Parser.h Parser.cpp
	g++ -c Parser.cpp -std=c++11

SymbolTable.o: SymbolTable.h SymbolTable.cpp
	g++ -c SymbolTable.cpp -std=c++11

clean:
	rm output main3.o Lexer.o Parser.o SymbolTable.o