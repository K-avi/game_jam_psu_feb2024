#ifndef AFFICHAGE_LABO_H 
#define AFFICHAGE_LABO_H 
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include "../timer/timer.h"

void create_shadow();
void free_shadow();
void init_affichage(SDL_Renderer*rend);
void create_objects();
void free_objects();
void print_mat(unsigned**mat, int pos_x, int pos_y, int col, int row, int mat_x, int mat_y);
void print_objet(int pos_x, int pos_y, int col, int row);
void print_player(int dx, int dy);

void print_shadow();

int event_loop(int*vx,int*vy,int*dx,int*dy);
void calc_move(int*col,int*row,int*pos_x, int*pos_y, int vx, int vy,int nb_col, int nb_row, unsigned**mat);
int test_use(int p_col, int p_row, int*recip,int nb_recip,Timer*timer);
void test();
#endif

/*
0 - plan de travail
1 - start
2 - burreau avec recette
3 - distributeur a eau plein
4 - distributeur a eau vide
*/