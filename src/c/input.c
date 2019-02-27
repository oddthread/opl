#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>
#endif

#include <stdlib.h>

#include "../h/input.h"
#include "../../../oul/src/h/oul.h"

#include <stdlib.h>
#include <string.h>

struct system_cursor 
{
	SDL_Cursor *c;
};

/* @bug @corruption
if CURSOR_TEXT and CURSOR_NORMAL are not zero initialized:

when pasting a multiline comment (or probably any add_text call with multiple lines)
then selecting multiple lines, and pressing tab to insert four spaces and delete the selection,
after that moving the mouse causes a crash.
It changes the value of the cursor and the next call to set_cursor will pass invalid memory.

This happens because some code is writing to statically allocated storage, .DATA segment, but
zero initialized static variables go in .BSS segment 
https://stackoverflow.com/questions/93039/where-are-static-variables-stored-in-c-c
edit: it is zero initialized by default but apparently just assigning NULL to it will move it to the .DATA segment

So by zero initializing it, it gets moved to a different location in memory and away from the area buggy code is overwriting
(this is so bad)

@FIXED
the statically allocated variable was the ote_log_buffer, it was only 100 characters and I was writing more than 100 character logs,
after increasing it to 1000 there are no more problems.

the nuance of the bug described above was probably just because that was the code path that was calling the logging function.
*/
system_cursor *CURSOR_TEXT;
system_cursor *CURSOR_NORMAL;
system_cursor *CURSOR_HAND;
static system_cursor *current_cursor=NULL;

vec2 get_mouse_position()
{
	vec2 v;
	s32 x;
	s32 y;
	SDL_GetMouseState(&x,&y);
	v.x=x;
	v.y=y;
	return v;
}
static system_cursor *ctor_system_cursor(SDL_Cursor* c)
{
	system_cursor *s=(system_cursor*)malloc(sizeof(system_cursor));
	s->c=c;
	return s;
}

void init_input()
{
	CURSOR_NORMAL=ctor_system_cursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
	CURSOR_TEXT=ctor_system_cursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM));
	CURSOR_HAND=ctor_system_cursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
}

void capture_mouse_curwindow(bool capture)
{
	#ifdef _WIN32	
	SDL_CaptureMouse(capture);
	#endif
}
	

void set_cursor(system_cursor *s)
{
	if(s!=current_cursor)
	{
		SDL_SetCursor(s->c);
		current_cursor=s;
	}
}

const s32 ALL_EVENTS=0;
const s32 MOUSE_EVENTS=1;
void flush_events(s32 event_type)
{
	SDL_PumpEvents();
	if(event_type==ALL_EVENTS)
	{
		SDL_FlushEvents(SDL_FIRSTEVENT,SDL_LASTEVENT);
	}
	if(event_type==MOUSE_EVENTS)
	{
		SDL_FlushEvent(SDL_MOUSEBUTTONDOWN | SDL_MOUSEBUTTONUP);
	}
}

s32 get_mod_state()
{
	return SDL_GetModState();
}
const s32 KEY_MOD_CAPS=KMOD_CAPS;
const s32 KEY_MOD_SHIFT=KMOD_SHIFT;

const s64 LEFT_MOUSE=SDL_BUTTON_LEFT;
const s64 RIGHT_MOUSE=SDL_BUTTON_RIGHT;
const s64 MIDDLE_MOUSE=SDL_BUTTON_MIDDLE;
const s64 MOUSE_MOTION=SDL_MOUSEMOTION;/*@bug i dont know what the range is on the SDL constants, this might also be another key*/
const s64 MOUSE_WHEEL=SDL_MOUSEWHEEL;

const s64 DROP_FILE=SDL_DROPFILE;

