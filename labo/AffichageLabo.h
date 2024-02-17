#ifndef AFFICHAGE_LABO_H 
#define AFFICHAGE_LABO_H 
#include <SDL2/SDL.h>

void create_shadow(SDL_Renderer*renderer);
void free_shadow();
void print_mat(SDL_Renderer*renderer, unsigned**mat, int pos_x, int pos_y, int col, int row, int mat_x, int mat_y);
void print_player(SDL_Renderer*renderer, int dx, int dy);
void print_shadow(SDL_Renderer*renderer);
int event_loop(int*vx,int*vy,int*dx,int*dy);
void calc_move(int*col,int*row,int*pos_x, int*pos_y, int vx, int vy,int nb_col, int nb_row, unsigned**mat);

#endif