#ifndef _PARSER_H_
#define  _PARSER_H_
#include "lexer.h"
#include "emitter.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//type sizes for 64-bit computers
#define SHORT_MAX_LENGTH 5
#define INT_MAX_LENGTH   10
#define LONG_MAX_LENGTH  19

typedef enum _var_type {
  INT=0,
  SHORT=1,
  LONG=2,
  LONGLONG=3,
  DOUBLE=4,
  STRING_TYPE=5
} Data_type;

typedef struct _labels {
  char * text;
  struct _labels * next;
} LabelList;

typedef struct _variables {
  char * text;
  Data_type type;
  struct _variables * next;
} VariableList;

typedef struct _parser {
  Lexer * lexer;
  Emitter * emitter;

  Token * currentToken;
  Token * peekToken;

  VariableList * symbols;
  LabelList * labelsDeclared;
  LabelList * labelsGotoed;
  LabelList * headersInclude;
} Parser;

//int check_token(Parser * parser, Token_type kind);
//int check_peek(Parser * parser, Token_type kind);
//void match(Parser * parser, Token_type kind);

Parser * init_parser(Lexer * lex, Emitter * emitter);
void free_parser(Parser * parser);

LabelList * new_label(char * text);
void free_label_list(LabelList * list);

//LabelList * add_label(LabelList * list, char * text);
//int check_label(LabelList * list, char * text);

//VariableList * new_variable(char * text, Data_type type);
//void free_variable_list(VariableList * list);

//VariableList * add_variable(VariableList * list, char * text, Data_type type);
//int check_variable(VariableList * list, char * text, Data_type type);

//void skip_newlines(Parser * parser);
//void nl(Parser * parser);

//void primary(Parser * parser);
//void unary(Parser * parser);
//void term(Parser * parser);
//void expression(Parser * parser);
//void comparison(Parser * parser);

//void add_variable_type(Parser * parser);

//void statement(Parser * parser);

void program(Parser * parser);
#endif
