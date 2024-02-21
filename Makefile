output: main2.o Lexer.o
	g++ -o output main2.o Lexer.o -std=c++11
	
main2.o: main2.cpp
	g++ -c main2.cpp -std=c++11

Lexer.o: Lexer.h Lexer.cpp
	g++ -c Lexer.cpp -std=c++11

clean:
	rm output main2.o Lexer.o