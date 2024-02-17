#include "intro/intro.c"
#include "point_and_click/point_and_click.c"

#ifndef WINDOW_SIZE
#define WINDOW_SIZE
const int WIN_X = 1000;
const int WIN_Y = 600;
#endif


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

	// On crée la fenêtre de résumé
	SDL_Window* window =
	    SDL_CreateWindow("Game Jam", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				   WIN_X, WIN_Y, SDL_WINDOW_OPENGL);
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

    // Init SDL audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Error audio initialization : %s", Mix_GetError());
        exit(5);
    }

    srand(time(NULL));

    intro(renderer, window, font);
    point_and_click(renderer, window, font);
}