const s64 KEY_LEFT=SDLK_LEFT;
const s64 KEY_RIGHT=SDLK_RIGHT;
const s64 KEY_UP=SDLK_UP;
const s64 KEY_DOWN=SDLK_DOWN;
const s64 KEY_INSERT=SDLK_INSERT;
const s64 KEY_DELETE=SDLK_DELETE;
const s64 KEY_HOME=SDLK_HOME;
const s64 KEY_END=SDLK_END;
const s64 KEY_PAGE_UP=SDLK_PAGEUP;
const s64 KEY_PAGE_DOWN=SDLK_PAGEDOWN;
const s64 KEY_PRINT_SCREEN=SDLK_PRINTSCREEN;
const s64 KEY_SCROLL_LOCK=SDLK_SCROLLLOCK;
const s64 KEY_PAUSE_BREAK=SDLK_PAUSE;
const s64 KEY_KP_0=SDLK_KP_0;
const s64 KEY_KP_1=SDLK_KP_1;
const s64 KEY_KP_2=SDLK_KP_2;
const s64 KEY_KP_3=SDLK_KP_3;
const s64 KEY_KP_4=SDLK_KP_4;
const s64 KEY_KP_5=SDLK_KP_5;
const s64 KEY_KP_6=SDLK_KP_6;
const s64 KEY_KP_7=SDLK_KP_7;
const s64 KEY_KP_8=SDLK_KP_8;
const s64 KEY_KP_9=SDLK_KP_9;
const s64 KEY_KP_NUM_LOCK=SDLK_NUMLOCKCLEAR;
const s64 KEY_KP_DIVIDE=SDLK_KP_DIVIDE;
const s64 KEY_KP_MULTIPLY=SDLK_KP_MULTIPLY;
const s64 KEY_KP_ADD=SDLK_PLUS;
const s64 KEY_KP_SUBTRACT=SDLK_MINUS;
const s64 KEY_KP_ENTER=SDLK_KP_ENTER;
const s64 KEY_KP_PERIOD=SDLK_KP_ENTER;
const s64 KEY_ESCAPE=SDLK_ESCAPE;
const s64 KEY_GRAVE=SDLK_BACKQUOTE;
const s64 KEY_0=SDLK_0;
const s64 KEY_1=SDLK_1;
const s64 KEY_2=SDLK_2;
const s64 KEY_3=SDLK_3;
const s64 KEY_4=SDLK_4;
const s64 KEY_5=SDLK_5;
const s64 KEY_6=SDLK_6;
const s64 KEY_7=SDLK_7;
const s64 KEY_8=SDLK_8;
const s64 KEY_9=SDLK_9;
const s64 KEY_SUBTRACT=SDLK_MINUS;
const s64 KEY_EQUALS=SDLK_EQUALS;
const s64 KEY_F1=SDLK_F1;
const s64 KEY_F2=SDLK_F2;
const s64 KEY_F3=SDLK_F3;
const s64 KEY_F4=SDLK_F4;
const s64 KEY_F5=SDLK_F5;
const s64 KEY_F6=SDLK_F6;
const s64 KEY_F7=SDLK_F7;
const s64 KEY_F8=SDLK_F8;
const s64 KEY_F9=SDLK_F9;
const s64 KEY_F10=SDLK_F10;
const s64 KEY_F11=SDLK_F11;
const s64 KEY_F12=SDLK_F12;
const s64 KEY_TAB=SDLK_TAB;
const s64 KEY_CAPS_LOCK=SDLK_CAPSLOCK;
const s64 KEY_ENTER=SDLK_RETURN;
const s64 KEY_BACKSPACE=SDLK_BACKSPACE;
const s64 KEY_SPACE=SDLK_SPACE;
const s64 KEY_LEFT_BRACKET=SDLK_LEFTBRACKET;
const s64 KEY_RIGHT_BRACKET=SDLK_RIGHTBRACKET;
const s64 KEY_SEMI_COLON=SDLK_SEMICOLON;
const s64 KEY_QUOTE=SDLK_QUOTE;
const s64 KEY_PERIOD=SDLK_PERIOD;
const s64 KEY_COMMA=SDLK_COMMA;
const s64 KEY_DIVIDE=SDLK_SLASH;
const s64 KEY_BACK_SLASH=SDLK_BACKSLASH;
const s64 KEY_A=SDLK_a;
const s64 KEY_B=SDLK_b;
const s64 KEY_C=SDLK_c;
const s64 KEY_D=SDLK_d;
const s64 KEY_E=SDLK_e;
const s64 KEY_F=SDLK_f;
const s64 KEY_G=SDLK_g;
const s64 KEY_H=SDLK_h;
const s64 KEY_I=SDLK_i;
const s64 KEY_J=SDLK_j;
const s64 KEY_K=SDLK_k;
const s64 KEY_L=SDLK_l;
const s64 KEY_M=SDLK_m;
const s64 KEY_N=SDLK_n;
const s64 KEY_O=SDLK_o;
const s64 KEY_P=SDLK_p;
const s64 KEY_Q=SDLK_q;
const s64 KEY_R=SDLK_r;
const s64 KEY_S=SDLK_s;
const s64 KEY_T=SDLK_t;
const s64 KEY_U=SDLK_u;
const s64 KEY_V=SDLK_v;
const s64 KEY_W=SDLK_w;
const s64 KEY_X=SDLK_x;
const s64 KEY_Y=SDLK_y;
const s64 KEY_Z=SDLK_z;
const s64 KEY_LEFT_SHIFT=SDLK_LSHIFT;
const s64 KEY_LEFT_CONTROL=SDLK_LCTRL;
const s64 KEY_LEFT_ALT=SDLK_LALT;
const s64 KEY_RIGHT_SHIFT=SDLK_RSHIFT;
const s64 KEY_RIGHT_CONTROL=SDLK_RCTRL;
const s64 KEY_RIGHT_ALT=SDLK_RALT;
const s64 KEY_RGUI=SDLK_RGUI;
const s64 KEY_LGUI=SDLK_LGUI;

