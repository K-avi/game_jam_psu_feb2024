#ifndef COMMON_H
#define COMMON_H 

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <stdbool.h>

#define dev 

#endif


void init_laboratoire(unsigned cols, unsigned rows, unsigned salles);
void end_Laboratoire();
void laboratoire_loop();