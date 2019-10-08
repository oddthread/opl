#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IOS
	#include "SDL.h"
	#include "SDL_image.h"
    #include "SDL_ttf.h"
    #else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
	#include <SDL2/SDL_image.h>
    #endif
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#endif

#include "../h/graphics.h"
#include "../h/system.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*@todo 
create flags for window creation instead of assuming stuff
*/

texture *ctor_texture_font(window *w,ttf_font *f,char const *text,color text_color){
	surface *s=ctor_surface_font(f,text,text_color);
	texture *t=ctor_texture(w,s);
	dtor_surface(s);
	return t;
}
texture *ctor_texture_pixels(window *w,color *pixels,u32 size_x,u32 size_y){
	surface *s=ctor_surface_pixels(pixels,size_x,size_y);
	texture *t=ctor_texture(w,s);
	dtor_surface(s);
	return t;
}

char const *get_error()
{
	return SDL_GetError();
}

color value_color(u8 r,u8 g,u8 b, u8 a)
{
	color c;
	c.r=r;
	c.g=g;
	c.b=b;
	c.a=a;
	return c;
}
color u32_to_color(u32 hex)
{
	u8 bpp=8;/*@todo actually handle variable size pixel*/
	color c;
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
		c.r=(hex >> bpp*3);
		c.g=(hex >> bpp*2);
		c.b=(hex >> bpp*1);
		c.a=(hex >> bpp*0);
	}
	else
	{
		c.a=(hex >> bpp*3);
		c.b=(hex >> bpp*2);
		c.g=(hex >> bpp*1);
		c.r=(hex >> bpp*0);
	}
	return c;
}

u32 color_to_u32(color c)/*@bug assumes 32bit pixel*/
{
	u8 bpc=8;/*@todo actually handle variable size pixel*/
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
		return ((c.r & 0xff) << bpc*3) + ((c.g & 0xff) << bpc*2) + ((c.b & 0xff) << bpc*1) + (c.a & 0xff << bpc*0);
	else
		return ((c.a & 0xff) << bpc*3) + ((c.b & 0xff) << bpc*2) + ((c.g & 0xff) << bpc*1) + (c.r & 0xff << bpc*0);
}

/*
If width or height is 0 sets width and height from screen size (displaymode).
Returns the screen width and height.
*/
void init_graphics()
{

	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		printf( "Critical Error\n SDL_Init failed. Application will now exit.\n" );
		exit(-1);
	}
	
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	{
		printf( "SDL_SetHint failed." );
	}

	if( !IMG_Init( IMG_INIT_PNG ) )
	{
		printf( "IMG_Init failed." );
	}

	if( TTF_Init() ) {
		printf( "TTF_Init failed." );
	}
	
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"); 
	SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_PING, "0"); 
	SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE); 
	
	/*if(SDL_GL_SetSwapInterval(-1)==-1)
	{
		SDL_GL_SetSwapInterval(1);
	}*/
}