const s64 WINDOW_CLOSE=SDL_WINDOWEVENT_CLOSE;
const s64 FOCUS_GAINED=SDL_WINDOWEVENT_FOCUS_GAINED;
const s64 FOCUS_LOST=SDL_WINDOWEVENT_FOCUS_LOST;

event *poll_input(event *e)
{
	SDL_Event sdl_event;
	if(SDL_PollEvent( &sdl_event ))
	{
		/*if( sdl_event.type == SDL_QUIT ) exit( 0 );*//*if they try to close the window we just quit immediately*/
		if (sdl_event.type == SDL_WINDOWEVENT) 
		{
			e->type=sdl_event.window.event;
			e->id=sdl_event.window.windowID;	
		}
		if(sdl_event.type==SDL_DROPFILE)
		{
			e->type=SDL_DROPFILE;
			e->str=sdl_event.drop.file;
		}
		if( sdl_event.key.type == SDL_KEYDOWN )
		{
			e->type=sdl_event.key.keysym.sym;
			e->pressed=true;
		}
		if( sdl_event.key.type == SDL_KEYUP )
		{
			e->type=sdl_event.key.keysym.sym;
			e->pressed=false;
		}

		if( sdl_event.button.type == SDL_MOUSEBUTTONDOWN )
		{
			e->type=sdl_event.button.button;
			e->pressed=true;
		}
		if( sdl_event.button.type == SDL_MOUSEBUTTONUP )
		{
			e->type=sdl_event.button.button;
			e->pressed=false;
		}

		if( sdl_event.button.type == SDL_MOUSEMOTION )
		{
			e->type=SDL_MOUSEMOTION;
			e->pressed=false;
			e->mouse_info.x = sdl_event.motion.x;
			e->mouse_info.y = sdl_event.motion.y;
		}
		if( sdl_event.wheel.type == SDL_MOUSEWHEEL )
		{
			e->type=SDL_MOUSEWHEEL;
			e->pressed=false;
			e->mouse_info.x = -sdl_event.wheel.x;
			e->mouse_info.y = -sdl_event.wheel.y;
		}
		return e;
	}
	return NULL;
}

char apply_shift(char c,bool include_non_alpha)
{
	if(include_non_alpha)
	{
		if(c=='`')
		{
			return '~';
		}
		if(c=='1')
		{
			return '!';
		}
		if(c=='2')
		{
			return '@';
		}
		if(c=='3')
		{
			return '#';
		}
		if(c=='4')
		{
			return '$';
		}
		if(c=='5')
		{
			return '%';
		}
		if(c=='6')
		{
			return '^';
		}
		if(c=='7')
		{
			return '&';
		}
		if(c=='8')
		{
			return '*';
		}
		if(c=='9')
		{
			return '(';
		}
		if(c=='0')
		{
			return ')';
		}
		if(c=='-')
		{
			return '_';
		}
		if(c=='=')
		{
			return '+';
		}

		if(c=='[')
		{
			return '{';
		}
		if(c==']')
		{
			return '}';
		}
		if(c=='\\')
		{
			return '|';
		}
		if(c==';')
		{
			return ':';
		}
		if(c=='\'')
		{
			return '"';
		}
		if(c==',')
		{
			return '<';
		}
		if(c=='.')
		{
			return '>';
		}
		if(c=='/')
		{
			return '?';
		}
	}

	if(c>96&&c<123){
		return c-32;
	}
	
	return c;
}
