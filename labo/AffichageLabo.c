#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include "AffichageLabo.h"
#include "matrix.h"
#include "../timer/timer.h"

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

const int SPEED_X = 12;
const int SPEED_Y = 12;

extern OBJ_LIST global_object_list ;

SDL_Renderer*renderer;
SDL_Texture*shadow;

typedef struct obj_t{
    int w, h;
    SDL_Texture*texture;
}obj;
obj*l_obj;

int player_cpt = 0;
int player_move = 0;
double rot=0;
SDL_Texture*player_still;
SDL_Texture*player_walk;

void init_affichage(SDL_Renderer*rend){
    renderer = rend;
    TILE_X = (WINDOW_SIZE_X / SIZE_TILE_X) + ((WINDOW_SIZE_X % SIZE_TILE_X)?1:0);
    TILE_Y = (WINDOW_SIZE_Y / SIZE_TILE_Y) + ((WINDOW_SIZE_Y % SIZE_TILE_Y)?1:0);
    PLAYER_SIZE_X = SIZE_TILE_X/4;
    PLAYER_SIZE_Y = SIZE_TILE_Y/4;
    X_SHIFT = (SIZE_TILE_X*(TILE_X-1) - WINDOW_SIZE_X)/2 + PLAYER_SIZE_X/2;
    Y_SHIFT = (SIZE_TILE_Y*(TILE_Y-1) - WINDOW_SIZE_Y)/2 - PLAYER_SIZE_Y/2;
}

void create_shadow() {
    SDL_Surface*surf = IMG_Load("./darken.png");
    shadow = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}
void free_shadow() {
    SDL_DestroyTexture(shadow);
}




void create_objects(){
    l_obj = (obj*)malloc(sizeof(obj)*6);
    SDL_Surface*surf;
    surf = IMG_Load("./asset/coin_expe.png");
    l_obj[0].texture = SDL_CreateTextureFromSurface(renderer,surf);
    l_obj[0].w = 390;
    l_obj[0].h = 250;
    SDL_FreeSurface(surf);
    surf = IMG_Load("./asset/bureau_debut.png");
    l_obj[1].texture = SDL_CreateTextureFromSurface(renderer,surf);
    l_obj[1].w = 200;
    l_obj[1].h = 150;
    SDL_FreeSurface(surf);
    surf = IMG_Load("./asset/bureau_recette.png");
    l_obj[2].texture = SDL_CreateTextureFromSurface(renderer,surf);
    l_obj[2].w = 100;
    l_obj[2].h = 200;
    SDL_FreeSurface(surf);
    surf = IMG_Load("./asset/eau_remplie.png");
    l_obj[3].texture = SDL_CreateTextureFromSurface(renderer,surf);
    l_obj[3].w = 100;
    l_obj[3].h = 100;
    SDL_FreeSurface(surf);
    surf = IMG_Load("./asset/eau_vide.png");
    l_obj[4].texture = SDL_CreateTextureFromSurface(renderer,surf);
    l_obj[4].w = 100;
    l_obj[4].h = 100;
    SDL_FreeSurface(surf);
    surf = IMG_Load("./asset/bureau_recette_vide.png");
    l_obj[5].texture = SDL_CreateTextureFromSurface(renderer,surf);
    l_obj[5].w = 100;
    l_obj[5].h = 200;
    SDL_FreeSurface(surf);
    surf = IMG_Load("./asset/perso_debout.png");
    player_still = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    surf = IMG_Load("./asset/perso_marche.png");
    player_walk = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    
}
void free_objects(){
    for(int i=0;i<6;i++){
        SDL_DestroyTexture(l_obj[i].texture);
    }
    free(l_obj);
}

