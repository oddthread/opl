#ifndef d_util_h
#define d_util_h

#include <stdint.h>


#define d_insert(array,array_size,position,element) array_size+=1;\
array=realloc(array,array_size*sizeof(element));\
for(i=array_size-2; i>=position; i--)\
{\
    array[i+1]=array[i];\
}\
array[position]=element;

#define d_append(array,array_size,element) array_size+=1;\
array=realloc(array,array_size*sizeof(element));\
array[array_size-1]=element;

#define d_remove(array,array_size,position,fn) if(array_size) {\
	fn(array[position]);\
	for(i=position; i<array_size-1; i++)\
	{\
		array[i]=array[i+1];\
	}\
	array_size-=1;\
	array=realloc(array,array_size*sizeof(array[0]));\
}

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

typedef double r64;
typedef float r32;

#ifndef __cplusplus
	#ifndef bool
	typedef u8 bool;
	#endif

    #ifndef true
    #define true 1
    #endif

    #ifndef false
    #define false 0
    #endif
#endif

extern const r64 PI;

void str_reverse(char *str, int length);
char* itoa(int num, char* str, int base);

char *malloc_str_slice(char *c, s32 begin, s32 end);
/*frees parameter and mallocs return val*/
char *str_insert(char *str, char c, u32 position);
/*frees parameter and mallocs return val*/
char *str_remove(char *str, u32 position);
/*frees parameter and mallocs return val*/
char *str_remove_characters(char *str, char c);
/*doesnt free either parameter*/
char *str_cat(char *a, char *b);

typedef struct vec2
{
	r32 x;
	r32 y;
} vec2;
typedef struct vec3
{
	r32 x;
	r32 y;
	r32 z;
} vec3;
typedef struct vec4
{
	r32 x;
	r32 y;
	r32 z;
	r32 w;
} vec4;

typedef struct rect
{
	r32 x;
	r32 y;
	r32 w;
	r32 h;
} rect;

vec2 value_vec2(r32 size_x, r32 size_y);
vec2 vec2_scale(vec2 v,r64 factor);
r64 vec2_to_angle();
r64 vec2_len();
vec2 vec2_abs();
vec2 vec2_unit();
vec2 vec2_add(vec2 a, vec2 b);
vec2 vec2_sub(vec2 a, vec2 b);

r64 vec4_len(vec4 v);
vec4 vec4_unit(vec4 v);
vec4 vec4_mul(vec4 a,vec4 b);

bool insec(vec2 pos_a, vec2 size_a, vec2 pos_b, vec2 size_b);

#endif
