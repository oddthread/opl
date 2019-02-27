/*This file was automatically generated.*/
#ifndef d_graphics_h
#define d_graphics_h

#include "../../../oul/src/h/oul.h"

void init_graphics();

typedef struct ttf_font ttf_font;
void size_ttf_font(ttf_font *font,const char *text,int *out_w,int *out_h);
void dtor_ttf_font(ttf_font *font);
ttf_font *ctor_ttf_font(char const *font_file_path,int fontsize);
typedef struct window window;
typedef struct texture texture;
void draw_texture(window *w,texture *texture,rect *dest,r32 angle,vec2 *origin,rect *src,rect *clip_region);
void dtor_texture(texture *t);
void texture_query(texture *t, int *w, int *h);
void texture_set_alpha(texture *t,u8 alpha);

typedef struct color color;
struct color {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};
typedef struct surface surface;
surface *ctor_surface_font(ttf_font *f,char const *text,color text_color);
surface *ctor_surface_pixels(color *pixels,u32 size_x,u32 size_y);
surface *ctor_surface_file(char const *image_file_path);
void dtor_surface(surface *s);

texture *ctor_texture(window *w,surface *s);
texture *ctor_texture_font(window *w,ttf_font *f,char const *text,color text_color);
texture *ctor_texture_pixels(window *w,color *pixels,u32 size_x,u32 size_y);

void window_set_icon(window *win,char *path);
void flip(window *w);
void clear(window *w);
s32 window_get_id(window *w);
void window_toggle_fullscreen(window *w,bool fullscreen);
void window_set_position(window *w,u32 position_x,u32 position_y);
void window_get_size(window *w,int *out_width,int *out_height);
void window_set_size(window *w,u32 size_x,u32 size_y);
void dtor_window(window *w);
window *ctor_window(char const *title,u32 size_x,u32 size_y);
u32 color_to_u32(color c);
#define color_to_hex color_to_u32
color u32_to_color(u32 hex);
color value_color(u8 r,u8 g,u8 b,u8 a);
char const *get_error();
void msgbox(const char *title, const char *msg);


#endif

