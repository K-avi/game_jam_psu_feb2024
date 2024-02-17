#ifndef INTRO_H
#define INTRO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../timer/timer.h"

#ifndef WINDOW_SIZE
#define WINDOW_SIZE
#define WIN_X 1000
#define WIN_Y 600
#endif

typedef struct {
    const char* text;
    const char* emote_path;
} Dialog;

int strlen2(const char* str);

typedef struct {
    SDL_Texture* texture;
    SDL_Rect dest;
} Image;

void display_text_image(SDL_Renderer* renderer, TTF_Font* font, const char* text, const char* emote, Mix_Music** sfx, int sfx_count, Image* bg, Timer* timer);

Image load_background(char* path, SDL_Renderer* renderer);

// Pour chaque dialogue, on affiche le texte et l'image et on attend que l'utilisateur appuie sur entrée
void display_dialogue(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog, int dialog_count, Mix_Music** sfx, int sfx_count, Timer* timer);

void intro(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font);


void intro_point_and_click(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, Timer* timer);

#endif