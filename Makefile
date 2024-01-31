output: main.o Lexer.o
	g++ -o output main.o Lexer.o -std=c++11
	
main.o: main.cpp
	g++ -c main.cpp -std=c++11

Lexer.o: Lexer.h Lexer.cpp
	g++ -c Lexer.cpp -std=c++11

clean:
	rm output main.o Lexer.o