struct window
{
	SDL_Window *sdl_window;
	SDL_Renderer *sdl_renderer;
	color clear_color;
};
window *ctor_window(char const *title, u32 size_x, u32 size_y)
{
	window *w=(window*)malloc(sizeof(window));

	w->clear_color.r=0;
	w->clear_color.g=0;
	w->clear_color.b=0;
	w->clear_color.a=255;
	
	u32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

	SDL_DisplayMode mode;
	if( SDL_GetCurrentDisplayMode(0, &mode) < 0 )
	{
		printf( "SDL_GetCurrentDisplayMode failed." );
	}

	if( size_x==0 || size_y==0 )
	{
		size_x=mode.w;
		size_y=mode.h;
	}
	if( !( w->sdl_window = SDL_CreateWindow( title,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		size_x, size_y, flags) ) )
	{
		printf( "SDL_CreateWindow failed. Application will now exit." );
		exit(-1);
	}
	if( !( w->sdl_renderer = SDL_CreateRenderer( w->sdl_window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ) ) )
	{
		printf( "SDL_CreateRenderer failed." );
	}
	
	if( SDL_SetRenderDrawColor( w->sdl_renderer, 255, 255, 255, 255 ) < 0 )
	{
		printf( "SDL_SetRenderDrawColor failed. Trying software fallback." );
		w->sdl_renderer=SDL_CreateRenderer( w->sdl_window, -1, SDL_RENDERER_SOFTWARE );
	}
	SDL_SetRenderDrawBlendMode(w->sdl_renderer,SDL_BLENDMODE_BLEND);

	return w;
}
void dtor_window(window *w)
{
	SDL_DestroyRenderer(w->sdl_renderer);
	SDL_DestroyWindow(w->sdl_window);
	free(w);
}
void window_set_size(window *w, u32 size_x, u32 size_y)
{
	SDL_SetWindowSize(w->sdl_window,size_x,size_y);
}
void window_get_size(window *w, int *out_width, int *out_height)
{
	SDL_GetWindowSize(w->sdl_window,out_width,out_height);
}
void window_set_position(window *w,u32 position_x, u32 position_y)
{
	SDL_SetWindowPosition(w->sdl_window,position_x,position_y);
}
void window_toggle_fullscreen(window *w,bool fullscreen)
{
	SDL_SetWindowFullscreen(w->sdl_window,fullscreen?SDL_WINDOW_FULLSCREEN_DESKTOP:0);
}
s32 window_get_id(window *w)
{
	return SDL_GetWindowID(w->sdl_window);
}
void clear(window *w)
{
	SDL_SetRenderDrawColor( w->sdl_renderer, w->clear_color.r, w->clear_color.g, w->clear_color.b, w->clear_color.a );
	SDL_RenderClear( w->sdl_renderer );
}
void flip(window *w)
{
	SDL_RenderPresent( w->sdl_renderer );
}

void window_set_icon(window* win, char *path)
{
	SDL_Surface *s=IMG_Load(path);
	SDL_SetWindowIcon(win->sdl_window,s);
	SDL_FreeSurface(s);
}

static void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint8 *pixel_to_write=(Uint8 *)surface->pixels + y * surface->pitch + x * 4;
	*(Uint32*)pixel_to_write=pixel;
}
static Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
	return *(Uint32 *)((Uint8 *)surface->pixels + y * surface->pitch + x * 4);
}
static SDL_Surface *make_rgb_surface(int size_x,int size_y)
{
	u32 rmask, gmask, bmask, amask;
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	}
	else
	{
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	}

	SDL_Surface *surf = SDL_CreateRGBSurface(0, size_x,size_y, 32, rmask, gmask, bmask, amask);

	if(surf == NULL)
	{
		printf("%s\n",SDL_GetError());
	}
	return surf;
}

struct texture
{
	SDL_Texture *sdl_texture;
};

struct ttf_font
{
	TTF_Font *font;
};

struct surface{
	SDL_Surface *s;
};

void dtor_surface(surface *s){
	SDL_FreeSurface( s->s );
	free(s);
}

texture *ctor_texture(window *w, surface *surf){
	texture *t=(texture*)malloc(sizeof(texture));
	if(!t){
		printf("Texture alloc failed.\n");
		exit(2);
	}
	t->sdl_texture = SDL_CreateTextureFromSurface( w->sdl_renderer, surf->s );
    if(!t->sdl_texture)
    {
        printf("%s\n",SDL_GetError());
    }
	SDL_SetTextureBlendMode(t->sdl_texture, SDL_BLENDMODE_BLEND);
	return t;
}

surface *ctor_surface_file(char const *image_file_path)
{
	surface *s=(surface*)malloc(sizeof(surface));
	if(!s){
		printf("Surface alloc failed.\n");
		exit(2);
	}
	s->s = IMG_Load( image_file_path );
	return s;
}
surface *ctor_surface_pixels(color *pixels, u32 size_x, u32 size_y)
{
	surface *s=(surface*)malloc(sizeof(surface));
	SDL_Surface *surf=make_rgb_surface(size_x,size_y);
	s->s=surf;

	if(surf==NULL) 
	{
        printf("%s\n",SDL_GetError());
		free(s);
		return NULL;
	}

	SDL_LockSurface(surf);
	for(u32 a=0; a<size_x; a++)
	{
		for(u32 b=0; b<size_y; b++)
		{
			set_pixel(surf,a,b,color_to_u32(pixels[a+b*size_x]));
		}
	}
	SDL_UnlockSurface(surf);
	return s;
}

