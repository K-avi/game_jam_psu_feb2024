
#include "intro/intro.c"
#include "point_and_click/point_and_click.c"
#include "timer/timer.c"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "labo/common.h"

#ifndef WINDOW_SIZE
#define WINDOW_SIZE
const int WIN_X = 1000;
const int WIN_Y = 600;
#endif

extern const int WINDOW_SIZE_X;
extern const int WINDOW_SIZE_Y;

Uint32 timer_callback(Uint32 interval, void* param) {
	Timer* timer = (Timer*)param;
	update_timer(timer);
	return interval;
}

int main() {

    // Init SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
		fprintf(stderr, "Error SDL2 Initialization : %s", SDL_GetError());
		exit(1);
	}

	if (TTF_Init() < 0) {
		fprintf(stderr, "Error TTF Initialization : %s", SDL_GetError());
		exit(1);
	}

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
		fprintf(stderr, "Error SDL2 Initialization : %s", SDL_GetError());
		exit(1);
	}

	// On crée la fenêtre de résumé
	SDL_Window* window =
	    SDL_CreateWindow("Game Jam", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				   WIN_X, WIN_Y, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
	if (!window) {
		fprintf(stderr, "Error window creation");
		exit(2);
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(
	    window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		fprintf(stderr, "Error renderer creation");
		exit(3);
	}

    // Load a font
    TTF_Font* font = TTF_OpenFont("intro/assets/font.ttf", 20);
    if (!font) {
        fprintf(stderr, "Error font loading : %s", TTF_GetError());
        exit(4);
    }

	TTF_Font* list_font = TTF_OpenFont("point_and_click/assets/font_list.ttf", 30);
	if (!list_font) {
		fprintf(stderr, "Error font loading : %s", TTF_GetError());
		exit(4);
	}

    // Init SDL audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Error audio initialization : %s", Mix_GetError());
        exit(5);
    }

    srand(time(NULL));

    intro(renderer, window, font);
	Timer* timer = create_timer(300, "timer/assets/background.png", "asset/compteur.png", renderer, font);

	// Run timer in a separate thread with the callback
	SDL_AddTimer(1000, timer_callback, timer);



	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

	window = SDL_CreateWindow("test labo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE_X, WINDOW_SIZE_Y,  SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	int nb_col = 15;
	int nb_row = 15;
    init_laboratoire(renderer, nb_col,nb_row,5, 2);
    laboratoire_loop(window, renderer,font, timer,5);

    //free
    end_Laboratoire();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

	window =
	    SDL_CreateWindow("Game Jam", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				   WIN_X, WIN_Y, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
	if (!window) {
		fprintf(stderr, "Error window creation");
		exit(2);
	}

	renderer = SDL_CreateRenderer(
	    window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		fprintf(stderr, "Error renderer creation");
		exit(3);
	}	


	intro_point_and_click(renderer, window, font, timer);
    int ending = point_and_click(renderer, window, list_font, timer);
	if (ending == 1) {
		printf("You win\n");
	} else if (ending == 0) {
		printf("You lose\n");
	} else if (ending == 2) {
		printf("Banana\n");
	}

	// Free memory
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_CloseFont(list_font);
	TTF_Quit();
	SDL_Quit();
}

