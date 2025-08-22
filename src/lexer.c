#include "lexer.h"

int countlines(char *filename)
{
  FILE *fp = fopen(filename,"r");
  int ch=0;
  int lines=0;

  if (fp == NULL) return 0;

  ++lines;
  while ( (ch = fgetc(fp)) != EOF)
  {
    if (ch == '\n') {
      ++lines;
    }
  }
  fclose(fp);
  return lines-1;
}

int is_digit(char c) {
  if(c >= '0' && c <='9') {
    return 1;
  }
  return 0;
}

// alpha: letter of a keyword
int is_alpha(char c) {
  if((c >= 'A' && c <= 'Z') || (c>='a' && c <= 'z') ) {
    return 1;
  }
  return 0;
}

int is_alpha_numeric(char c) {
  if(is_alpha(c) || is_digit(c)) {
    return 1;
  }
  return 0;
}

int is_keyword(char * str) {
  if(strcmp(str,STR_IF)==0) {
    return 1;
  } else if(strcmp(str,STR_GOTO)==0) {
    return 1;
  } else if(strcmp(str,STR_PRINT)==0) {
    return 1;
  } else if(strcmp(str,STR_INPUT)==0) {
    return 1;
  } else if(strcmp(str,STR_LET)==0) {
    return 1;
  } else if(strcmp(str,STR_THEN)==0) {
    return 1;
  } else if(strcmp(str,STR_ENDIF)==0) {
    return 1;
  } else if(strcmp(str,STR_WHILE)==0) {
    return 1;
  } else if(strcmp(str,STR_REPEAT)==0) {
    return 1;
  } else if(strcmp(str,STR_ENDWHILE)==0) {
    return 1;
  } else if(strcmp(str,STR_LABEL)==0) {
    return 1;
  }
  return 0;
}

Token_type str_to_token(char * str) {
  if(strcmp(str,STR_IF)==0) {
    return IF;
  } else if(strcmp(str,STR_GOTO)==0) {
    return GOTO;
  } else if(strcmp(str,STR_PRINT)==0) {
    return PRINT;
  } else if(strcmp(str,STR_INPUT)==0) {
    return INPUT;
  } else if(strcmp(str,STR_LET)==0) {
    return LET;
  } else if(strcmp(str,STR_THEN)==0) {
    return THEN;
  } else if(strcmp(str,STR_ENDIF)==0) {
    return ENDIF;
  } else if(strcmp(str,STR_WHILE)==0) {
    return WHILE;
  } else if(strcmp(str,STR_REPEAT)==0) {
    return REPEAT;
  } else if(strcmp(str,STR_ENDWHILE)==0) {
    return ENDWHILE;
  } else if(strcmp(str,STR_LABEL)==0) {
    return LABEL;
  } else if(strcmp(str,STR_ENDOF)==0) {
    return ENDOF;
  } else if(strcmp(str,STR_NEWLINE)==0) {
    return NEWLINE;
  } else if(strcmp(str,STR_NUMBER)==0) {
    return NUMBER;
  } else if(strcmp(str,STR_IDENT)==0) {
    return IDENT;
  } else if(strcmp(str,STR_STRING)==0) {
    return STRING;
  } else if(strcmp(str,STR_EQ)==0) {
    return EQ;
  } else if(strcmp(str,STR_PLUS)==0) {
    return PLUS;
  } else if(strcmp(str,STR_MINUS)==0) {
    return MINUS;
  } else if(strcmp(str,STR_ASTERISK)==0) {
    return ASTERISK;
  } else if(strcmp(str,STR_SLASH)==0) {
    return SLASH;
  } else if(strcmp(str,STR_EQEQ)==0) {
    return EQEQ;
  } else if(strcmp(str,STR_NOTEQ)==0) {
    return NOTEQ;
  } else if(strcmp(str,STR_LT)==0) {
    return LT;
  } else if(strcmp(str,STR_LTEQ)==0) {
    return LTEQ;
  } else if(strcmp(str,STR_GT)==0) {
    return GT;
  } else if(strcmp(str,STR_GTEQ)==0) {
    return GTEQ;
  }
  return UNDEFINED;
}