void print_mat(unsigned**mat, int pos_x, int pos_y, int col, int row, int mat_x, int mat_y){
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
                        default:
                            SDL_SetRenderDrawColor(renderer, 175, 175, 175, 255);
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

void print_objet(int pos_x, int pos_y, int col, int row){
    for (int m=0; m<global_object_list.nb_objects_cur;m++){
        OBJ_INFOS obj = global_object_list.list[m];
        int x = (obj.j + TILE_X/2 - col)*SIZE_TILE_X + obj.shift_x - pos_x; 
        int y = (obj.i + TILE_Y/2 - row)*SIZE_TILE_Y + obj.shift_y - pos_y;
        SDL_Rect r = {x, y, l_obj[obj.id].w, l_obj[obj.id].h};
        SDL_RenderFillRect(renderer, &r);
        SDL_RenderCopy(renderer, l_obj[obj.id].texture,NULL, &r);
    }
}

void print_player(int dx, int dy, int vx, int vy){
    SDL_Rect r = {(WINDOW_SIZE_X - PLAYER_SIZE_X)/2 ,(WINDOW_SIZE_Y - PLAYER_SIZE_Y)/2,PLAYER_SIZE_X,PLAYER_SIZE_Y};
    //SDL_RenderCopy(renderer, (player_cpt%20>10)?player_walk:player_still, NULL, &r);
    player_cpt++;
    if (vx==1){
        rot = vy?
            (vy==1?225:135):
            180;
    }else{
        if (vx==-1) {
            rot = vy?
                (vy==1?315:45):
                0;
        } else {
            rot = vy?
                (vy==1?270:90):
                rot;
        }
    }
    SDL_Point p = {PLAYER_SIZE_X/2,PLAYER_SIZE_Y/2};
    SDL_RenderCopyEx(renderer, (player_move)?player_walk:player_still, NULL,&r, rot , &p, (player_cpt%30>15)?SDL_FLIP_VERTICAL:SDL_FLIP_NONE);
}

void print_shadow(){
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
                player_move=1;
                break;
            case SDLK_RIGHT:
                *vx = 1;
                *dx = 1;
                *dy = (*vy)?*dy:0;
                player_move=1;
                break;
            case SDLK_UP:
                *vy = -1;
                *dy = -1;
                *dx = (*vx)?*dx:0;
                player_move=1;
                break;
            case SDLK_DOWN:
                *vy = 1;
                *dy = 1;
                *dx = (*vx)?*dx:0;
                player_move=1;
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
                player_move = (*vx || *vy)?1:0;
                break;
            case SDLK_RIGHT:
                *vx = (*vx==1)?0:*vx;
                *dx = (*dy)?0:*dx;
                player_move = (*vx || *vy)?1:0;
                break;
            case SDLK_UP:
                *vy = (*vy==-1)?0:*vy;
                *dy = (*dx)?0:*dy;
                player_move = (*vx || *vy)?1:0;
                break;
            case SDLK_DOWN:
                *vy = (*vy==1)?0:*vy;
                *dy = (*dx)?0:*dy;
                player_move = (*vx || *vy)?1:0;
                break;
            }
            return 1;
        default :
            break;
        }
    }
    return -1;
}

void calc_move(int*col,int*row,int*pos_x, int*pos_y, int vx, int vy,int nb_col, int nb_row, unsigned**mat){
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


int test_use(int p_col, int p_row,int*recip,int nb_recip,Timer*timer) {
    for (int m=0; m<global_object_list.nb_objects_cur;m++){
        OBJ_INFOS obj = global_object_list.list[m];
        for(int a=-1;a<2;a++){
            for(int b=-1;b<2;b++){
                if (p_row+a==obj.i && p_col+b==obj.j){
                    switch (obj.id){
                        case 0:
                            if (*recip>=nb_recip) return 1;
                            break;
                        case 2:
                            *recip = *recip + 1;
                            global_object_list.list[m].id = 5;
                            break;
                        case 3:
                            global_object_list.list[m].id = 4;
                            timer->current_time = (timer->current_time-10<0?0:timer->current_time-10);
                            break;
                    }
                    return 0;
                }
            }
        }
    }
    return 0;
}
