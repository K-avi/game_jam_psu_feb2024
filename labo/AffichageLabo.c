#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include "AffichageLabo.h"

#define SIZE_TILE_X 55
#define SIZE_TILE_Y 48

#define WINDOW_X 1850
#define WINDOW_Y 1050

#define TILE_X 35
#define TILE_Y 23

#define Y_SHIFT 10
#define X_SHIFT 2


void printMat(SDL_Renderer*renderer, int**mat, int pos_x, int pos_y, int mat_x, int mat_y){
    int i = pos_y - TILE_Y/2 - 1;
    int j = pos_x - TILE_X/2 - 1;
    
    int i_to = pos_y + TILE_Y/2;
    int j_to = pos_x + TILE_X/2;
    //fprintf(stdout, "%d : %d | %d : %d | %d : %d\n", i, j, i_to, j_to,mat_y, mat_x);
    SDL_Rect r = {-X_SHIFT, -Y_SHIFT, SIZE_TILE_X, SIZE_TILE_Y};
    for (int m=i;m<=i_to; m++) {
        //fprintf(stdout, "\n%d : ",m);
        if (m>=0 && m<mat_y){
            //fprintf(stdout, "in");
            r.x = -X_SHIFT;
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
    SDL_Rect r = {955,535,10,10};
    SDL_RenderFillRect(renderer, &r);
}

void printShadow(SDL_Renderer*renderer){
    
    SDL_Surface*surf = IMG_Load("./darken.png");
    SDL_Texture*text = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect r = {0,0,1920,1080};
    SDL_RenderCopy(renderer,text, NULL,&r);
}


int eventLoop(int*x,int*y){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch (event.type) {
        case SDL_QUIT :
            return 0;
        case SDL_KEYUP :
            switch (event.key.keysym.sym){
            case SDLK_ESCAPE :
                return 0;
            case SDLK_LEFT:
                *x = *x + 1;
                break;
            case SDLK_RIGHT:
                *x = *x - 1;
                break;
            case SDLK_UP:
                *y = *y + 1;
                break;
            case SDLK_DOWN:
                *y = *y - 1;
                break;
            default :
                return 1;
            }
            break;
        default :
            break;
        }
    }
    return -1;
}

