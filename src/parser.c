#include "parser.h"
int check_token(Parser * parser, Token_type kind) {
  return parser->currentToken->type == kind;
}

int check_peek(Parser * parser, Token_type kind) {
  return parser->peekToken->type == kind;
}

void next_token(Parser * parser) {
  Token * tmp1, * tmp2;
  tmp1=parser->currentToken;
  tmp2=parser->peekToken;

  if(parser->peekToken!=NULL) {
    parser->currentToken=new_token(parser->peekToken->text,parser->peekToken->type);
//    free_token(parser->peekToken);
  }

  parser->peekToken=get_token(parser->lexer);

  if(tmp1) {
    free_token(tmp1);
  }

  if(tmp2) {
    free_token(tmp2);
  }

//  if(parser->peekToken) {
//    printf("NEXT TOKEN\n");
//    if(parser->peekToken->text[0]=='\n') {
//      printf("\\n %d\n", parser->peekToken->type);
//    } else if(parser->peekToken->text[0]=='\0') {
//      printf("\\0 %d\n", parser->peekToken->type);
//    } else {
//      printf("%s %d\n", parser->peekToken->text, parser->peekToken->type);
//    }
//  }
}

void match(Parser * parser, Token_type kind) {
  if(!check_token(parser,kind)) {
    printf("\tExpected %s", token_to_str(kind));
    printf(", got %s\n", parser->currentToken->text);
    exit(EXIT_FAILURE);
  }
  next_token(parser);
}

Parser * init_parser(Lexer * lex, Emitter * emitter) {
//Parser * init_parser(Lexer * lex) {
  Parser * parser = (Parser *) malloc(sizeof(Parser));
  assert(parser);
  
  parser->lexer=lex;
  parser->emitter=emitter;

  parser->currentToken=NULL;
  parser->peekToken=NULL;

  parser->symbols=NULL;
  parser->labelsDeclared=NULL;
  parser->labelsGotoed=NULL;
  parser->headersInclude=NULL;
  
  next_token(parser);
  next_token(parser);
  
  return parser;
}

void free_parser(Parser * parser) {
  free_lexer(parser->lexer);
  free_token(parser->currentToken);
  free_token(parser->peekToken);

  free_emitter(parser->emitter);

  free_label_list(parser->symbols);
  free_label_list(parser->labelsDeclared);
  free_label_list(parser->labelsGotoed);
  free_label_list(parser->headersInclude);

  free(parser);
}

LabelList * new_label(char * text) {
  LabelList * label=(LabelList *) malloc(sizeof(LabelList));
  assert(label);
  label->text=strdup(text);
  label->next=NULL;
  return label;
}

void free_label_list(LabelList * list) {
  LabelList * temp;
  while(list) {
    temp=list;
    list=list->next;
    free(temp->text);
    free(temp);
  }
}

LabelList * add_label(LabelList * list, char * text) {
  if(list==NULL) {
    list=new_label(text);
    return list;
  }
  LabelList * temp=list;
  while(temp->next) {
    temp=temp->next;
  }
  temp->next=new_label(text);
  return list;
}

int check_label(LabelList * list, char * text) {
  while(list && strcmp(list->text,text)!=0) {
    list=list->next;
  }
  if(list==NULL) {
    return 0;
  }
  return 1;
}

void skip_newlines(Parser * parser) {
  while(check_token(parser,NEWLINE)) {
    next_token(parser);
  }
}

void nl(Parser * parser) {
  match(parser,NEWLINE);
  skip_newlines(parser);
}

void primary(Parser * parser) {
  printf("PRIMARY: %s\n", parser->currentToken->text);
  if(check_token(parser,NUMBER)){
    next_token(parser);
  } else if(check_token(parser,IDENT)){
    if(!check_label(parser->symbols,parser->currentToken->text)) {
      printf("\tVariable not declared %s line %d column %d\n", parser->currentToken->text, parser->lexer->currentLine+1, parser->lexer->currentPosition+1);
      exit(EXIT_FAILURE);
    }

    next_token(parser);
  } else {
    printf("\tExpected number or variable at `%s` line %d column %d\n", parser->currentToken->text, parser->lexer->currentLine+1, parser->lexer->currentPosition+1);
    exit(EXIT_FAILURE);
  }
}

void unary(Parser * parser) {
  printf("UNARY\n");
  if(check_token(parser,MINUS) || check_token(parser,PLUS)){
    next_token(parser);
  }
  primary(parser);
}

void term(Parser * parser) {
  printf("TERM\n");
  unary(parser);
  while(check_token(parser,ASTERISK) || check_token(parser,SLASH)){
    next_token(parser);
    unary(parser);
  }
}

void expression(Parser * parser) {
  printf("EXPRESSION\n");
  term(parser);
  while(check_token(parser,MINUS) || check_token(parser,PLUS)){
    next_token(parser);
    term(parser);
  }
}

int is_comparison_operator(Parser * parser) {
  return parser->currentToken->type == GT || parser->currentToken->type == GTEQ || parser->currentToken->type == EQEQ || parser->currentToken->type == NOTEQ || parser->currentToken->type == LT || parser->currentToken->type == LTEQ;
}

