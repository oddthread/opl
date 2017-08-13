/*This file was automatically generated.*/
#ifndef d_system_h
#define d_system_h

#include "oul/src/h/oul.h"
char *get_base_path();
void str_to_file(char *path,char *cstr);
char *alloc_file_to_str(char *path);
void sleep_milli(u32 milliseconds);
s64 milli_current_time();
void set_clipboard_text(char *c);
void system_free(void *p);
char *get_clipboard_text();
void mem_free(void *p);
void *mem_realloc(void *ptr,u32 bytes);
void *mem_alloc(u32 bytes);
void log_str(char *str);
#define EXPORT_INTERFACE 0

#endif
