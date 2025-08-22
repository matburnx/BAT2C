#include <stdlib.h>
#include <stdio.h>
#include "emitter.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[]) {
  Lexer * lex;
  if(argc==1) {
    lex = new_lexer("tests/fibo.bas"); //tests/hello.bas
  } else { // TODO
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

  Emitter * emitter = init_emitter("hello.c");
  Parser * parser = init_parser(lex, emitter);
//  Parser * parser = init_parser(lex);

  program(parser);
  write_file(parser->emitter);
/*
  Token * tok = get_token(lex);
  printf("%s\n", tok->text);

  while(tok->type != ENDOF) {
    free_token(tok);
    tok = get_token(lex);
    if(*(tok->text)=='\n') {
      printf("NEWLINE\n");
    } else if(*(tok->text)=='\0') {
      printf("END OF FILE\n");
    } else {
      printf("%s\n", tok->text);
    }
  }
  free_token(tok);
*/
  free_parser(parser);

  //free_lexer(lex);
  return 0;
}
