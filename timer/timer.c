#include "timer.h"

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
SDL_Surface* load_image(const char* filename) {
    SDL_Surface* img = IMG_Load(filename);
    if (!img) {
        fprintf(stderr, "Error loading image %s : %s", filename, IMG_GetError());
        exit(5);
    }
    return img;
}

Timer* create_timer(int time_max, char* background_path, char* timer_path, SDL_Renderer* renderer, TTF_Font* font) {
    Timer* timer = malloc(sizeof(Timer));
    if (timer == NULL) {
        fprintf(stderr, "Error allocating memory for timer");
        exit(6);
    }
    timer->current_time = 0;
    timer->time_max = time_max;
    timer->font = font;
    timer->background = load_image(background_path);
    timer->timer = load_image(timer_path);
    timer->dest_timer.x = WIN_X - 140;
    timer->dest_timer.y = 40;
    timer->dest_timer.w = 160;
    timer->dest_timer.h = 160;
    timer->dest_background.x = WIN_X - 150;
    timer->dest_background.y = -100;
    timer->dest_background.w = 200;
    timer->dest_background.h = 200;
    return timer;
}

void update_timer(Timer* timer) {
    static int nb_times_called = 0;
    if (timer == NULL) {
        return;
    }
    timer->current_time++;
    // load a sound mp3
    // make it louder every 17 calls

    //printf("Current time : %d\n", timer->current_time);
    
    Mix_Music* sfx = Mix_LoadMUS("timer/assets/geiger.mp3");
    if (sfx == NULL) {
        fprintf(stderr, "Error loading sfx : %s", Mix_GetError());
        exit(7);
    }
    if (nb_times_called % 17 == 0) {
        Mix_MasterVolume(30 + (nb_times_called / 3));
        Mix_PlayMusic(sfx, 1);
    }
    nb_times_called++;

    //Mix_FreeMusic(sfx);
}

int is_timer_finished(Timer* timer) {
    if (timer == NULL) {
        return 0;
    }
    return timer->current_time >= timer->time_max;
}

void render_line(Timer* timer, SDL_Renderer* renderer) {
    if (timer == NULL) {
        return;
    }
    // make a line that rotates around a point depending on the percentage of time
    int center_x = WIN_X - 35;
    int center_y = 125;
    int radius = 20;
    int angle = (timer->current_time * 360) / timer->time_max;
    int x = center_x + radius * cos(angle * 3.14159 / 180);
    int y = center_y + radius * sin(angle * 3.14159 / 180);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, center_x, center_y, x, y);
}

void render_timer(Timer* timer, SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    if (timer == NULL) {
        return;
    }

    SDL_Texture* timer_texture = SDL_CreateTextureFromSurface(renderer, timer->timer);
    SDL_RenderCopy(renderer, timer_texture, NULL, &timer->dest_timer);
    SDL_DestroyTexture(timer_texture);

    SDL_Texture* background_texture = SDL_CreateTextureFromSurface(renderer, timer->background);
    SDL_RenderCopy(renderer, background_texture, NULL, &timer->dest_background);
    SDL_DestroyTexture(background_texture);
    
    SDL_Color color = {0, 0, 0, 255};
    char text[100];
    sprintf(text, "%d %%", (timer->current_time * 100) / timer->time_max);
    SDL_Surface* text_surface = TTF_RenderText_Blended_Wrapped(timer->font, text, color, 100);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_QueryTexture(text_texture, NULL, NULL, &timer->dest_text.w, &timer->dest_text.h);
    timer->dest_text.x = WIN_X - 80;
    timer->dest_text.y = 50;
    SDL_RenderCopy(renderer, text_texture, NULL, &timer->dest_text);
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);

    render_line(timer, renderer);

}

void banana_end(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Surface* banana = load_image("asset/gamejam2_ending/banana_ending.png");
    SDL_Texture* banana_texture = SDL_CreateTextureFromSurface(renderer, banana);
    SDL_Rect dest_banana;
    dest_banana.x = 0;
    dest_banana.y = 0;
    dest_banana.w = WIN_X;
    dest_banana.h = WIN_Y;
    SDL_RenderCopy(renderer, banana_texture, NULL, &dest_banana);
    SDL_RenderPresent(renderer);
    while (1) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) {
            break;
        }
    }
    SDL_DestroyTexture(banana_texture);
    SDL_FreeSurface(banana);
    exit(0);
}

void good_end(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Surface* good = load_image("asset/gamejam2_ending/good_ending.png");
    SDL_Texture* good_texture = SDL_CreateTextureFromSurface(renderer, good);
    SDL_Rect dest_good;
    dest_good.x = 0;
    dest_good.y = 0;
    dest_good.w = WIN_X;
    dest_good.h = WIN_Y;
    SDL_RenderCopy(renderer, good_texture, NULL, &dest_good);
    SDL_RenderPresent(renderer);
    while (1) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) {
            break;
        }
    }
    SDL_DestroyTexture(good_texture);
    SDL_FreeSurface(good);
    exit(0);
}

void boom_end(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Surface* boom = load_image("asset/gamejam2_ending/boom_ending.png");
    SDL_Texture* boom_texture = SDL_CreateTextureFromSurface(renderer, boom);
    SDL_Rect dest_boom;
    dest_boom.x = 0;
    dest_boom.y = 0;
    dest_boom.w = WIN_X;
    dest_boom.h = WIN_Y;
    SDL_RenderCopy(renderer, boom_texture, NULL, &dest_boom);
    SDL_RenderPresent(renderer);
    while (1) {
        SDL_Event event;
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) {
            break;
        }
    }
    SDL_DestroyTexture(boom_texture);
    SDL_FreeSurface(boom);
    exit(0);
}