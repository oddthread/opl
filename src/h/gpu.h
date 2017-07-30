#ifdef d_opl_use_gpu

#ifndef d_gpu_h
#define d_gpu_h

#include "oul/src/h/oul.h"

/*Returns the screen width and height.*/
vec2 init_graphics();

typedef struct vertex_shader vertex_shader;
typedef struct fragment_shader fragment_shader;
typedef struct shader_program shader_program;
typedef struct texture texture;
typedef struct model model;

void log_error();

model *ctor_model_obj(char *path);
model *ctor_model_fbx(char *path);
void dtor_model(model *m);

texture *ctor_texture(char *path);
texture *ctor_texture_dds(char *path);
void dtor_texture(texture *t);

vertex_shader *ctor_compile_vertex_shader(const char *source);
void dtor_vertex_shader(vertex_shader *v);
fragment_shader *ctor_compile_fragment_shader(const char *source);
void dtor_fragment_shader(fragment_shader *f);
void use_shaders(vertex_shader *v, fragment_shader *f);

shader_program *ctor_shader_program(vertex_shader *v, fragment_shader *f);
void dtor_shader_program(shader_program *s);

void render_model(model *m,shader_program *s, texture* t, vec3 translation, vec4 rotation, vec3 scale);

void clear();
void flip();

typedef struct color
{
	u8 r,g,b,a;
} color;
u32 color_to_hex(color c);

#endif

#endif
