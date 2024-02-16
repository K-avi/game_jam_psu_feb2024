#include "matrix.h"
#include <stdbool.h>
#include <stdlib.h>


//macro pour taille 
//max et min des salles generees
#define ROOM_WIDTH_MIN 2
#define ROOM_WIDTH_MAX 4 

#define ROOM_LENGTH_MIN 2 
#define ROOM_LENGTH_MAX 4 

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





typedef struct bool_arr{
    unsigned size ; 
    bool * arr; 
}B_ARR;

static void find_candidates(S_MATRIX * matrix, B_ARR * candidates_arr, unsigned room_width, unsigned room_length){
    /*
    algorithme simple : pour chaque case, regarde si la case peut etre le bord superieur 
    gauche d'un rectangle de taille room_width * room_length , 

    met le tableau candidates arr a vrai a l'index i*j si c'est le cas, le met a faux 
    sinon
    */
    for(unsigned i = 0 ; i < matrix->row ; i++){
        
        //cas ou l'on a depasse la largeur, on ne va pas trouver de nouveaux rectangles
        if( i + room_width > matrix->row){
               ;
        }else{
            for(unsigned j = 0 ; j < matrix->col ; j++){
                //cas ou l'on depasse la longueur, on ne va pas trouver de nouveaux rectangles
                if(j+room_length > matrix->col){
                    candidates_arr->arr[ j + (i*j) ] = false ; 

                }else{//on doit verifier que les cases ne sont pas occupees par un autre rectangle
                    
                    bool found = false ; 
                    for(unsigned k = i ; k < i + room_width ; k++){
                        for(unsigned l = j ; l < j + room_length ; l++ ){
                            if(matrix->matrix[k][l]!=0){
                                found = true ; 
                            }
                        }
                    }
                    candidates_arr->arr[ j + (i*j) ] = !found ; 
                    //faux si l'on a trouve une case appartennant a un autre polygone, vrai sinon 
                }
            }
        }
    }
}


static void generate_room(S_MATRIX * matrix, unsigned id_room){

    //room size guess
    unsigned rand_width = (rand()%ROOM_WIDTH_MAX +1) ;
    unsigned room_width = rand_width< ROOM_WIDTH_MIN ? ROOM_WIDTH_MIN : rand_width ; 

    unsigned rand_length = (rand()%ROOM_LENGTH_MAX +1) ;
    unsigned room_length = rand_length< ROOM_LENGTH_MIN ? ROOM_LENGTH_MIN : rand_length ;

    B_ARR candidates_arr ;
    candidates_arr.size = matrix->col * matrix->row ; 
    candidates_arr.arr = calloc(matrix->col * matrix->row, sizeof(unsigned)) ;

    find_candidates(matrix, &candidates_arr , room_width, room_length);

    free(candidates_arr.arr);
}//assumes that the room can be fitted 

static void generate_rooms(S_MATRIX * matrix,  unsigned nb_salles){
    
    unsigned nb_gen = 0 ; 
    while(nb_gen < nb_salles){
        generate_room(matrix, nb_gen+2);
        nb_gen++; 
    }
}


//ufind boolean to connect rooms ; 
//not necessary -> path finding in a matrix is very easy 
typedef struct simple_ufind{
    unsigned size ; 
    unsigned * arr ; //should be size of row*col 
}S_UFIND_ARR ;

static void connect_rooms(S_MATRIX * matrix){

    return;
}

S_MATRIX * generate_matrix(unsigned row, unsigned col, unsigned nb_salles){

    S_MATRIX * ret = create_matrix(row, col);

    generate_rooms(ret, nb_salles);
    connect_rooms(ret); 

    return ret ; 

}
