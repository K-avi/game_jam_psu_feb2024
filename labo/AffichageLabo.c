#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include "AffichageLabo.h"

extern const int WINDOW_SIZE_X;
extern const int WINDOW_SIZE_Y;

const int SIZE_TILE_X = 200;
const int SIZE_TILE_Y = 200;

int TILE_X;
int TILE_Y;

int PLAYER_SIZE_X;
int PLAYER_SIZE_Y;

int X_SHIFT; 
int Y_SHIFT = 55;

const int SPEED_X = 6;
const int SPEED_Y = 6;




SDL_Texture*shadow;
SDL_Texture**l_text_obj;
void init_affichage(){
    TILE_X = (WINDOW_SIZE_X / SIZE_TILE_X) + ((WINDOW_SIZE_X % SIZE_TILE_X)?1:0);
    TILE_Y = (WINDOW_SIZE_Y / SIZE_TILE_Y) + ((WINDOW_SIZE_Y % SIZE_TILE_Y)?1:0);
    PLAYER_SIZE_X = SIZE_TILE_X/5;
    PLAYER_SIZE_Y = SIZE_TILE_Y/5;
    X_SHIFT = (SIZE_TILE_X*(TILE_X-1) - WINDOW_SIZE_X)/2 + PLAYER_SIZE_X/2;
    Y_SHIFT = (SIZE_TILE_Y*(TILE_Y-1) - WINDOW_SIZE_Y)/2 - PLAYER_SIZE_Y/2;
}

void create_shadow(SDL_Renderer*renderer) {
    SDL_Surface*surf = IMG_Load("./darken.png");
    shadow = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}
void free_shadow() {
    SDL_DestroyTexture(shadow);
}
void create_objet_texture(){
    l_text_obj = (SDL_Texture**)malloc(sizeof(SDL_Texture*)*10);
    SDL_Surface*tmp;
    tmp = IMG_Load("");
}
void free_objects(objet*l_obj){
    if (!l_obj)return;
    objet*nxt = l_obj->next;
    free(l_obj);
    free_objects(nxt);
}

void print_mat(SDL_Renderer*renderer, unsigned**mat, int pos_x, int pos_y, int col, int row, int mat_x, int mat_y){
    //start
    int i = row - TILE_Y/2;
    int j = col - TILE_X/2;

    //end
    int i_to = row + TILE_Y/2;
    int j_to = col + TILE_X/2;

    //screen shift
    int y_shift = Y_SHIFT - pos_y - ((TILE_Y%2)?0:SIZE_TILE_Y);
    int x_shift = X_SHIFT - pos_x - ((TILE_X%2)?0:SIZE_TILE_X);

    //rec
    SDL_Rect r = {x_shift+1, y_shift+1, SIZE_TILE_X-2, SIZE_TILE_Y-2};

    //loop matrice y
    for (int m=i-1;m<=i_to+1; m++) {
        if (m>=0 && m<mat_y){ //test indice bon
            //init x a 0
            r.x = x_shift+1;
            
            //loop matrice x
            for (int n=j-1;n<=j_to+1; n++) {
                if (n>=0 && n<mat_x) { //test indice bon

                    //couleur
                    switch (mat[m][n]) {
                        case 0:
                            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                            break;
                        case 1:
                            SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);
                            break;
                        case 2:
                            SDL_SetRenderDrawColor(renderer, 255,0,0,100);
                            break;
                        case 3:
                            SDL_SetRenderDrawColor(renderer, 0,255,0,100);
                            break;
                        case 4:
                            SDL_SetRenderDrawColor(renderer, 0,0,255,100);
                            break;
                        default:
                            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
                            break;
                    }
                    //show
                    SDL_RenderFillRect(renderer, &r);
                }
                r.x += SIZE_TILE_X;
            }
        }
        r.y += SIZE_TILE_Y;
    }
}

void print_objet(SDL_Renderer*renderer, objet*l_obj, int pos_x, int pos_y, int col, int row){
    if (!l_obj) return;
    int test = ((col - TILE_X/2 - 1) <= l_obj->col) &&
        ((col + TILE_X/2) >= l_obj->col) &&
        ((row - TILE_Y/2 - 1) <= l_obj->row) &&
        ((row + TILE_Y/2) >= l_obj->row);
    if (test) {
        int x = (l_obj->col + TILE_X/2 - col)*SIZE_TILE_X + l_obj->x - pos_x; 
        int y = (l_obj->row + TILE_Y/2 - row)*SIZE_TILE_Y + l_obj->y - pos_y;
        SDL_Rect r = {x, y, l_obj->w, l_obj->h};
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &r);
    }
    print_objet(renderer, l_obj->next, pos_x, pos_y, col, row);
}

