#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <unistd.h>


#include "common.h"

extern const int WINDOW_SIZE_X;
extern const int WINDOW_SIZE_Y;

int main(int argc,char**argv){
    int nb_col, nb_row;
    if (argc < 3){
        fprintf(stdout,"less the 2 argument passed ... Swithcing to default 50 x 50\n");
        nb_col = 25;
        nb_row = 25;
    } else {
        nb_col = atoi(argv[1]);
        nb_row = atoi(argv[2]);
    }
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);
    SDL_Window*w = SDL_CreateWindow("test labo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE_X, WINDOW_SIZE_Y,  SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
    SDL_Renderer*renderer = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    init_laboratoire(renderer, nb_col,nb_row,3, 2);

    laboratoire_loop(renderer);

    //free
    end_Laboratoire();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(w);
    return 0;
}


