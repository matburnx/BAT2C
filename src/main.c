#include <stdlib.h>
#include <stdio.h>
#include "emitter.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[]) {
  Lexer * lex;
  if(argc==1) {
    printf("No file were given!\n");
    printf("BAT2C: BASIC Auto Transpiler To C\n");
    printf("Usage: ./build.sh <file.bas>\n");
  } else {
    lex = new_lexer(argv[1]);
  }

/*
  Token * tok = get_token(lex);
  char * symbol=token_to_str(tok->type);
  printf("`%s` %s\n", tok->text, symbol);
  free(symbol);

  while(tok->type != ENDOF) {
    free_token(tok);
    tok = get_token(lex);
    symbol=token_to_str(tok->type);
    printf("`%s` %s\n", tok->text, symbol);
    free(symbol);
  }
  free_token(tok);
*/

  Emitter * emitter = init_emitter("out.c");
  Parser * parser = init_parser(lex, emitter);

  program(parser);
  write_file(parser->emitter);

  free_parser(parser);

  return 0;
}
