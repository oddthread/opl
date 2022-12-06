/*This file was automatically generated.*/
#ifndef d_sound_h
#define d_sound_h

void init_sound();

typedef struct sound sound;
void resume_sound(sound *s);
void pause_sound(sound *s);
void set_volume_sound(sound *s, float normalized);
void play_sound(sound *s);
void dtor_sound(sound *s);
sound *ctor_sound(char const *sound_file_path, bool loop, int channel = -1);
sound *ctor_sound_rw(SDL_RWops *sound_file, bool loop, int channel = -1);

#endif
