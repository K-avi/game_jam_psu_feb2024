#include "matrix.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


//macro pour taille 
//max et min des salles generees
#define ROOM_WIDTH_MIN 2
#define ROOM_WIDTH_MAX 4 

#define ROOM_LENGTH_MIN 2 
#define ROOM_LENGTH_MAX 4 

S_MATRIX * create_matrix(unsigned row, unsigned col){
    /*
    cree une matrice de taille row*col dont 
    les elements sont inities a zero 
    */
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
    /*
    libere le contenu de la matrice mat ainsi que 
    le pointeur de matrice mat
    */
    for(unsigned i = 0 ; i < mat->row ; i++){
        free(mat->matrix[i]);
        mat->matrix[i] = NULL ; 
    }
    free(mat->matrix); 
    mat->matrix = NULL ; 
    free(mat); 
    mat  = NULL ; 
}


//structure de tableau booleen pour les 
//Calculs intermediaires
typedef struct bool_arr{
    unsigned size ; 
    bool * arr; 
}B_ARR;

static unsigned find_candidates(S_MATRIX * matrix, B_ARR * candidates_arr, unsigned room_width, unsigned room_length){
    /*
    algorithme simple : pour chaque case, regarde si la case peut etre le bord superieur 
    gauche d'un rectangle de taille room_width * room_length , 

    met le tableau candidates arr a vrai a l'index i*j si c'est le cas, le met a faux 
    sinon

    renvoie le nombre de candidats trouves
    */
    unsigned nb_candidats = 0 ;
    for(unsigned i = 0 ; i < matrix->row ; i++){
        
        //cas ou l'on a depasse la largeur, on ne va pas trouver de nouveaux rectangles
        if( i + room_width > matrix->row){
            ; //on suppose que le tableau est initialise a zero == faux
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
                    if(!found){
                        candidates_arr->arr[ j + (i*j) ] = true;
                        nb_candidats++; 
                    }
                    //faux si l'on a trouve une case appartennant a un autre polygone, vrai sinon 
                    //permet aux pieces de se coller, modifier si besoin de 
                    //garantie que les pieces ne soient pas collees
                }
            }
        }
    }
    return nb_candidats;
}//ne pas utiliser hors des fonctions ou elle est appelee

static void fill_from(S_MATRIX * matrix, unsigned start_i, unsigned start_j, unsigned val, unsigned width, unsigned length){
    /*
    remplis un rectangle width*len avec la valeur val a partir de l'index mat[start_i][start_j], 
    en considerant que mat[i][j] est le coin superieur gauche du rectangle

    ne verifie RIEN ne se pose PAS de questions
    n'appel PAS memset n'est PAS efficace >:O
    */
    for(unsigned i = start_i; i < start_i + width; i++){
        for (unsigned j = start_j; j < start_j +  length ; j++) {
            matrix->matrix[i][j] = val;
        }
    }
}//ne pas utiliser hors des fonctions ou elle est appelee


static void generate_room(S_MATRIX * matrix, unsigned id_room){

    //room size guess
    unsigned rand_width = (rand()%ROOM_WIDTH_MAX +1) ;
    unsigned room_width = rand_width< ROOM_WIDTH_MIN ? ROOM_WIDTH_MIN : rand_width ; 

    unsigned rand_length = (rand()%ROOM_LENGTH_MAX +1) ;
    unsigned room_length = rand_length< ROOM_LENGTH_MIN ? ROOM_LENGTH_MIN : rand_length ;

    B_ARR candidates_arr ;
    candidates_arr.size = matrix->col * matrix->row ; 
    candidates_arr.arr = calloc(matrix->col * matrix->row, sizeof(unsigned)) ;

    unsigned nb_candidats =  find_candidates(matrix, &candidates_arr , room_width, room_length);

    if(nb_candidats){//evite les divisions par zero

        
        unsigned choisis = rand()%nb_candidats ; //choisis un candidat au hasard
        long long int courant = -1 ; 
        //permet de trouver la case de depart de coloriage 
        //parmis les cases du tableau de candidat; ce code est 
        //vraiment sale jpp
        for(unsigned l = 0 ; l < candidates_arr.size; l++){
            if(candidates_arr.arr[l] == true){
                courant++; 
                if(courant == choisis ){

                    //2 prochaines lignes peut etre fausses
                    int start_i = l/matrix->row;
                    int start_j = l%matrix->col; //surtout elle là

                    fill_from(matrix, start_i, start_j, id_room, room_width, room_length);
                    break; 
                }
            }
        }
    }else{
        fprintf(stderr, "erreur dans generate_room : impossible de generer\n");
    }

    free(candidates_arr.arr);
}//assumes that the room can be fitted 

static void generate_rooms(S_MATRIX * matrix,  unsigned nb_salles){
    /*
    essaie de generer nb_salles dans la matrice matrix de tailles comprises entre 

    ROOM_MIN_WIDTH * ROOM_MIN_LEN  
    et 
    ROOM_MAX_WIDTH * ROOM_MAX_LEN 
    */
    unsigned nb_gen = 0 ; 
    while(nb_gen < nb_salles){
        generate_room(matrix, nb_gen+2);
        nb_gen++; // :)
    }
}//PAS TESTE 

//ufind boolean to connect rooms ; 
//not necessary -> path finding in a matrix is very easy 
typedef struct simple_ufind{
    unsigned size ; 
    unsigned * arr ; //should be size of row*col 
}S_UFIND_ARR ;

static void connect_rooms(S_MATRIX * matrix){

    return;
}//PAS ENCORE FAIT 

S_MATRIX * generate_matrix(unsigned row, unsigned col, unsigned nb_salles){

    S_MATRIX * ret = create_matrix(row, col);

    generate_rooms(ret, nb_salles);
    connect_rooms(ret); 

    return ret ; 

}//PAS ENCORE FAIT 

void fprint_matrix(FILE * flux, S_MATRIX * mat){
    for(unsigned i = 0 ; i < mat->row; i++){
        for(unsigned j = 0 ; j < mat->col ; j++){
            fprintf(flux, "%u", mat->matrix[i][j]); 
        }
        fprintf(flux, "\n");
    }
}
