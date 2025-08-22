#ifndef _EMITTER_H_
#define _EMITTER_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct _emitter {
  char * path;
  char * header;
  char * code;
} Emitter;

char * str_concat(const char * s1, const char * s2);
Emitter * init_emitter(char * path);
void free_emitter(Emitter * emitter);
void emit(Emitter * emitter, char * code);
void emit_line(Emitter * emitter, char * line);
void header_line(Emitter * emitter, char * header);
void write_file(Emitter * emitter);
#endif
