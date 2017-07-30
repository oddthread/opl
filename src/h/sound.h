#ifndef d_sound_h
#define d_sound_h

void init_sound();

typedef struct sound sound;

sound *ctor_sound(char *sound_file_path,bool loop);
void dtor_sound(sound *s);
void play_sound(sound *s);
void pause_sound(sound *s);
void resume_sound(sound *s);

#endif