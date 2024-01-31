# Lexical Analyzer

This project creates a lexical analyzer for use with a subset of the Oberon-O programming langauges.
It defines functions to parse and tokenize source code, with the ability to store both real and integer values,
as well as literals.

## Installation

Simply clone the repository to a location of your choosing.
Make sure you have text files of oberon-o source code that you can use to test the lexer.

## Usage

After installation, run the 'make' command to build the project.

```
$ make
```

After building the project, type './output <file.txt>', including the file you would like to perform lexical analysis on.
```
$ ./output test.txt
```

## Cleanup

To remove the .o files from the project directory, run the 'make clean' command to remove them.

```
$ make clean
```
