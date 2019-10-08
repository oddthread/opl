#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IOS
	#include "SDL.h"
    #else
	#include <SDL2/SDL.h>
    #endif
#else
#include <SDL2/SDL.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include "../h/system.h"
#include "../../../oul/src/h/oul.h"
#include <time.h>

#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void list_dir(char const *path){
	struct dirent *de;
    DIR *dr = opendir(path); 
    if (dr == NULL){
        printf("Directory doesn't exist: %s\n",path); 
        return;
    } 
    while ((de = readdir(dr)) != NULL) 
            printf("%s\n", de->d_name); 
  
    closedir(dr);  
}
char* sdl_file_read(char const *filename) {
		/*
		#ifdef __APPLE__
			#include "TargetConditionals.h"
			#if TARGET_OS_IOS
            char buff[2048];
            strcat(buff,SDL_GetBasePath());
			list_dir("./");
            strcat(buff,"Resources/");
            strcat(buff,filename);
            filename=buff;
			printf("final filename: %s\n",filename);
			#endif
		#endif
		*/
        SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
        if (rw == NULL) return NULL;

        Sint64 res_size = SDL_RWsize(rw);
        char* res = (char*)malloc(res_size + 1);

        Sint64 nb_read_total = 0, nb_read = 1;
        char* buf = res;
        while (nb_read_total < res_size && nb_read != 0) {
                nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
                nb_read_total += nb_read;
                buf += nb_read;
        }
        SDL_RWclose(rw);
        if (nb_read_total != res_size) {
                free(res);
                return NULL;
        }

        res[nb_read_total] = '\0';
        return res;
}
float opl_ntohf(float value){
    return SDL_SwapFloatBE(value);
}
float opl_htonf(float value){
	if(SDL_BYTEORDER==SDL_LIL_ENDIAN){
		return SDL_SwapFloat(value);
	}
	return value;
}

void log_str(char *str)
{
	printf("%s",str);
}

void *mem_alloc(u32 bytes)
{
	return malloc(bytes);
}
void *mem_realloc(void *ptr, u32 bytes)
{
	return realloc(ptr,bytes);
}
void mem_free(void *p)
{
	free(p);
}

/*must be freed with system_free*/
char *get_clipboard_text()
{
	return SDL_GetClipboardText();
}
void system_free(void* p)
{
	SDL_free(p);
}
void set_clipboard_text(char *c)
{
	SDL_SetClipboardText(c);
}
s64 milli_current_time()
{
	return SDL_GetTicks();
}
void sleep_milli(u32 milliseconds)
{
	SDL_Delay(milliseconds);
}

void create_empty_file_if_not_exists(char *filepath){
	FILE *fptr;
	fptr = fopen(filepath, "rb+");
	if(fptr == NULL)
	{
		fptr = fopen(filepath, "wb");
	}
}

char *alloc_file_to_str(char *path)
{
#ifdef  __APPLEdisabled__
	char adjpath[1024];
	uint32_t size = sizeof(adjpath);
	_NSGetExecutablePath(adjpath, &size);
	string adjpathstr=adjpath;
	for(int i=adjpathstr.size(); i>0; i--)
	{
		if(adjpathstr[i]=='/' || adjpathstr[i]=='\\')
		{
			adjpathstr=adjpathstr.slice(0,i-1);
			break;
		}
	}
	string finalpathadj=adjpathstr+("/")+(path);
	path=finalpathadj.c_str();
#endif

    FILE* fp=fopen(path, "rb");
    if(!fp)
    {
        printf("Could not open file.");
        return NULL;
    }
	
	fseek(fp, 0L, SEEK_END);
	u32 sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char *buff=(char*)malloc(sz+1);
	fread(buff, 1, sz, fp);
	buff[sz]=0;
	fclose(fp);
	
    return buff;
}
void str_to_file(char *path, char *cstr)
{
#ifdef  __APPLEdisabled__
	char adjpath[1024];
	uint32_t size = sizeof(adjpath);
	_NSGetExecutablePath(adjpath, &size);
	string adjpathstr=adjpath;
	for(int i=adjpathstr.size(); i>0; i--)
	{
		if(adjpathstr[i]=='/' || adjpathstr[i]=='\\')
		{
			adjpathstr=adjpathstr.slice(0,i-1);
			break;
		}
	}
	string finalpathadj=adjpathstr+("/")+(path);
	path=finalpathadj.c_str();
#endif

    FILE* fp=fopen(path, "w");
    if(!fp)
    {
        printf("Could not open file: %s\n",path);
		return;
    }

    fprintf(fp,"%s",cstr);
    fclose(fp);
}

/*must be freed with system_free*/
char *get_base_path()
{
	return SDL_GetBasePath();
}
#if 0
int rand_num(s32 min, s32 max)
{
    return 42;//@todo @bug @temp
}
#endif

struct raw_thread{
	SDL_Thread *st;
};
void dtor_raw_thread(raw_thread *t,int *status_out){
	SDL_WaitThread(t->st, status_out);
	free(t);
}
raw_thread *ctor_raw_thread(int(*func)(void*),char const *name,void *data){
	raw_thread *rt=(raw_thread*)malloc(sizeof(raw_thread));

	rt->st=SDL_CreateThread(func,name,data);
	if(!rt || !rt->st){
		printf("raw_thread could not be created.\n");
		exit(2);
	}
	return rt;
}

struct sem{
	SDL_sem *ss;
};
sem *ctor_sem(int val){
	sem *l=(sem*)malloc(sizeof(sem));
	l->ss=SDL_CreateSemaphore(val);
	return l;
}
void dtor_sem(sem* s){
	SDL_DestroySemaphore(s->ss);
	free(s);
}
int remove_sem(sem *s){
	return SDL_SemWait(s->ss);
}
int try_remove_sem(sem *s){
	return SDL_SemTryWait(s->ss);
}
int add_sem(sem *s){
	return SDL_SemPost(s->ss);
}

