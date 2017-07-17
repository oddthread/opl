#include <math.h>
#include "../h/util.h"
#include "../h/system.h"
#include <stdio.h>

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

const r64 PI=3.1415926535897;
    
#include <stdlib.h>
#include <string.h>
    
void str_reverse(char *str, int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        char temp=*(str+start);
	char temp2=*(str+end);
	*(str+start)=temp2;
	*(str+end)=temp;

        start++;
        end--;
    }
}
char* itoa(int num, char* str, int base)
{
    int i;
    bool is_negative;
    
    is_negative = false;
    i=0;
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    } 
    if (num < 0 && base == 10)
    {
        is_negative = true;
        num = -num;
    } 
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    if (is_negative)
        str[i++] = '-';
 
    str[i] = '\0';
    
    str_reverse(str, i);
 
    return str;
}

char *malloc_str_slice(char *c, s32 begin, s32 end)
{
	//signed parameters because then if you index negatively you can get a negative value here and return emptystr
	if(end-begin<0)
	{
		char *emptystr=malloc(1);
		emptystr[0]=0;
		return emptystr; 
	}
	u32 str_length=end-begin+1+1;
	
	char *retval=(char*)malloc(str_length);
	
	u32 i2=0;
	for(u32 i=begin; i<=end; i+=1)
	{
		retval[i2]=c[i];
		i2+=1;
	}
	
	retval[str_length-1]=0;
	
	return retval;
}
/*frees parameter and mallocs return val*/
char *str_insert(char *str, char c, u32 position)
{
	u32 i;
	u32 new_strlen=strlen(str)+1;
	char *newstr=malloc(new_strlen+1);
	for(i=0; i<position; i++)
	{
		newstr[i]=str[i];
	}
	newstr[position]=c;
	i++;
	for(;i<new_strlen+1; i++)
	{
		newstr[i]=str[i-1];
	}
	newstr[new_strlen]=0;
	free(str);
	return newstr;
}
/*doesnt free either parameter*/
char *str_cat(char *a, char *b)
{
	u32 i=0;
	u32 i2=0;
	u32 strlen_a=strlen(a);
	u32 strlen_b=strlen(b);

	char *newstring=malloc(strlen_a+strlen_b+1);
	for(; i<strlen_a; i++)
	{	
		newstring[i]=a[i];
	}
	for(; i2<strlen_b; i2++)
	{
		newstring[i]=b[i2];
		i++;
	}
	newstring[strlen_a+strlen_b]=0;

	return newstring;
}
/*frees parameter and mallocs return val*/
char *str_remove(char *str, u32 position)
{
	u32 i;
	u32 str_size=strlen(str);
	for(i=position; i<str_size-1; i++)
	{
		str[i]=str[i+1];
	}
	str[str_size-1]=0;
	str=(char*)realloc(str,str_size);
	return str;
}
char *str_remove_characters(char *str, char c)
{
	u32 i=0;
	while(str[i])
	{
		if(str[i]==c)
		{
			str=str_remove(str,i);
			i--;
		}
		i++;
	}
	return str;
}

vec2 value_vec2(r32 size_x, r32 size_y)
{
	vec2 v;
	v.x=size_x;
	v.y=size_y;
	return v;
}
vec2 vec2_scale(vec2 v,r64 factor)
{
	v.x*=factor;
	v.y*=factor;
	return v;
}
r64 vec2_to_angle(vec2 v)
{
	return atan2(v.y, v.x)*180/PI;
}
r64 vec2_len(vec2 v)
{
	return sqrt(pow(v.x,2)+pow(v.y,2));
}
vec2 vec2_unit(vec2 v)
{
	r64 lenvalue=vec2_len(v);
	v.x=v.x/lenvalue;
	v.y=v.y/lenvalue;
	return v;
}
vec2 vec2_abs(vec2 v)
{
	v.x=(r64)fabs(v.x);
	v.y=(r64)fabs(v.y);
	return v;
}
vec2 vec2_add(vec2 a, vec2 b)
{
	vec2 v;
	v.x=a.x+b.x;
	v.y=a.y+b.y;
	return v;
}
vec2 vec2_sub(vec2 a, vec2 b)
{
	vec2 v;
	v.x=a.x-b.x;
	v.y=a.y-b.y;
	return v;
}

r64 vec4_len(vec4 v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}
vec4 vec4_unit(vec4 v)
{
	r64 lenvalue=vec4_len(v);
	v.x/=lenvalue;
	v.y/=lenvalue;
	v.z/=lenvalue;
	v.w/=lenvalue;
	return v;
}
vec4 vec4_mul(vec4 a,vec4 b)
{
	a.x*=b.x;
	a.y*=b.y;
	a.z*=b.z;
	a.w*=b.w;
	return a;
}

bool insec(vec2 pos_a, vec2 size_a, vec2 pos_b, vec2 size_b) 
{
    return !(pos_b.x > pos_a.x + size_a.x ||
        pos_b.x + size_b.x < pos_a.x ||
        pos_b.y > pos_a.y + size_a.y ||
        pos_b.y + size_b.y < pos_a.y);
}
