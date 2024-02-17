#include "place_objets.h"
#include "matrix.h"
#include <stdlib.h>

/*
typedef struct obj_infos{
    unsigned i ; 
    unsigned j ; 

    unsigned shift ; 
}OBJ_INFOS;

typedef struct obj_struct{

    OBJ_INFOS * list ; 
    unsigned nb_objects ; 
}OBJ_LIST ; */

static OBJ_LIST * place_objets(S_MATRIX * matrix){
    OBJ_LIST * ret = malloc(sizeof(OBJ_LIST));

    unsigned max_id = 0 ; 

    for(unsigned i = 0 ; i < matrix->row ; i++){
        for(unsigned j = 0 ; j < matrix->col ; j++){
            if(matrix->matrix[i][j] > max_id ){
                max_id = matrix->matrix[i][j] ; 
            }
        }
    }

    unsigned nb_pages = max_id - 2 ; 

    ret->list = calloc(max_id, sizeof(OBJ_INFOS));

    
    
    return ret ; 
}
