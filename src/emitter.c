#include "emitter.h"

char * str_concat(const char * s1, const char * s2) {
    int len1;
    int len2;
    if(s1==NULL) {
      len1=0;
    } else {
      len1 = strlen(s1);
    }
    len2 = strlen(s2);
    char * result = malloc(len1 + len2 + 1);
    assert(result);
    for(int i=0; i< len1; ++i) {
      result[i]=s1[i];
    }
    for(int i=len1; i<len1+len2; ++i) {
      result[i]=s2[i-len1];
    }
    result[len1+len2]='\0';
    //if(len1!=0) {
    //  strcpy(result, s1);
    //}
    //strcat(result, s2);
    //memcpy(result, s1, len1);
    //memcpy(result + len1, s2, len2 + 1);
    return result;
}

Emitter * init_emitter(char * path) {
  Emitter * emitter = (Emitter *) malloc(sizeof(Emitter));
  assert(emitter);
  emitter->path=strdup(path);
  emitter->code=NULL;
  emitter->variables=NULL;
  emitter->header=NULL;
  return emitter;
}

void free_emitter(Emitter * emitter) {
  free(emitter->path);
  free(emitter->code);
  free(emitter->variables);
  free(emitter->header);
  free(emitter);
}

void emit_code(Emitter * emitter, char * code) {
  char * tmp = emitter->code;
  emitter->code = str_concat(emitter->code, code);
  free(tmp);
}

void emit_code_line(Emitter * emitter, char * line) {
  char * tmp = emitter->code;
  char * newline = str_concat(line,"\n");
  emitter->code = str_concat(emitter->code, newline);
  free(tmp);
  free(newline);
}

void emit_variable(Emitter * emitter, char * variable) {
  char * tmp = emitter->variables;
  emitter->variables = str_concat(emitter->variables, variable);
  free(tmp);
}

void emit_variable_line(Emitter * emitter, char * variable) {
  char * tmp = emitter->variables;
  char * newline = str_concat(variable,"\n");
  emitter->variables = str_concat(emitter->variables, newline);
  free(tmp);
  free(newline);
}

void emit_header_line(Emitter * emitter, char * header) {
  char * tmp = emitter->header;
  char * newline = str_concat(header,"\n");
  emitter->header = str_concat(emitter->header, newline);
  free(tmp);
  free(newline);
}

void write_file(Emitter * emitter) {
  FILE * file = fopen(emitter->path, "w");
  if(emitter->header) {
    printf("header to file:\n%s", emitter->header);
    fprintf(file, "%s", emitter->header);
  }

  if(emitter->variables) {
    printf("variables to file:\n%s", emitter->variables);
    fprintf(file, "%s", emitter->variables);
  }

  if(emitter->code) {
    printf("code to file:\n%s", emitter->code);
    fprintf(file, "%s", emitter->code);
  }
  fclose(file);
}
