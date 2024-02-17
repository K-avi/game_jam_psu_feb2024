#include "labo/matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char ** argv){

    time_t t; 
    time(&t);
    srand(t);

    S_MATRIX * mat =  generate_matrix(20, 20,5,2);
    fprint_matrix(stdout, mat);
    fprintf(stdout, "\n-------\n");

    fprint_matrix_var(stdout, mat);

    free_matrix(mat);
    return 0; 
}
