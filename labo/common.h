#ifndef COMMON_H
#define COMMON_H 

#include <SDL2/SDL_rect.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <stdbool.h>
#include <SDL2/SDL.h>



#define dev 
#endif


void init_laboratoire(unsigned cols, unsigned rows, unsigned salles, unsigned p_salles);
void end_Laboratoire();
void laboratoire_loop();