#include "matrix.h"
#include <stdlib.h>

#define ROOM_SIZE_MIN 4
#define ROOM_SIZE_MAX 9 //macro pour taille 
//max et min des salles generees

S_MATRIX * create_matrix(unsigned row, unsigned col){
    S_MATRIX * ret = malloc(sizeof(S_MATRIX)); 

    //calloc for 0 init 
    ret->matrix = calloc( row, sizeof(unsigned*)); 

    for(unsigned i = 0 ; i < row ; i++ ){
        ret->matrix[i] = calloc(col,  sizeof(unsigned));
    }

    ret->row = row ; 
    ret->col = col ; 

    return ret ; 
}

void free_matrix(S_MATRIX * mat){

    for(unsigned i = 0 ; i < mat->row ; i++){
        free(mat->matrix[i]);
        mat->matrix[i] = NULL ; 
    }
    free(mat->matrix); 
    mat->matrix = NULL ; 
    free(mat); 
    mat  = NULL ; 
}


typedef struct simple_ufind{
    unsigned size ; 
    unsigned * arr ; //should be size of row*col 
}S_UFIND_ARR ;


typedef struct bool_arr{
    unsigned size ; 
    bool * arr; 
}B_ARR;

static void find_candidates(S_MATRIX * matrix, U_ARR * candidates_arr, unsigned room_size){
    /*
    sets the elements of candidates arr to one if they correspond to a squa
    */
}

static void generate_room(S_MATRIX * matrix, unsigned id_room){

    //room size guess
    unsigned rand_size = (rand()%ROOM_SIZE_MAX +1) ;
    unsigned room_size = rand_size < ROOM_SIZE_MIN ? ROOM_SIZE_MIN : rand_size ; 

    U_ARR candidates_arr ;
    candidates_arr.size = matrix->col * matrix->row ; 
    candidates_arr.arr = calloc(matrix->col * matrix->row, sizeof(unsigned)) ;


    free(candidates_arr.arr);
}
//assumes that the room can be fitted 

static void generate_rooms(S_MATRIX * matrix,  unsigned nb_salles){
    
    unsigned nb_gen = 0 ; 
    while(nb_gen < nb_salles){
        generate_room(matrix, nb_gen+2);
        nb_gen++; 
    }
}

static void connect_rooms(S_MATRIX * matrix){

    return;
}

S_MATRIX * generate_matrix(unsigned row, unsigned col, unsigned nb_salles){

    S_MATRIX * ret = create_matrix(row, col);

    generate_rooms(ret, nb_salles);
    connect_rooms(ret); 

    return ret ; 

}
