#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <unistd.h>


#ifndef AFFICHAGE_LABO_H
#include "AffichageLabo.h"
#endif
#ifndef MATRIX_H
#include "matrix.h"
#endif

extern SDL_Renderer*renderer;

S_MATRIX*matrice;

unsigned nb_col;
unsigned nb_row;

int pos_x;
int pos_y;

int pos_col;
int pos_row;

int vx;
int vy;

int dx;
int dy;

void init_laboratoire(unsigned cols, unsigned rows, unsigned salles){
    nb_col = cols;
    nb_row = rows;

    int pos_x = 50;
    int pos_y = 50;

    int pos_col = 5;
    int pos_row = 5;

    vx = 0;
    vy = 0;

    matrice = generate_matrix(nb_row, nb_col, salles);
    create_shadow(renderer);
}
void end_Laboratoire(){
    free_matrix(matrice);
    free_shadow();
}

void laboratoire_loop(){
    int ev=1;;
    while(ev){
        //clear
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        //print on screen
        print_mat(renderer, matrice->matrix, pos_x, pos_y, pos_col, pos_row, nb_col, nb_row);
        print_player(renderer,dx,dy);
        print_shadow(renderer);

        //render
        SDL_RenderPresent(renderer);

        //event
        ev = event_loop(&vx, &vy, &dx, &dy);
        calc_move(&pos_col, &pos_row, &pos_x, &pos_y, vx, vy, nb_col, nb_row, matrice->matrix);
        //if (ev==2){
            //test use event
        //}
    }
}