surface *ctor_surface_font(ttf_font *f,char const *text, color text_color)
{
	surface *s=(surface*)malloc(sizeof(surface));
	SDL_Color sdl_c;
	sdl_c.r=text_color.r;
	sdl_c.g=text_color.g;
	sdl_c.b=text_color.b;
	sdl_c.a=text_color.a;
	if(!text||!text[0])
	{
		return NULL;	
	}
	SDL_Surface *surf=TTF_RenderText_Blended(f->font,text,sdl_c);
	if(!surf)
	{
		printf("%s\n",SDL_GetError());
		return NULL;
	}
	s->s=surf;
	return s;
}
/*this is incorrect i think, need to use rendertarget
color *malloc_query_texture_pixels(texture *t, u32 *out_size_x, u32 *out_size_y)
{
	color *pixels;
	SDL_QueryTexture(t->SDL_Texture,pixels,NULL,out_size_x,out_size_y);
	return pixels;
}
*/

void texture_set_alpha(texture *t, u8 alpha)
{
	SDL_SetTextureAlphaMod(t->sdl_texture, alpha);
}
void dtor_texture(texture *t)
{
	if(t && t->sdl_texture)
	{
		SDL_DestroyTexture(t->sdl_texture);
		free (t);
	}
}
void draw_texture(window *w, texture *texture, rect *dest, r32 angle, vec2 *origin, rect *src, rect *clip_region)
{
	if(!texture)return;
	/*these rect structs have same format? can just cast?*/
	SDL_Rect sdl_clip_region;

	SDL_Rect sdl_src;
	SDL_Rect sdl_dest;
	
	SDL_Point sdl_center;

	sdl_src.x=0;
	sdl_src.y=0;
	sdl_src.w=0;
	sdl_src.h=0;
	if(src)
	{
		sdl_src.x=src->x;
		sdl_src.y=src->y;
		sdl_src.w=src->w;
		sdl_src.h=src->h;
	}
	
	sdl_dest.x=0;
	sdl_dest.y=0;
	sdl_dest.w=0;
	sdl_dest.h=0;
	if(dest)
	{
		sdl_dest.x=dest->x;
		sdl_dest.y=dest->y;
		sdl_dest.w=dest->w;
		sdl_dest.h=dest->h;
	}
	sdl_center.x=0;
	sdl_center.y=0;

	if(clip_region)
	{
		sdl_clip_region.x=clip_region->x;
		sdl_clip_region.y=clip_region->y;
		sdl_clip_region.w=clip_region->w;
		sdl_clip_region.h=clip_region->h;
		SDL_RenderSetClipRect(w->sdl_renderer, &sdl_clip_region);
	}

	if(origin)
	{
		sdl_center.x=origin->x;
		sdl_center.y=origin->y;
	}
	SDL_RenderCopyEx( w->sdl_renderer, texture->sdl_texture, src?&sdl_src:NULL, dest?&sdl_dest:NULL, angle, origin?&sdl_center:NULL, SDL_FLIP_NONE );
	
	/*@bug this wasnt getting reset when it was before if(clip_region)
		when cursor was !visible it was causing line numbers to get redrawn multiple times per frame (or the appearance of that) when clipping was enabled on the normal line textures
		making cursor permanently visible made it go away
		this was probably due to the cursor setting the null clip rect
		moving the sdl_rendersetcliprect call down below fixes this

		it was doing some drawing elsewhere after the clip rect was set?
	*/ 
	SDL_RenderSetClipRect(w->sdl_renderer, NULL);
}

ttf_font *ctor_ttf_font(char const *font_file_path,int fontsize)
{
	ttf_font *f=(ttf_font*)malloc(sizeof(ttf_font));
	f->font=TTF_OpenFont(font_file_path, fontsize);

	if(!f->font)printf("%s\n",SDL_GetError());

	return f;
}
void dtor_ttf_font(ttf_font *font)
{
	TTF_CloseFont(font->font);
	free(font);
}
void size_ttf_font(ttf_font *font, const char *text, int *out_w, int *out_h)
{
	if(!text || !text[0] || !out_w || !out_h){
		if(out_w){
			*out_w=0;
		}
		if(out_h){
			*out_h=0;
		}
		return;
	}
	TTF_SizeText(font->font,text,out_w,out_h);
}
void texture_query(texture *t,int *w,int *h){
	u32 a;
	int b;
	SDL_QueryTexture(t->sdl_texture,&a,&b,w,h);
}
void msgbox(char const *title, char const *message){
	
	SDL_ShowSimpleMessageBox(0,
    	title,message,
    	NULL);
}