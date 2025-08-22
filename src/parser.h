#ifndef _PARSER_H_
#define  _PARSER_H_
#include "lexer.h"
#include "emitter.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct _labels {
  char * text;
  struct _labels * next;
} LabelList;

typedef struct _parser {
  Lexer * lexer;
  Emitter * emitter;

  Token * currentToken;
  Token * peekToken;

  LabelList * symbols;
  LabelList * labelsDeclared;
  LabelList * labelsGotoed;
  LabelList * headersInclude;
} Parser;

int check_token(Parser * parser, Token_type kind);
int check_peek(Parser * parser, Token_type kind);
void match(Parser * parser, Token_type kind);

Parser * init_parser(Lexer * lex, Emitter * emitter);
//Parser * init_parser(Lexer * lex);
void free_parser(Parser * parser);

LabelList * new_label(char * text);
void free_label_list(LabelList * list);
LabelList * add_label(LabelList * list, char * text);
int check_label(LabelList * list, char * text);

void skip_newlines(Parser * parser);
void nl(Parser * parser);
void primary(Parser * parser);
void unary(Parser * parser);
void term(Parser * parser);
void expression(Parser * parser);
void comparison(Parser * parser);
void statement(Parser * parser);
void program(Parser * parser);
#endif
