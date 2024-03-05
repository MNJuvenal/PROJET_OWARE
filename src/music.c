#include "music.h"

void play_music(char *music)
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr,"Erreur SDL_INIT_AUDIO : %s", SDL_GetError());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) 
    {
        fprintf(stderr,"Erreur Mix_OpenAudio : %s", Mix_GetError());
    }

    Mix_Music *audioMusic = Mix_LoadMUS(music);
    if (audioMusic == NULL) 
    {
        fprintf(stderr,"Erreur Mix_LoadMUS : %s", Mix_GetError());
    }

    //On ne répétera la musique qu'une seule fois
    Mix_PlayMusic(audioMusic, 0);

    while (Mix_PlayingMusic() == 1)
    {
        //On attend que la lecture soit terminée
    }

    Mix_FreeMusic(audioMusic);
    Mix_CloseAudio();
}