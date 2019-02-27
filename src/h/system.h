/*This file was automatically generated.*/
#ifndef d_system_h
#define d_system_h

#include "../../../oul/src/h/oul.h"

char *get_base_path();
void str_to_file(char *path,char *cstr);
char *alloc_file_to_str(char *path);
void sleep_milli(u32 milliseconds);
s64 milli_current_time();
void create_empty_file_if_not_exists(char *filepath);
float opl_ntohf(float value);
float opl_htonf(float value);
void set_clipboard_text(char *c);
void system_free(void *p);
char *get_clipboard_text();
void mem_free(void *p);
void *mem_realloc(void *ptr,u32 bytes);
void *mem_alloc(u32 bytes);
void log_str(char *str);

typedef struct raw_thread raw_thread;
void dtor_raw_thread(raw_thread *t,int *status_out);
raw_thread *ctor_raw_thread(int(*func)(void*),char const *name,void *data);

typedef struct sem sem;
sem *ctor_sem(int val);
void dtor_sem(sem* s);
int remove_sem(sem *s);
int try_remove_sem(sem *s);
int add_sem(sem *s);


#endif
