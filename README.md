# BASIC Auto Transpiled To C (BAT2C)

Very simple program to transpile BASIC code to C and then compile generated C code.

## Dependencies
* gcc
* make
* bash (optional)

## Usage
With bash:
```
make

./build.sh <file.bas>

./file
```

Without bash:
```
make

./bat2c <file.bas>

gcc out.c -o file

./file
```

Roadmap:
- [x] ~Compile automatically the generated C code~
- [x] ~Finish transpiling for every (initial) keyword~
- [ ] Implementation of strong regression tests
- [ ] Parentheses for expressions
- [ ] Logical operators
- [ ] ELSE IF and ELSE
- [ ] FOR loop
- [ ] More primitive types (strings, boolean)
- [ ] Functions
- [ ] Lazy writing (i.e., 1. instead of 1.0, x++ instead of x=x+1...)
- [ ] Number literals written in binary, hex, and octal
- [ ] Abstract syntax tree representation
- [ ] Allow multiple files
- [ ] Arrays
- [ ] *Better compiler errors*
- [ ] *Lexical scope (see scope)*
- [ ] *Record types (i.e., structs or tuples)*
- [ ] *Type checking (see type systems)*
- [ ] *Compiler optimizations (e.g., constant folding)*

