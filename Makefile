GCC_OPTS=-Wall -Wextra -pedantic -g
DEBUG_OPTS=-fsanitize=address
all: main

main: main.o
	gcc $(GCC_OPTS) main.o emitter.o lexer.o parser.o -o main

lexer.o: src/lexer.c src/lexer.h parser.o emitter.o
	gcc $(GCC_OPTS) -c $< -o $@

main.o: src/main.c lexer.o parser.o emitter.o
	gcc $(GCC_OPTS) -c $< -o $@

parser.o: src/parser.c src/parser.h
	gcc $(GCC_OPTS) -c $< -o $@

emitter.o: src/emitter.c src/emitter.h 
	gcc $(GCC_OPTS) -c $< -o $@

clean:
	rm -v *.o main
