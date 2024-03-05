output: main2.o Lexer.o Parser.o STEntry.o SymbolTable.o
	g++ -o output main2.o Lexer.o Parser.o  STEntry.o SymbolTable.o -std=c++11
	
main2.o: main2.cpp
	g++ -c main2.cpp -std=c++11

Lexer.o: Lexer.h Lexer.cpp
	g++ -c Lexer.cpp -std=c++11

Parser.o: Parser.h Parser.cpp
	g++ -c Parser.cpp -std=c++11

STEntry.o: STEntry.h STEntry.cpp
	g++ -c STEntry.cpp -std=c++11

SymbolTable.o: SymbolTable.h SymbolTable.cpp
	g++ -c SymbolTable.cpp -std=c++11

clean:
	rm output main2.o Lexer.o Parser.o STEntry.o SymbolTable.o