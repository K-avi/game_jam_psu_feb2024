#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include "AffichageLabo.h"

#define SIZE_TILE_X 105
#define SIZE_TILE_Y 100

#define TILE_X 19
#define TILE_Y 11

#define X_SHIFT 37
#define Y_SHIFT 10

#define SPEED_X 2
#define SPEED_Y 2

SDL_Texture*shadow;

void createShadow(SDL_Renderer*renderer){
    SDL_Surface*surf = IMG_Load("./darken.png");
    shadow = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}
void freeShadow(){
    SDL_DestroyTexture(shadow);
}

void printMat(SDL_Renderer*renderer, unsigned**mat, int pos_x, int pos_y, int col, int row, int mat_x, int mat_y){
    int i = row - TILE_Y/2 - 1;
    int j = col - TILE_X/2 - 1;

    int y_shift = Y_SHIFT - pos_y;
    int x_shift = X_SHIFT - pos_x;
    
    int i_to = row + TILE_Y/2;
    int j_to = col + TILE_X/2;
    //fprintf(stdout, "%d : %d | %d : %d | %d : %d\n", i, j, i_to, j_to,mat_y, mat_x);
    SDL_Rect r = {x_shift, y_shift, SIZE_TILE_X-2, SIZE_TILE_Y-2};
    for (int m=i;m<=i_to; m++) {
        //fprintf(stdout, "\n%d : ",m);
        if (m>=0 && m<mat_y){
            //fprintf(stdout, "in");
            r.x = x_shift;
            for (int n=j;n<=j_to; n++) {
                //fprintf(stdout, "%d : %d\n",m,n);
                if (n>=0 && n<mat_x) {
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
                    SDL_RenderFillRect(renderer, &r);
                }
                r.x += SIZE_TILE_X;
            }
        }
        r.y += SIZE_TILE_Y;
    }
}

void printPlayer(SDL_Renderer*renderer){
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect r = {950,530,20,20};
    SDL_RenderFillRect(renderer, &r);
}

void printShadow(SDL_Renderer*renderer){
    SDL_Rect r = {0,0,1920,1080};
    SDL_RenderCopy(renderer,shadow, NULL,&r);
}


int eventLoop(int*vx,int*vy){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch (event.type) {
        case SDL_QUIT :
            return 0;
        case SDL_KEYDOWN :
            switch (event.key.keysym.sym){
            case SDLK_ESCAPE :
                return 0;
            case SDLK_LEFT:
                *vx = -1;
                break;
            case SDLK_RIGHT:
                *vx = 1;
                break;
            case SDLK_UP:
                *vy = -1;
                break;
            case SDLK_DOWN:
                *vy = 1;
                break;
            }
            return 1;
        case SDL_KEYUP :
            switch (event.key.keysym.sym){
            case SDLK_ESCAPE :
                return 0;
            case SDLK_LEFT:
                *vx = 0;
                break;
            case SDLK_RIGHT:
                *vx = 0;
                break;
            case SDLK_UP:
                *vy = 0;
                break;
            case SDLK_DOWN:
                *vy = 0;
                break;
            }
            return 1;
        default :
            break;
        }
    }
    return -1;
}

void calcMove(int*col,int*row,int*pos_x, int*pos_y, int vx, int vy,int nb_col, int nb_row, unsigned**mat){
    
    
    if (vx) {
        *pos_x += vx*SPEED_X;
        if (*pos_x<0) {
            *pos_x = *pos_x+SIZE_TILE_X;
            *col = (*col)?
                (*col-1):
                0;
        }
        if (*pos_x>SIZE_TILE_X) {
            *pos_x = *pos_x-SIZE_TILE_X;
            *col = (*col)?
                (*col+1):
                *col;
        }
    }
    if (vy) {
        *pos_y += vy*SPEED_Y;
        if (*pos_y<0) {
            *pos_y = *pos_y+SIZE_TILE_Y;
            *row = (*row)?
                (*row-1):
                0;
        }
        if (*pos_y>SIZE_TILE_Y) {
            *pos_y = *pos_y-SIZE_TILE_Y;
            *row = (*row)?
                (*row+1):
                *row;
        }
    }
}