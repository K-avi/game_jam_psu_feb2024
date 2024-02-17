#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <unistd.h>


#include "common.h"
#ifndef AFFICHAGE_LABO_H
#include "AffichageLabo.h"
#endif
#ifndef MATRIX_H
#include "matrix.h"
#endif


SDL_Renderer*renderer;

int main(int argc,char*argv[]){
    int nb_col, nb_row;
    if (argc < 3){
        fprintf(stdout,"less the 2 argument passed ... Swithcing to default 50 x 50\n");
        nb_col = 50;
        nb_row = 50;
    } else {
        nb_col = atoi(argv[1]);
        nb_row = atoi(argv[2]);
    }

    SDL_Window*w = SDL_CreateWindow("test labo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1850, 1050, SDL_WINDOW_SHOWN || SDL_WINDOW_BORDERLESS);
    renderer = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);

    init_laboratoire(nb_col,nb_row,5);

    laboratoire_loop();

    //free
    end_Laboratoire();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(w);
    return 0;
}