char * token_to_str(Token_type type) {
  char * str=NULL;
  if(IF==type) {
    str=strdup(STR_IF);
  } else if(GOTO==type) {
    str=strdup(STR_GOTO);
  } else if(PRINT==type) {
    str=strdup(STR_PRINT);
  } else if(INPUT==type) {
    str=strdup(STR_INPUT);
  } else if(LET==type) {
    str=strdup(STR_LET);
  } else if(THEN==type) {
    str=strdup(STR_THEN);
  } else if(ENDIF==type) {
    str=strdup(STR_ENDIF);
  } else if(WHILE==type) {
    str=strdup(STR_WHILE);
  } else if(REPEAT==type) {
    str=strdup(STR_REPEAT);
  } else if(ENDWHILE==type) {
    str=strdup(STR_ENDWHILE);
  } else if(LABEL==type) {
    str=strdup(STR_LABEL);
  } else if(WHILE==type) {
    str=strdup(STR_WHILE);
  } else if(REPEAT==type) {
    str=strdup(STR_REPEAT);
  } else if(ENDOF==type) {
    str=strdup(STR_ENDOF);
  } else if(NEWLINE==type) {
    str=strdup(STR_NEWLINE);
  } else if(NUMBER==type) {
    str=strdup(STR_NUMBER);
  } else if(IDENT==type) {
    str=strdup(STR_IDENT);
  } else if(STRING==type) {
    str=strdup(STR_STRING);
  } else if(EQ==type) {
    str=strdup(STR_EQ);
  } else if(PLUS==type) {
    str=strdup(STR_MINUS);
  } else if(ASTERISK==type) {
    str=strdup(STR_ASTERISK);
  } else if(SLASH==type) {
    str=strdup(STR_SLASH);
  } else if(EQEQ==type) {
    str=strdup(STR_EQEQ);
  } else if(NOTEQ==type) {
    str=strdup(STR_NOTEQ);
  } else if(NOTEQ==type) {
    str=strdup(STR_LT);
  } else if(LT==type) {
    str=strdup(STR_LTEQ);
  } else if(GT==type) {
    str=strdup(STR_GTEQ);
  } else {
    str=strdup(STR_UNDEFINED);
  }
  return str;
}

void next_char(Lexer * lex) {
  if(lex->currentLine > lex->length-1) return;

  ++(lex->currentPosition);
  if(lex->currentPosition > (int) strlen(lex->source[lex->currentLine])-1) {
    lex->currentPosition=0;
    ++(lex->currentLine);
    if(lex->currentLine > lex->length-1) {
      lex->currentCharacter='\0';
      return;
    }
  }

  lex->currentCharacter=lex->source[lex->currentLine][lex->currentPosition];
}

Lexer * new_lexer(char * file_name) {
  int nb_of_lines = countlines(file_name);

  Lexer * lex = (Lexer *) malloc(sizeof(Lexer));
  assert(lex);
  lex->source = (char **) malloc(nb_of_lines *sizeof(char *));
  assert(lex->source);
  lex->currentCharacter='\0';
  lex->currentPosition=-1;
  lex->currentLine=0;
  lex->length=nb_of_lines;

  FILE * file = fopen(file_name,"r");
  if(file==NULL) {
    printf("Couldn't open file %s\n", file_name);
    exit(EXIT_FAILURE);
  }
  int i=0;
  char buffer[MAX_LINE_LENGTH];
  while(i<nb_of_lines && fgets(buffer,MAX_LINE_LENGTH,file)) {
    lex->source[i]=strdup(buffer);
    ++i;
  }
  
  fclose(file);
  next_char(lex);
  return lex;
}

void free_lexer(Lexer * lex) {
  for(int i=0; i<lex->length; ++i) {
    free(lex->source[i]);
  }
  free(lex->source);
  free(lex);
}

Token * init_token() {
  Token * tok = (Token *) malloc(sizeof(Token));
  assert(tok);
  tok->text=NULL;
  tok->type=UNDEFINED;
  return tok;
}

Token * new_token(char * text, Token_type type) {
  Token * tok = init_token();
  tok->text=strdup(text);
  if(type>=0 && type <= 27) {
    tok->type=type;
  } else {
    printf("Unexpected type %d\n", type);
    exit(EXIT_FAILURE);
  }
  return tok;
}

void free_token(Token * tok) {
  free(tok->text);
  free(tok);
}

char peek(Lexer * lex) {
  if(lex->currentLine > lex->length-1) return '\0';

  int new_pos = lex->currentPosition+1;
  int new_line = lex->currentLine;
  if(new_pos > (int) strlen(lex->source[lex->currentLine])-1) {
    new_pos=0;
    ++new_line;
    if(new_line > lex->length-1) {
      return '\0';
    }
  }

  return lex->source[new_line][new_pos];
}

void skip_whitespace(Lexer * lex) {
  while(lex->currentCharacter==' ' || lex->currentCharacter=='\r' || lex->currentCharacter=='\t') {
    next_char(lex);
  }
}

void skip_comment(Lexer * lex) {
  if(lex->currentCharacter=='#') {
    while(lex->currentCharacter!='\n' && lex->currentCharacter!='\0') {
      next_char(lex);
    }
  }
}

char * set_expression(Lexer* lex, const int text_size) {
  char * expr = (char *) malloc( sizeof(char) * (text_size+1) );
  assert(expr);

  for(int i=0; i<text_size; ++i) {
    expr[i]=lex->currentCharacter;
    next_char(lex);
  }
  expr[text_size]='\0';
  return expr;
}

