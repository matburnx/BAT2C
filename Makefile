CC_OPTS=-Wall -Wextra -pedantic
O_NAME=bat2c

ifdef DEBUG
	ifeq ($(DEBUG) ,1)
		DEBUG_OPTS=-g -fsanitize=address
		CC_OPTS+=$(DEBUG_OPTS)
	endif
endif

all: main

main: main.o
	gcc $(CC_OPTS) main.o emitter.o lexer.o parser.o -o $(O_NAME)

lexer.o: src/lexer.c src/lexer.h parser.o emitter.o
	gcc $(CC_OPTS) -c $< -o $@

main.o: src/main.c lexer.o parser.o emitter.o
	gcc $(CC_OPTS) -c $< -o $@

parser.o: src/parser.c src/parser.h
	gcc $(CC_OPTS) -c $< -o $@

emitter.o: src/emitter.c src/emitter.h 
	gcc $(CC_OPTS) -c $< -o $@

clean:
	rm -v *.o main
