#include "labo/common.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char ** argv){

    //initializes the random number generator 
    time_t t ; 
    time(&t); 
    srand(t);

    return 0 ; 
}