Token * get_token(Lexer * lex) {
  Token * tok=NULL;
  skip_whitespace(lex);
  skip_comment(lex);

  char * expr=NULL;
  int start_pos, length;
  if(lex->currentCharacter == '+') {
    expr=set_expression(lex,1);
    tok=new_token(expr, PLUS);
  } else if(lex->currentCharacter == '-') {
    expr=set_expression(lex,1);
    tok=new_token(expr, MINUS);
  } else if(lex->currentCharacter == '*') {
    expr=set_expression(lex,1);
    tok=new_token(expr, ASTERISK);
  } else if(lex->currentCharacter == '/') {
    expr=set_expression(lex,1);
    tok=new_token(expr, SLASH);
  } else if(lex->currentCharacter == '=') {
    if (peek(lex) == '=') {
      expr=set_expression(lex,2);
      tok=new_token(expr, EQEQ);
    } else {
      expr=set_expression(lex,1);
      tok=new_token(expr, EQ);
    }
  } else if(lex->currentCharacter == '<') {
    if (peek(lex) == '=') {
      expr=set_expression(lex,2);
      tok=new_token(expr, LTEQ);
    } else {
      expr=set_expression(lex,1);
      tok=new_token(expr, LT);
    }
  } else if(lex->currentCharacter == '>') {
    if (peek(lex) == '=') {
      expr=set_expression(lex,2);
      tok=new_token(expr, GTEQ);
    } else {
      expr=set_expression(lex,1);
      tok=new_token(expr, GT);
    }
  } else if(lex->currentCharacter == '!') {
    if (peek(lex) == '=') {
      expr=set_expression(lex,2);
      tok=new_token(expr, NOTEQ);
    } else {
      printf("Expected `!=` got `!` at line %d column %d\n", lex->currentLine+1, lex->currentPosition+1);
      exit(EXIT_FAILURE);
    }
  } else if(lex->currentCharacter == '"') {
    next_char(lex);
    start_pos = lex->currentPosition;
    while(lex->currentCharacter!='"') {
      if(lex->currentCharacter=='\n' || lex->currentCharacter=='\0') {
        printf("Illegal character `%c` in string at line %d column %d\n", lex->currentCharacter, lex->currentLine+1, lex->currentPosition+1);
        exit(EXIT_FAILURE);
      }
      next_char(lex);
    }
    length = lex->currentPosition - start_pos; //currentPosition is 1 further since currentCharacter='"' so you end up with 1 - 1 = 0
    // Reset currentPosition of the lexer and call next_char to reset currentCharacter
    // TODO
    lex->currentPosition=start_pos-1;
    next_char(lex);
    expr=set_expression(lex,length);
    tok=new_token(expr,STRING);
    next_char(lex); // Skip the ending `"` character
  } else if(is_digit(lex->currentCharacter)) {
    start_pos = lex->currentPosition;
    while(is_digit(peek(lex))) {
      next_char(lex);
    }
    if(peek(lex) == '.') {
      next_char(lex);
    
      if(!is_digit(peek(lex))) {
        printf("Illegal character `%c` in number at line %d column %d\n", lex->currentCharacter, lex->currentLine+1, lex->currentPosition+1);
        exit(EXIT_FAILURE);
      }
      while(is_digit(peek(lex))) {
        next_char(lex);
      }
    }
    length = 1 + lex->currentPosition - start_pos;
    // Reset currentPosition of the lexer and call next_char to reset currentCharacter
    // TODO
    lex->currentPosition=start_pos-1;
    next_char(lex);
    expr=set_expression(lex,length);
    tok=new_token(expr, NUMBER);
  } else if(is_alpha(lex->currentCharacter)) {
    start_pos = lex->currentPosition;
    while(is_alpha_numeric(peek(lex))) {
      next_char(lex);
    }

    length = 1 + lex->currentPosition - start_pos;
    // Reset currentPosition of the lexer and call next_char to reset currentCharacter
    // TODO
    lex->currentPosition=start_pos-1;
    next_char(lex);
    expr=set_expression(lex,length);
    if(!is_keyword(expr)) {
      tok=new_token(expr, IDENT);
    } else {
      tok=new_token(expr, str_to_token(expr));
    }
  } else if(lex->currentCharacter == '\n') {
    expr=set_expression(lex,1);
    tok=new_token(expr, NEWLINE);
  } else if(lex->currentCharacter == '\0') {
    expr=set_expression(lex,0);
    assert(expr);
    next_char(lex);
    tok=new_token(expr, ENDOF);
  } else {
    printf("Unknown symbol `%c` at line %d column %d\n", lex->currentCharacter, lex->currentLine+1, lex->currentPosition+1);
    exit(EXIT_FAILURE);
  }
  //next_char(lex);
//  printf("\tBEFORE RETURN`%s` %d\n", tok->text, tok->type);
  free(expr); 
//  printf("AFTER FREE(expr)\n");
  return tok;
}
