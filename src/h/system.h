#ifndef d_system_h
#define d_system_h

#include "oul/src/h/oul.h"

void *mem_alloc(u32 bytes);

void *mem_realloc(void *ptr, u32 bytes);
void mem_free(void *p);

void log_str(char *str);

int rand_num(s32 min, s32 max);
void sleep_milli(u32 milliseconds);
s64 milli_current_time();
char *alloc_file_to_str(char *path);
void str_to_file(char *path, char *str);
int rand_num(s32 min, s32 max);

/*must be freed with system_free*/
char *get_clipboard_text();

void set_clipboard_text(char *str);

/*must be freed with system_free*/
char *get_base_path();

void system_free(void *p);
/*
struct raw_thread;
void wait_thread(raw_thread *t,int *status_out);
raw_thread *ctor_raw_thread(int(*func)(void*),char const *name,void *data);

struct mutex;
mutex *ctor_mutex();
void dtor_mutex(mutex* m);
void lock_mutex(mutex *m);
void unlock_mutex(mutex *m);
*/
#endif
