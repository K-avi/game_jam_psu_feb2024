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

typedef struct obj_infos{
    unsigned i ; 
    unsigned j ; 

    unsigned shift_x ; 
    unsigned shift_y ; 

    unsigned id; 
}OBJ_INFOS;

typedef struct obj_struct{

    OBJ_INFOS * list ; 
    unsigned nb_objects_max ; 
    unsigned nb_objects_cur ; 
}OBJ_LIST ; 
extern OBJ_LIST global_object_list ;
//I can't justify this tbh 

extern unsigned ID_SMALL_ROOM ; 


typedef struct mat_square{
    unsigned i ; 
    unsigned j ;
} MAT_SQUARE;

extern MAT_SQUARE start_square ; 

#ifdef dev

S_MATRIX * create_matrix(unsigned row, unsigned col); 
void free_matrix(S_MATRIX * matrix); 

#endif

S_MATRIX * generate_matrix(unsigned row, unsigned col, unsigned nb_salles, unsigned nb_petites_salles ); 

void fprint_matrix(FILE * flux, S_MATRIX * mat); 
void fprint_matrix_var(FILE * flux, S_MATRIX * mat);



#endif 
