#ifndef AFFICHAGE_LABO_H 
#define AFFICHAGE_LABO_H 
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

typedef struct objet_t{
    int x, y;
    int col, row;
    unsigned id;
    unsigned w, h;
    struct objet_t*next;
} objet;

void create_shadow(SDL_Renderer*renderer);
void free_shadow();
void init_affichage();
objet*create_objects(SDL_Renderer*renderer);
void free_objects(objet*l_obj);
void print_mat(SDL_Renderer*renderer, unsigned**mat, int pos_x, int pos_y, int col, int row, int mat_x, int mat_y);
void print_objet(SDL_Renderer*renderer, objet*l_obj, int pos_x, int pos_y, int col, int row);
void print_player(SDL_Renderer*renderer, int dx, int dy);

void print_shadow(SDL_Renderer*renderer);

int event_loop(int*vx,int*vy,int*dx,int*dy);
void calc_move(int*col,int*row,int*pos_x, int*pos_y, int vx, int vy,int nb_col, int nb_row, unsigned**mat, objet*l_obj);
int test_use(unsigned**matrice, int p_col, int p_row, int dx, int dy, objet*l_obj);
#endif