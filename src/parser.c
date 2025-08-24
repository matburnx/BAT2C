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
  }

  parser->peekToken=get_token(parser->lexer);

  if(tmp1) {
    free_token(tmp1);
  }

  if(tmp2) {
    free_token(tmp2);
  }

}

void match(Parser * parser, Token_type kind) {
  if(!check_token(parser,kind)) {
    printf("\tExpected %s", token_to_str(kind));
    printf(", got %s\n", parser->currentToken->text);
    exit(EXIT_FAILURE);
  }
  next_token(parser);
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

VariableList * new_variable(char * text, Data_type type) {
  VariableList * label=(VariableList *) malloc(sizeof(VariableList));
  assert(label);
  label->text=strdup(text);
  label->type=type;
  label->next=NULL;
  return label;
}

void free_variable_list(VariableList * list) {
  VariableList * temp;
  while(list) {
    temp=list;
    list=list->next;
    free(temp->text);
    free(temp);
  }
}

VariableList * add_variable(VariableList * list, char * text, Data_type type) {
  if(list==NULL) {
    list=new_variable(text, type);
    return list;
  }
  VariableList * temp=list;
  while(temp->next) {
    temp=temp->next;
  }
  temp->next=new_variable(text, type);
  return list;
}

int check_variable(VariableList * list, char * text) {
  while(list && strcmp(list->text,text)!=0) {
    list=list->next;
  }
  if(list==NULL) {
    return 0;
  }
  return 1;
}

VariableList * get_variable(VariableList * list, char * text) {
  VariableList * var=list;
  while(var && strcmp(var->text,text)!=0) {
    var=var->next;
  }
  return var;
}

Parser * init_parser(Lexer * lex, Emitter * emitter) {
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

  free_variable_list(parser->symbols);
  free_label_list(parser->labelsDeclared);
  free_label_list(parser->labelsGotoed);
  free_label_list(parser->headersInclude);

  free(parser);
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
  //printf("PRIMARY: %s\n", parser->currentToken->text);
  if(check_token(parser,NUMBER)){
    emit_code(parser->emitter,parser->currentToken->text);
    next_token(parser);
  } else if(check_token(parser,IDENT)){
    if(!check_variable(parser->symbols,parser->currentToken->text)) {
      printf("\tVariable not declared %s line %d column %d\n", parser->currentToken->text, parser->lexer->currentLine+1, parser->lexer->currentPosition+1);
      exit(EXIT_FAILURE);
    }

    emit_code(parser->emitter,parser->currentToken->text);
    next_token(parser);
  } else {
    printf("\tExpected number or variable at `%s` line %d column %d\n", parser->currentToken->text, parser->lexer->currentLine+1, parser->lexer->currentPosition+1);
    exit(EXIT_FAILURE);
  }
}

void unary(Parser * parser) {
  //printf("UNARY\n");
  if(check_token(parser,MINUS) || check_token(parser,PLUS)){
    emit_code(parser->emitter,parser->currentToken->text);
    next_token(parser);
  }
  primary(parser);
}

void term(Parser * parser) {
  //printf("TERM\n");
  unary(parser);
  while(check_token(parser,ASTERISK) || check_token(parser,SLASH)){
    emit_code(parser->emitter,parser->currentToken->text);
    next_token(parser);
    unary(parser);
  }
}

void expression(Parser * parser) {
  //printf("EXPRESSION\n");
  term(parser);
  while(check_token(parser,MINUS) || check_token(parser,PLUS)){
    emit_code(parser->emitter,parser->currentToken->text);
    next_token(parser);
    term(parser);
  }
}

int is_comparison_operator(Parser * parser) {
  return parser->currentToken->type == GT || parser->currentToken->type == GTEQ || parser->currentToken->type == EQEQ || parser->currentToken->type == NOTEQ || parser->currentToken->type == LT || parser->currentToken->type == LTEQ;
}

void comparison(Parser * parser) {
  //printf("COMPARISON\n");
  expression(parser);
  if(is_comparison_operator(parser)) {
    emit_code(parser->emitter,parser->currentToken->text);
    next_token(parser);
    expression(parser);
  } else {
    printf("\tExpected comparison at %s line %d column %d\n", parser->currentToken->text, parser->lexer->currentLine+1, parser->lexer->currentPosition+1);
    exit(EXIT_FAILURE);
  }
  while(is_comparison_operator(parser)) {
    emit_code(parser->emitter,parser->currentToken->text);
    next_token(parser);
    expression(parser);
  }
}

void add_variable_type(Parser * parser, char * variable_name) {
  int i = strlen(parser->emitter->code)-1;
  int is_string=0;
  int is_double=0;
  int max_length=0;
  int current_length=0;
  Data_type type;
  char c;
  while(i>=0 && parser->emitter->code[i]!='=' && parser->emitter->code[i]!='\n') {
    c=parser->emitter->code[i];
    --i;
    if(c=='"') {
      is_string=1;
      continue;
    } else if(c=='.') {
      is_double=1;
    } else if(is_digit(c)) {
      ++current_length;
      if(current_length>max_length) {
        max_length=current_length;
      }
    } else {
      current_length=0;
    }
  }

  if(is_string) {
    emit_variable(parser->emitter, "char * ");
    type=STRING_TYPE;
  } else if(is_double) {
    emit_variable(parser->emitter, "double ");
    type=DOUBLE;
  } else {
    if(max_length<SHORT_MAX_LENGTH) {
      emit_variable(parser->emitter, "short ");
      type=SHORT;
    } else if(max_length<INT_MAX_LENGTH) {
      emit_variable(parser->emitter, "int ");
      type=INT;
    } else if(max_length<LONG_MAX_LENGTH) {
      emit_variable(parser->emitter, "long ");
      type=LONG;
    } else {
      emit_variable(parser->emitter, "long long ");
      type=LONGLONG;
    }
  }
  parser->symbols = add_variable(parser->symbols,variable_name, type);
}

char * data_format(VariableList * var) {
  if(var==NULL) {
    printf("\tWarning format for NULL variable\n");
    return NULL;
  }
  char * format;
  switch(var->type) {
    case STRING_TYPE:
      format = (char *) malloc(sizeof(char)*3);
      format[0]='%';
      format[1]='s';
      format[2]='\0';
      break;
    case INT:
      format = (char *) malloc(sizeof(char)*3);
      format[0]='%';
      format[1]='d';
      format[2]='\0';
      break;
    case SHORT:
      format = (char *) malloc(sizeof(char)*4);
      format[0]='%';
      format[1]='h';
      format[2]='d';
      format[3]='\0';
      break;
    case LONG:
      format = (char *) malloc(sizeof(char)*4);
      format[0]='%';
      format[1]='l';
      format[2]='d';
      format[3]='\0';
      break;
    case LONGLONG:
      format = (char *) malloc(sizeof(char)*5);
      format[0]='%';
      format[1]='l';
      format[2]='l';
      format[3]='d';
      format[4]='\0';
      break;
    case DOUBLE:
      format = (char *) malloc(sizeof(char)*4);
      format[0]='%';
      format[1]='l';
      format[2]='f';
      format[3]='\0';
      break;
    default:
      format = (char *) malloc(sizeof(char)*3);
      format[0]='%';
      format[1]='d';
      format[2]='\0';
      break;
  }
  return format;
}

void statement(Parser * parser) {
  if(check_token(parser,PRINT)) {
    //printf("STATEMENT: PRINT\n");
    if(!check_label(parser->headersInclude,"stdio")) {
      parser->headersInclude = add_label(parser->headersInclude,"stdio");
      char * tmp = strdup("#include <stdio.h>");
      emit_header_line(parser->emitter,tmp);
      free(tmp);
    }
    next_token(parser);
    if(check_token(parser,STRING)) {
      char * line = strdup("printf(\"");
      emit_code(parser->emitter,line);
      emit_code(parser->emitter,parser->currentToken->text);
      free(line);
      line = strdup("\\n\");");
      emit_code_line(parser->emitter,line);
      
      free(line);

      next_token(parser);
    } else {
      if(!check_variable(parser->symbols,parser->currentToken->text)) {
        //parser->symbols = add_variable(parser->symbols,parser->currentToken->text);
        printf("\tVariable used but not declared %s line %d column %d\n", parser->currentToken->text, parser->lexer->currentLine+1, parser->lexer->currentPosition+1);
        exit(EXIT_FAILURE);
      }

      emit_code(parser->emitter,"printf(\"");
      VariableList * var = get_variable(parser->symbols,parser->currentToken->text);
      if(var==NULL) {
        printf("\tVariable used but not declared %s line %d column %d\n", parser->currentToken->text, parser->lexer->currentLine+1, parser->lexer->currentPosition+1);
        exit(EXIT_FAILURE);
      }

      char * format = data_format(var);
      emit_code(parser->emitter,format);
      free(format);

      emit_code(parser->emitter,"\\n\", ");
      expression(parser);
      emit_code_line(parser->emitter,");");
    }
  } else if(check_token(parser,IF)) {
    //printf("STATEMENT: IF\n");
    next_token(parser);
    emit_code(parser->emitter,"if(");

    comparison(parser);

    match(parser,THEN);
    nl(parser);
    emit_code_line(parser->emitter,"){");

    while(!check_token(parser,ENDIF)) {
      statement(parser);
    }

    match(parser,ENDIF);
    emit_code_line(parser->emitter,"}");
  } else if(check_token(parser,WHILE)) {
    //printf("STATEMENT: WHILE\n");
    next_token(parser);
    emit_code(parser->emitter,"while(");

    comparison(parser);

    match(parser,REPEAT);
    nl(parser);
    emit_code_line(parser->emitter,"){");

    while(!check_token(parser,ENDWHILE)) {
      statement(parser);
    }

    match(parser,ENDWHILE);
    emit_code_line(parser->emitter,"}");
  } else if(check_token(parser,LABEL)) {
    //printf("STATEMENT: LABEL\n");
    next_token(parser);

    if(check_label(parser->labelsDeclared,parser->currentToken->text)) {
      printf("\tLabel `%s` already declared line %d column %d\n", parser->currentToken->text, parser->lexer->currentLine, parser->lexer->currentPosition);
      exit(EXIT_FAILURE);
    }
    parser->labelsDeclared = add_label(parser->labelsDeclared,parser->currentToken->text);

    emit_code(parser->emitter,parser->currentToken->text);
    emit_code_line(parser->emitter,":");

    match(parser,IDENT);
  } else if(check_token(parser,GOTO)) {
    //printf("STATEMENT: GOTO\n");
    next_token(parser);

    parser->labelsGotoed = add_label(parser->labelsGotoed,parser->currentToken->text);

    emit_code(parser->emitter,"goto ");
    emit_code(parser->emitter,parser->currentToken->text);
    emit_code_line(parser->emitter,";");

    match(parser,IDENT);
  } else if(check_token(parser,LET)) {
    //printf("STATEMENT: LET\n");
    next_token(parser);
    int has_type=1;

    if(!check_variable(parser->symbols,parser->currentToken->text)) {
      has_type=0;
    }
    char * var_name = strdup(parser->currentToken->text);
    emit_code(parser->emitter, var_name);
    emit_code(parser->emitter, " = ");

    match(parser,IDENT);
    match(parser,EQ);

    if(!check_token(parser,STRING)) {
      expression(parser);
    } else {
      emit_code(parser->emitter,"\"");
      emit_code(parser->emitter,parser->currentToken->text);
      emit_code(parser->emitter,"\"");
      next_token(parser);
    }
    
    if(has_type==0) {
      add_variable_type(parser, var_name);
      emit_variable(parser->emitter, var_name);
      emit_variable_line(parser->emitter, ";");
    }

    emit_code_line(parser->emitter, ";");

    free(var_name);
  } else if(check_token(parser,INPUT)) {
    //printf("STATEMENT: INPUT\n");
    next_token(parser);
    if(!check_label(parser->headersInclude,"stdio")) {
      parser->headersInclude = add_label(parser->headersInclude,"stdio");
      char * tmp = strdup("#include <stdio.h>");
      emit_header_line(parser->emitter,tmp);
      free(tmp);
    }
    
    if(!check_variable(parser->symbols,parser->currentToken->text)) {
      //parser->symbols = add_variable(parser->symbols,parser->currentToken->text);
      printf("\tVariable used but not declared %s line %d column %d\n", parser->currentToken->text, parser->lexer->currentLine+1, parser->lexer->currentPosition+1);
      exit(EXIT_FAILURE);
    }

    emit_code(parser->emitter,"scanf(\"");
    VariableList * var = get_variable(parser->symbols,parser->currentToken->text);
    if(var==NULL) {
      printf("\tVariable used but not declared %s line %d column %d\n", parser->currentToken->text, parser->lexer->currentLine+1, parser->lexer->currentPosition+1);
      exit(EXIT_FAILURE);
    }
    char * format = data_format(var);
    emit_code(parser->emitter,format);
    free(format);
    if(var->type!=STRING_TYPE) {
      emit_code(parser->emitter,"\", &");
    } else {
      emit_code(parser->emitter,"\", ");
    }
    emit_code(parser->emitter,parser->currentToken->text);
    emit_code_line(parser->emitter,");");

    match(parser,IDENT);
  } else {
    printf("\tInvalid statement at %s (%s)\n", parser->currentToken->text, token_to_str(parser->currentToken->type));
    exit(EXIT_FAILURE);
  }
  nl(parser);
}

void program(Parser * parser) {
  //printf("PROGRAM\n");

  skip_newlines(parser);
  
  while(!check_token(parser, ENDOF)) {
    statement(parser);
  }

  emit_header_line(parser->emitter, "int main(void) {");

  emit_code_line(parser->emitter, "return 0;");
  emit_code_line(parser->emitter, "}");

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
