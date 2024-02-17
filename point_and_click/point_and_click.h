#ifndef POINT_AND_CLICK_H
#define POINT_AND_CLICK_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../timer/timer.h"

#ifndef WINDOW_SIZE
#define WINDOW_SIZE
#define WIN_X 1000
#define WIN_Y 600
#endif

typedef struct {
    char* bottle_type;
    int nb_bottles;
} Recipe;

typedef struct {
    int x, y;
} BottleSpot;

typedef struct {
    SDL_Texture* texture;
    SDL_Rect dest;
    char* type;
} Bottle;

typedef struct {
    int x, y;
    SDL_Texture* texture;
} Pot;

typedef struct {
    SDL_Texture* background;
    SDL_Rect dest;
    Bottle* bottles;
    int bottle_count;
    Recipe* recipe;
    int recipe_size;
    Pot* pot;
} Scene;

typedef struct {
    char* bottle_type;
    int nb_bottles;
} SceneInfo;

typedef struct {
    int click_x, click_y;
    Bottle* bottle_grabbed;
} ClickInfo;



char* translation(char* taille, char* couleur, char* conteneur, char* shape);


ClickInfo* grab_bottle(Scene* scene, int x, int y);

void render_list(SDL_Renderer* renderer, TTF_Font* font, Recipe* recipe, int recipe_size, ClickInfo* click_info, char* background_path);
void render_pot(SDL_Renderer* renderer, Pot* pot);
Recipe* generate_recipe(int place_totale, int min_to_win, int* recipe_size);
SceneInfo* generate_bottle_types(int place_totale, Recipe* recipe, int recipe_size);

Scene* load_scene(SDL_Renderer* renderer);
void render_scene(SDL_Renderer* renderer, Scene* scene, TTF_Font* font);

int point_and_click(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, Timer* timer);    

#endif