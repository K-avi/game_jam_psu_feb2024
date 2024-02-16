#ifndef MATRIX_H 
#define MATRIX_H 

#include "common.h"

#define DEFAULT_ROW 5 
#define DEFAULT_COL 5 

typedef struct s_matrix{
		
        unsigned row; 
        unsigned col ; 

        unsigned ** matrix ; 
	
}S_MATRIX ; 


#ifdef dev

S_MATRIX * create_matrix(unsigned row, unsigned col); 
void free_matrix(S_MATRIX * matrix); 

#endif

S_MATRIX * generate_matrix(unsigned row, unsigned col, unsigned nb_salles ); 

void fprint_matrix(FILE * flux, S_MATRIX * mat); 


#endif 
