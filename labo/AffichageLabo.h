#ifndef AFFICHAGE_LABO_H 
#define AFFICHAGE_LABO_H 
#include <SDL2/SDL.h>


void printMat(SDL_Renderer*renderer, int**mat, int pos_x, int pos_y, int mat_x, int mat_y);
int eventLoop(int*x,int*y);

#endif