void print_player(SDL_Renderer*renderer, int dx, int dy){
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect r = {(WINDOW_SIZE_X - PLAYER_SIZE_X)/2 ,(WINDOW_SIZE_Y - PLAYER_SIZE_Y)/2,PLAYER_SIZE_X,PLAYER_SIZE_Y};
    SDL_RenderFillRect(renderer, &r);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderDrawLine(renderer,WINDOW_SIZE_X/2,WINDOW_SIZE_Y/2,WINDOW_SIZE_X/2+dx*PLAYER_SIZE_X,WINDOW_SIZE_Y/2+dy*PLAYER_SIZE_Y);
}

void print_shadow(SDL_Renderer*renderer){
    SDL_Rect r = {0,0,WINDOW_SIZE_X,WINDOW_SIZE_Y};
    SDL_RenderCopy(renderer,shadow, NULL,&r);
}


int event_loop(int*vx,int*vy,int*dx,int*dy){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch (event.type) {
        case SDL_QUIT :
            return 0;
        case SDL_KEYDOWN :
            switch (event.key.keysym.sym){
            case SDLK_ESCAPE :
                return 0;
            case SDLK_SPACE:
                return 2;
            case SDLK_LEFT:
                *vx = -1;
                *dx = -1;
                *dy = (*vy)?*dy:0;
                break;
            case SDLK_RIGHT:
                *vx = 1;
                *dx = 1;
                *dy = (*vy)?*dy:0;
                break;
            case SDLK_UP:
                *vy = -1;
                *dy = -1;
                *dx = (*vx)?*dx:0;
                break;
            case SDLK_DOWN:
                *vy = 1;
                *dy = 1;
                *dx = (*vx)?*dx:0;
                break;
            }
            return 1;
        case SDL_KEYUP :
            switch (event.key.keysym.sym){
            case SDLK_ESCAPE :
                return 0;
            case SDLK_LEFT:
                *vx = (*vx==-1)?0:*vx;
                *dx = (*dy)?0:*dx;
                break;
            case SDLK_RIGHT:
                *vx = (*vx==1)?0:*vx;
                *dx = (*dy)?0:*dx;
                break;
            case SDLK_UP:
                *vy = (*vy==-1)?0:*vy;
                *dy = (*dx)?0:*dy;
                break;
            case SDLK_DOWN:
                *vy = (*vy==1)?0:*vy;
                *dy = (*dx)?0:*dy;
                break;
            }
            return 1;
        default :
            break;
        }
    }
    return -1;
}

void calc_move(int*col,int*row,int*pos_x, int*pos_y, int vx, int vy,int nb_col, int nb_row, unsigned**mat, objet*l_obj){
    ///*
    int nx = *pos_x;
    int ny = *pos_y;
    int ncol = *col;
    int nrow = *row;
    if (vx){
        nx += (vy)?vx*SPEED_X*2/3:vx*SPEED_X;
        if (nx<0){
            if (ncol-1>=0 && mat[nrow][ncol-1]!=0){
                nx += SIZE_TILE_X;
                ncol --;
            } else {
                nx = 0; //change to fit player
            }
            goto mv_next1;
        }
        if (nx>SIZE_TILE_X){
            if (ncol+1 < nb_col && mat[nrow][ncol+1]!=0){
                nx = nx - SIZE_TILE_X+2;
                ncol ++;
            } else {
                nx = SIZE_TILE_X;
            }
        }
    }
    mv_next1:
    *pos_x = nx;
    *col = ncol;
    if (vy){
        ny += (vx)?vy*SPEED_Y*2/3:vy*SPEED_Y;
        if (ny<0){
            if (nrow-1>=0 && mat[nrow-1][ncol]!=0){
                ny += SIZE_TILE_Y;
                nrow --;
            } else {
                ny = 0; //change to fit player
            }
            goto mv_next2;
        }
        if (ny>SIZE_TILE_Y){
            if (nrow+1 < nb_row && mat[nrow+1][ncol]!=0){
                ny -= SIZE_TILE_Y;
                nrow ++;
            } else {
                ny = SIZE_TILE_Y;
            }
        }
    }
    mv_next2:
    *pos_y = ny;
    *row = nrow;
}

/*
int test_use(unsigned**matrice, int p_col, int p_row, int dx, int dy) {

    return 0;
}*/