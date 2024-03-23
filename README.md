# Part 1 - Lexical Analyzer

This project creates a lexical analyzer for use with a subset of the Oberon-O programming langauge.
It defines functions to parse and tokenize source code, with the ability to store both real and integer values,
as well as literals.

# Part 2 - Recursive Descent Parser
The second part of this project involves the integration of a parser using recursive descent for the Oberon-O CFG into the Lexer. This give the program the ability to detect invalid syntax based on the expecations of the grammar.

# Part 3 - Symbol Table with Hashing
The third part of this project involves the construction of a symbol table that will hold the variables, constants, and procedures
for functions written in source. The hashing function is a direct implementation of the PJW hashing function to reduce collions.

## Installation

Simply clone the repository to a location of your choosing.
Make sure you have text files of Oberon-O source code that you can use to test the lexer.

## Usage

After installation, run the `make` command to build the project.

```
$ make
```

After building the project, type `./output` to see the symbol table in action with a predefined driver program that tests the functionality.
```
$ ./output test.txt
```

## Cleanup

To remove the object files from the project directory, run the `make clean` command to remove them.

```
$ make clean
```
