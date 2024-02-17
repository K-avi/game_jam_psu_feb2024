#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#ifndef WINDOW_SIZE
#define WINDOW_SIZE
#define WIN_X 1000
#define WIN_Y 600
#endif

// Load image from file
extern SDL_Surface* load_image(const char* filename);

typedef struct {
    int current_time;
    int time_max;
    SDL_Surface* timer;
    SDL_Surface* background;
    SDL_Surface* texture;
    TTF_Font* font;
    SDL_Rect dest_timer;
    SDL_Rect dest_text;
    SDL_Rect dest_background;
} Timer;

extern Timer* create_timer(int time_max, char* background_path, char* timer_path, SDL_Renderer* renderer, TTF_Font* font);
extern void update_timer(Timer* timer);

extern int is_timer_finished(Timer* timer);

extern void render_line(Timer* timer, SDL_Renderer* renderer);

extern void render_timer(Timer* timer, SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);

extern void banana_end(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);
extern void good_end(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);
extern void boom_end(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font);

#endif