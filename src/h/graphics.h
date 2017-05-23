#ifndef OSAL_USE_GPU

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../h/util.h"

/*@todo 
create flags for window creation instead of assuming stuff

remove gpu.h or move shared code outwards or something. everything is duplicated (can do later when working on porting)
*/
typedef struct window window;
//Returns the screen width and height.
void init_graphics();
window *ctor_window(char const *title, u32 size_x, u32 size_y);
void dtor_window(window *w);
void window_toggle_fullscreen(window *w, bool fullscreen);
void window_set_size(window *w, u32 size_x, u32 size_y);
void window_get_size(window *w, int *out_width, int *out_height);
void window_set_position(window *w,u32 position_x, u32 position_y);
s32 window_get_id(window *w);
//Clear screen
void clear(window *w);
//Copy your renderer to the window
void flip(window *w);
//pop implementation defined error message
char const *get_error();


typedef struct color
{
	u8 r;
	u8 g;
	u8 b;
	u8 a;
} color;
color value_color(u8 r,u8 g,u8 b, u8 a);
color u32_to_color(u32 hex);
u32 color_to_u32(color c);

//if all ctors return 0 on error, dont have to move error handling to inside

typedef struct ttf_font ttf_font;
ttf_font *ctor_ttf_font(char const *font_file_path,int fontsize);
void dtor_ttf_font(ttf_font *font);

void size_ttf_font(ttf_font *font, const char *text, int *out_w, int *out_h);

typedef struct texture texture;
texture *ctor_texture(window *w, char const *image_file_path);
texture *ctor_texture_pixels(window *w, color *pixels, u32 size_x, u32 size_y);
texture *ctor_texture_font(window *w, ttf_font *f, char const *text, color text_color);
void dtor_texture(texture *t);
void texture_set_alpha(texture *t, u8 alpha);
//color *malloc_query_texture_pixels(texture *t, u32 *out_size_x, u32 *out_size_y);
void draw_texture(window *w, texture *texture, rect *dest, f32 angle, vec2 *origin, rect *src, rect *clip_region);

#endif

#endif