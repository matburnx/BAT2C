#ifndef _LEXER_H_
#define _LEXER_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_LINE_LENGTH 256

#define STR_LABEL     "LABEL" 
#define STR_GOTO      "GOTO" 
#define STR_PRINT     "PRINT" 
#define STR_INPUT     "INPUT" 
#define STR_LET       "LET" 
#define STR_IF        "IF" 
#define STR_THEN      "THEN" 
#define STR_ENDIF     "ENDIF" 
#define STR_WHILE     "WHILE" 
#define STR_REPEAT    "REPEAT" 
#define STR_ENDWHILE  "ENDWHILE" 
#define STR_UNDEFINED "UNDEFINED"
#define STR_ENDOF     "ENDOF"
#define STR_NEWLINE   "NEWLINE"
#define STR_NUMBER    "NUMBER"
#define STR_IDENT     "IDENT"
#define STR_STRING    "STRING"
#define STR_EQ        "EQ"
#define STR_PLUS      "PLUS"
#define STR_MINUS     "MINUS"
#define STR_ASTERISK  "ASTERISK"
#define STR_SLASH     "SLASH"
#define STR_EQEQ      "EQEQ"
#define STR_NOTEQ     "NOTEQ"
#define STR_LT        "LT"
#define STR_LTEQ      "LTEQ"
#define STR_GT        "GT"
#define STR_GTEQ      "GTEQ"

typedef enum _token_type {
  ENDOF=1
  ,NEWLINE=2
  ,NUMBER=3
  ,IDENT=4
  ,STRING=5
  ,LABEL=6
  ,GOTO=7
  ,PRINT=8
  ,INPUT=9
  ,LET=10
  ,IF=11
  ,THEN=12
  ,ENDIF=13
  ,WHILE=14
  ,REPEAT=15
  ,ENDWHILE=16
  ,EQ=17
  ,PLUS=18
  ,MINUS=19
  ,ASTERISK=20
  ,SLASH=21
  ,EQEQ=22
  ,NOTEQ=23
  ,LT=24
  ,LTEQ=25
  ,GT=26
  ,GTEQ=27
  ,UNDEFINED=0
} Token_type;

typedef struct _lexer {
  char ** source;
  char currentCharacter;
  int currentPosition;
  int currentLine;
  int length;
} Lexer;

typedef struct _token {
  char * text;
  Token_type type;
} Token;

int countlines(char *filename);
int is_digit(char c);
int is_alpha(char c);
int is_alpha_numeric(char c);
int is_keyword(char * str);
Token_type str_to_token(char * str);
char * token_to_str(Token_type type);
void next_char(Lexer * lex);
Token * init_token();
Token * new_token(char * text, Token_type type);
char peek(Lexer * lex);
void skip_whitespace(Lexer * lex);
void skip_comment(Lexer * lex);
char * set_expression(Lexer* lex, const int text_size);


Lexer * new_lexer(char * file_name);
Token * get_token(Lexer * lex);

void free_lexer(Lexer * lex);
void free_token(Token * tok);
#endif