void comparison(Parser * parser) {
  printf("COMPARISON\n");
  expression(parser);
  if(is_comparison_operator(parser)) {
    next_token(parser);
    expression(parser);
  } else {
    printf("\tExpected comparison at %s line %d column %d\n", parser->currentToken->text, parser->lexer->currentLine+1, parser->lexer->currentPosition+1);
    exit(EXIT_FAILURE);
  }
  while(is_comparison_operator(parser)) {
    next_token(parser);
    expression(parser);
  }
}

void statement(Parser * parser) {
  if(check_token(parser,PRINT)) {
    printf("STATEMENT: PRINT\n");
    if(!check_label(parser->headersInclude,"stdio")) {
      parser->headersInclude = add_label(parser->headersInclude,"stdio");
      char * tmp = strdup("#include <stdio.h>");
      header_line(parser->emitter,tmp);
      free(tmp);
    }
    next_token(parser);
    if(check_token(parser,STRING)) {
      char * line = strdup("printf(\"");
      emit(parser->emitter,line);
      emit(parser->emitter,parser->currentToken->text);
      free(line);
      line = strdup("\\n\");");
      emit_line(parser->emitter,line);
      //int length = snprintf(line, MAX_LINE_LENGTH,"printf(\"%s\\n\");", parser->currentToken->text) + 1;
      //free(line);
      //line = (char *) malloc(sizeof(char)*(length+1));
      //assert(line);
      //snprintf(line,length,"printf(\"%s\\n\");", parser->currentToken->text);
      //emit_line(parser->emitter,line);
      
      free(line); //Maybe not needed

      next_token(parser);
    } else {
      /*
      if(is_float(parser->currentToken->text)) {
       //emit(parser->emitter,"printf(\"%f\\n\"), ");
      } else {
       //emit(parser->emitter,"printf(\"%d\\n\"), ");
      }
      */
     emit(parser->emitter,"printf(\"%f\\n\", ");
     expression(parser);
     emit_line(parser->emitter,");");
    }
  } else if(check_token(parser,IF)) {
    printf("STATEMENT: IF\n");
    next_token(parser);
    emit(parser->emitter,"if(");

    comparison(parser);

    match(parser,THEN);
    nl(parser);
    emit_line(parser->emitter,"){");

    while(!check_token(parser,ENDIF)) {
      statement(parser);
    }

    match(parser,ENDIF);
    emit_line(parser->emitter,"}");
  } else if(check_token(parser,WHILE)) {
    printf("STATEMENT: WHILE\n");
    next_token(parser);
    emit(parser->emitter,"while(");

    comparison(parser);

    match(parser,REPEAT);
    nl(parser);
    emit_line(parser->emitter,"){");

    while(!check_token(parser,ENDWHILE)) {
      statement(parser);
    }

    match(parser,ENDWHILE);
    emit_line(parser->emitter,"}");
  } else if(check_token(parser,LABEL)) {
    printf("STATEMENT: LABEL\n");
    next_token(parser);

    if(check_label(parser->labelsDeclared,parser->currentToken->text)) {
      printf("\tLabel `%s` already declared line %d column %d\n", parser->currentToken->text, parser->lexer->currentLine, parser->lexer->currentPosition);
      exit(EXIT_FAILURE);
    }
    parser->labelsDeclared = add_label(parser->labelsDeclared,parser->currentToken->text);

    emit(parser->emitter,parser->currentToken->text);
    emit_line(parser->emitter,":");

    match(parser,IDENT);
  } else if(check_token(parser,GOTO)) {
    printf("STATEMENT: GOTO\n");
    next_token(parser);

    parser->labelsGotoed = add_label(parser->labelsGotoed,parser->currentToken->text);

    emit(parser->emitter,"goto ");
    emit(parser->emitter,parser->currentToken->text);
    emit_line(parser->emitter,";");

    match(parser,IDENT);
  } else if(check_token(parser,LET)) {
    printf("STATEMENT: LET\n");
    next_token(parser);

    if(!check_label(parser->symbols,parser->currentToken->text)) {
      parser->symbols = add_label(parser->symbols,parser->currentToken->text);
    }

    match(parser,IDENT);
    match(parser,EQ);

    expression(parser);
  } else if(check_token(parser,INPUT)) {
    printf("STATEMENT: INPUT\n");
    next_token(parser);
    
    if(!check_label(parser->symbols,parser->currentToken->text)) {
      parser->symbols = add_label(parser->symbols,parser->currentToken->text);
    }
    
    match(parser,IDENT);
  } else {
    printf("\tInvalid statement at %s (%s)\n", parser->currentToken->text, token_to_str(parser->currentToken->type));
    exit(EXIT_FAILURE);
  }
  nl(parser);
}

void program(Parser * parser) {
  printf("PROGRAM\n");

  emit_line(parser->emitter, "int main(void) {");

  skip_newlines(parser);
  
  while(!check_token(parser, ENDOF)) {
    statement(parser);
  }

  emit_line(parser->emitter, "return 0;");
  emit_line(parser->emitter, "}");

  LabelList * label = parser->labelsGotoed;
  while(label) {
    if(!check_label(parser->labelsDeclared,label->text)) {
      printf("\tAttempt to GOTO to undeclared label `%s`\n", label->text);
      exit(EXIT_FAILURE);
    }
    label=label->next;
  }

  label = parser->labelsDeclared;
  while(label) {
    if(!check_label(parser->labelsGotoed,label->text)) {
      printf("\tWARNING: Unused label `%s`\n", label->text);
    }
    label=label->next;
  }
}
