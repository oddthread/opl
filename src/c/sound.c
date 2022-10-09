#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IOS
	#include "SDL.h"
    #include "SDL_mixer.h"
    #else
	#include <SDL2/SDL.h>
    #include <SDL2_mixer/SDL_mixer.h>
    #endif
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "../h/system.h"
#include "../../../oul/src/h/oul.h"

#include <string.h>

static int sound_counter=0;	

void init_sound()
{
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;
	
	if( Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0 ) 
	{
		printf("Unable to initialize audio.\n");
		exit(1);
	}
}

typedef struct sound
{
	bool loop;
	Mix_Chunk *m;
	int channel;
} sound;
sound *ctor_sound( char const *sound_file_path, bool loop )
{	
	sound *s=(sound*)malloc(sizeof(sound));
	s->m=Mix_LoadWAV(sound_file_path);
	s->loop=loop;
	s->channel=sound_counter++;
	return s;
}
void dtor_sound(sound *s)
{
	Mix_FreeChunk(s->m);
	free(s);
}
void set_volume_sound(sound *s, float normalized){
	Mix_Volume(s->channel, normalized * 128);
}
void play_sound(sound *s)
{
	Mix_PlayChannel( s->channel, s->m, s->loop?-1:0 );
}
void pause_sound(sound *s)
{
	Mix_Pause( s->channel );
}
void resume_sound(sound *s)
{
	Mix_Resume( s->channel );
}
