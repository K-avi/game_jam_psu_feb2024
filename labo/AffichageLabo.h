#ifndef AFFICHAGE_LABO_H 
#define AFFICHAGE_LABO_H 
#include <SDL2/SDL.h>

void createShadow(SDL_Renderer*renderer);
void freeShadow();
void printMat(SDL_Renderer*renderer, unsigned**mat, int pos_x, int pos_y, int col, int row, int mat_x, int mat_y);
void printPlayer(SDL_Renderer*renderer);
void printShadow(SDL_Renderer*renderer);
int eventLoop(int*vx,int*vy);
void calcMove(int*col,int*row,int*pos_x, int*pos_y, int vx, int vy,int nb_col, int nb_row, unsigned**mat);

#endif