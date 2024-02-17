#include "matrix.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#ifndef SIZE_TILE_X 
#define SIZE_TILE_X 200 
#endif 

#ifndef SIZE_TILE_Y 
#define SIZE_TILE_Y 200 
#endif 


//macro pour taille 
//max et min des salles generees
#define ROOM_WIDTH_MIN 3
#define ROOM_WIDTH_MAX 5

#define ROOM_LENGTH_MIN 3 
#define ROOM_LENGTH_MAX 5


OBJ_LIST global_object_list ; 
unsigned ID_SMALL_ROOM ; 


MAT_SQUARE start_square ; 
/*
structure "statique" du fichier, definis les cases ou les routes vont aboutir pour 
relier les pieces de manieres coherentes; la structure est interne et ne devrait pas 
etre utilisee hors des fonctions internes
*/


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
                        candidates_arr->arr[(i*matrix->col) + j ] = true;
                        //printf("nb_candidats=%u\n", nb_candidats);
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
           //printf("i=%u, j=%u\n",i,j);
            matrix->matrix[i][j] = val;
        }
    }
}//ne pas utiliser hors des fonctions ou elle est appelee

static void place_object(OBJ_LIST * list_obj, unsigned i, unsigned j, unsigned room_widht, unsigned room_length, unsigned id){
    

    if(id == 2){
        OBJ_INFOS * obj = &(list_obj->list[list_obj->nb_objects_cur]);
        obj->i = i ; 
        obj->j = j ; 
        obj->shift_x = 0 ; 
        obj->shift_y = 0 ; 
        obj->id = 0; 
        list_obj->nb_objects_cur ++ ; 
    }else if(id == 3){
        OBJ_INFOS * obj = &(list_obj->list[list_obj->nb_objects_cur]);
        
        while(obj->i == start_square.i && obj->j == start_square.j){
            obj->i = (rand()%room_widht) + i ; 
            obj->j = (rand()%room_length) + j ; 
        }

        obj->shift_x = 0;
        obj->shift_y = 0;

        obj->id = 1 ; 

        list_obj->nb_objects_cur ++ ; 
    }else if(id == 255 ){
        OBJ_INFOS * obj = &(list_obj->list[list_obj->nb_objects_cur]);
        obj->i = (rand()%room_widht) + i ; 
        obj->j = (rand()%room_length) + j ; 

        obj->shift_x = 0;
        obj->shift_y = 0;

        obj->id = 3 ; 

        list_obj->nb_objects_cur ++ ; 
    }else {
        OBJ_INFOS * obj = &(list_obj->list[list_obj->nb_objects_cur]);
        obj->i = (rand()%room_widht) + i ; 
        obj->j = (rand()%room_length) + j ; 

        obj->shift_x = 0;
        obj->shift_y = 0;

        obj->id = 2 ; 

        list_obj->nb_objects_cur ++ ; 
    }
}//i'm discovering new lows when it comes to writing code

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
    //printf("nbcandidats=%u\n", nb_candidats);
    if(nb_candidats){//evite les divisions par zero

        
        unsigned choisis = rand()%nb_candidats ; //choisis un candidat au hasard
        long long int courant = -1 ; 
        //permet de trouver la case de depart de coloriage 
        //parmis les cases du tableau de candidat; ce code est 
        //vraiment sale jpp

        bool appele = 0 ; 

        for(unsigned i = 0 ; i < matrix->row; i++){
            for(unsigned j = 0 ; j < matrix->col; j++){
                if( candidates_arr.arr[ (i * matrix->col) + j ] != 0 ){
                    courant ++ ; 
                    if(courant == choisis){
                        //printf("appel a fill_from i=%u\n",id_room);
                        appele = true ; 
                        fill_from(matrix, i, j, id_room, room_width, room_length);            
                        place_object( &global_object_list, i,j, room_width, room_length, id_room);
                        if(id_room==2){
                            start_square.i = i ; 
                            start_square.j = j ; 
                        }
                        goto end_loop;
                    }
                }
            }
        }
        if(!appele){
            for(unsigned i = 0 ; i < matrix->row; i++){
                for(unsigned j = 0 ; j < matrix->col; j++){
                    if( candidates_arr.arr[ (i * matrix->col) + j ] != 0 ){
                        fill_from(matrix, i, j, id_room, room_width, room_length);            
                        place_object( &global_object_list, i,j, room_width, room_length, id_room);
                        if(id_room==2){
                            start_square.i = i ; 
                            start_square.j = j ; 
                        }
                        goto end_loop; 
                    }
                }
            }
        }
        end_loop: ;
    
    }else{
        fprintf(stderr, "erreur dans generate_room : impossible de generer\n");
    }
    free(candidates_arr.arr);
}//assumes that the room can be fitted 

static void generate_room_small(S_MATRIX * matrix, unsigned id_room, MAT_SQUARE * petites_salles_ref){

    //room size guess
  
    unsigned room_width = ROOM_WIDTH_MIN ;
    unsigned room_length = ROOM_LENGTH_MIN ;

    B_ARR candidates_arr ;
    candidates_arr.size = matrix->col * matrix->row ; 
    candidates_arr.arr = calloc(matrix->col * matrix->row, sizeof(unsigned)) ;

    unsigned nb_candidats =  find_candidates(matrix, &candidates_arr , room_width, room_length);
    //printf("nbcandidats=%u\n", nb_candidats);
    if(nb_candidats){//evite les divisions par zer 
        unsigned choisis = rand()%nb_candidats ; //choisis un candidat au hasard
        long long int courant = -1 ; 
        //permet de trouver la case de depart de coloriage 
        //parmis les cases du tableau de candidat; ce code est 
        //vraiment sale jpp
        bool appele = 0 ; 

        for(unsigned i = 0 ; i < matrix->row; i++){
            for(unsigned j = 0 ; j < matrix->col; j++){
                if( candidates_arr.arr[ (i * matrix->col) + j ] != 0 ){
                    courant ++ ; 
                    if(courant == choisis){
                       // printf("appel a fill_from i=%u\n",id_room);
                        appele = true ; 
                        petites_salles_ref->i = i ; 
                        petites_salles_ref->j = j ; 
                        fill_from(matrix, i, j, id_room, room_width, room_length);            
                        place_object( &global_object_list, i,j, room_width, room_length, 255);

                        goto end_loop;
                    }
                }
            }
        }
        if(!appele){
            for(unsigned i = 0 ; i < matrix->row; i++){
                for(unsigned j = 0 ; j < matrix->col; j++){
                    if( candidates_arr.arr[ (i * matrix->col) + j ] != 0 ){
                        petites_salles_ref->i = i ; 
                        petites_salles_ref->j = j ; 
                        fill_from(matrix, i, j, id_room, room_width, room_length);            
                        place_object( &global_object_list, i,j, room_width, room_length, 255);

                        goto end_loop; 
                    }
                }
            }
        }
        end_loop: ;    
    }else{
        fprintf(stderr, "erreur dans generate_room : impossible de generer\n");
    }
    free(candidates_arr.arr);
}//assumes that the room can be fitted 

static void generate_rooms(S_MATRIX * matrix,  unsigned nb_salles, unsigned nb_smalls, MAT_SQUARE * petites_salles_arr ){
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

    unsigned nb_gen_small = 0 ;
    while (nb_gen_small < nb_smalls) {
        generate_room_small(matrix, nb_gen+2+nb_gen_small, petites_salles_arr+nb_gen_small);
        nb_gen_small++; 
    }
}//teste : fonctionne; comportement potentiellement a revoir


typedef struct room_connection_squares{
    unsigned identifier ; //le coin en haut a gauche d'une 
    //salle sert d'identifiant
    MAT_SQUARE north_square ;
    MAT_SQUARE south_square ; 
    MAT_SQUARE east_square ; 
    MAT_SQUARE west_square ; 

    MAT_SQUARE start_square ;
}S_ROOM_CS; 
/*
tableau des cases de connections des 
rectangles d'une matrice. Ne pas utiliser hors des 
fonctions qui s'en servent deja
*/
typedef struct room_connection_squares_tab{
    S_ROOM_CS * rcs_tab; 
    unsigned size; 
}S_RCS_TAB; 



static void fill_rcs_tab(S_MATRIX * matrix, S_RCS_TAB * rcstab){
    /*
    remplis rcstab avec les rectangles de matrix , dans l'ordre de l'id le plus 
    petit au plus grand
    */

    for(unsigned i = 0 ; i < matrix->row ; i++){
        for(unsigned j = 0 ; j < matrix->col ; j++){
            if(matrix->matrix[i][j] > 1){
                
                unsigned identifier = matrix->matrix[i][j] - 2 ; 
                //printf("wtf\n");
                if(rcstab->rcs_tab[identifier].identifier == 0 ){
                    rcstab->rcs_tab[identifier].identifier = matrix->matrix[i][j];

                    unsigned length = 0 ; 
                    unsigned width = 0 ; 

                    unsigned tmp_j = j ; 
                    //printf("i=%u, tmp_j=%u\n",i,tmp_j);
                    while(matrix->matrix[i][tmp_j] == rcstab->rcs_tab[identifier].identifier){
                       
                        tmp_j++; 
                        length++;
                        if(tmp_j == matrix->col) break;
                    }
                    unsigned tmp_i = i ;
                    while (matrix->matrix[tmp_i][j] == rcstab->rcs_tab[identifier].identifier ) {
                        tmp_i ++; 
                        width ++ ;
                        if(tmp_i == matrix->row) break;
                    }
                    //sets it's connection using north west south east instead of up down left right is 
                    //ambitious tbh
                    rcstab->rcs_tab[identifier].start_square = (MAT_SQUARE){i,j};
                    rcstab->rcs_tab[identifier].north_square = (MAT_SQUARE){i, j+ length/2};
                    rcstab->rcs_tab[identifier].south_square = (MAT_SQUARE){i + width , j+ length/2};
                    rcstab->rcs_tab[identifier].west_square = (MAT_SQUARE){i+width/2, j};
                    rcstab->rcs_tab[identifier].east_square = (MAT_SQUARE){i+width/2, j  + length};
                }
            }
        }
    }

}//encore une fois ne pas utiliser hors de son appel etc
//pas teste

static S_RCS_TAB * create_rcs_tab(S_MATRIX * matrix){
    /*
    matrix -> initialise, pas de sales geja generees (sinon probleme)

    cree un tableau contenant des informations (valeur d'identifiant, position du 
    coin superieur gauche,..) sur chaque rectangle de la matrice 
    */
    S_RCS_TAB * rcstab = malloc(sizeof(S_RCS_TAB));
    unsigned max = 0 ; //trouver max - 1 permet de determiner le nb de rectangles
    for(unsigned i = 0 ; i < matrix->row; i++){
        for(unsigned j = 0 ; j < matrix->col ; j++){
            if(matrix->matrix[i][j] > max ){
                max = matrix->matrix[i][j] ;
            }
        }
    }
    if(max > 1){
        rcstab->size = max - 1 ; 
        rcstab->rcs_tab = calloc(max - 1, sizeof(S_ROOM_CS));

        for(unsigned i = 0 ; i < rcstab->size; i++){
            rcstab->rcs_tab[i].identifier = 0 ; 
        }
        fill_rcs_tab(matrix, rcstab);

        return rcstab;
    }else{
        fprintf(stderr, "create_rcs_tab : erreur pas de polygone dans mat\n");
        free(rcstab); 
        return NULL;
    }

}//giga casse gueule vraiment ne pas reutiliser svp 

static void draw_path_rooms(S_MATRIX * matrix, S_ROOM_CS * rcs1, S_ROOM_CS * rcs2){
    /*
    relie les rectangles rcs1 et rcs2 dans matrix avec un chemin de 1
    */
    
    MAT_SQUARE start_1 = rcs1->start_square ; 
    MAT_SQUARE start_2 = rcs2->start_square ; 

    if(rcs1->south_square.i < rcs2->north_square.i){ //cas A et B 
        if(rcs1->south_square.j <= rcs2->north_square.j){  //cas A

            unsigned stop_i = rcs1->south_square.i + (rcs2->north_square.i - rcs1->south_square.i) ;
            unsigned stop_j = rcs1->south_square.j + (rcs2->north_square.j - rcs1->south_square.j) ;
            
            for(unsigned i = rcs1->south_square.i ; i < stop_i ;i++ ){
                // printf("case A i=%u, rcs1.south.i=%u\n", i , rcs1->south_square.i);
                if(!matrix->matrix[i][rcs1->south_square.j]){
                    matrix->matrix[i][rcs1->south_square.j] = 1 ; 
                }
            }
            for(unsigned j = rcs1->south_square.j ; j < stop_j +2 ; j++  ){
               if(j < matrix->row){
                if(!matrix->matrix[stop_i][j]){
                    matrix->matrix[stop_i][j] = 1 ; 
                }
               }
            }
        }else { //if(rcs1->south_square.j > rcs2->souht_square.j) //cas B 
            unsigned stop_i = rcs1->south_square.i + (rcs2->north_square.i - rcs1->south_square.i) ;
            unsigned stop_j = rcs2->north_square.j + (rcs1->south_square.j - rcs2->north_square.j) ;

            for(unsigned i = rcs1->south_square.i ; i < stop_i ;i++ ){
                //printf("case B i=%u, rcs1.south.j=%u\n", i , rcs1->south_square.j);
                if(!matrix->matrix[i][rcs1->south_square.j]){
                    matrix->matrix[i][rcs1->south_square.j] = 1 ; 
                }
            }
            for(unsigned j = rcs2->north_square.j ; j < stop_j+2 ; j++){
                if(j < matrix->row){
                if(!matrix->matrix[stop_i][j]){
                    matrix->matrix[stop_i][j] = 1 ; 
                }
                }
            }

        }
    }else if(rcs1->north_square.i > rcs2->south_square.i){ //cas E et F 
        if(rcs1->north_square.j > rcs2->south_square.j){//cas E 
            unsigned stop_i = rcs2->south_square.i + (rcs1->north_square.i - rcs2->south_square.i);
            unsigned stop_j = rcs2->south_square.j + (rcs1->north_square.j - rcs2->south_square.j);

            for(unsigned i = rcs2->south_square.i ; i < stop_i ; i ++){
                // printf("case C i=%u, rcs2.south.i=%u\n", i , rcs2->south_square.i);
                if(!matrix->matrix[i][stop_j]){
                    matrix->matrix[i][stop_j] = 1 ; 
                }
            }
            for(unsigned j = rcs2->south_square.j ; j < stop_j +2 ; j++){
                if(j < matrix->row){
                if(!matrix->matrix[rcs2->south_square.i][j]){
                    matrix->matrix[rcs2->south_square.i][j] = 1 ; 
                }
                }
            }
        }else{ //cas F
            unsigned stop_i = rcs2->south_square.i + (rcs1->north_square.i - rcs2->south_square.i);
            unsigned stop_j = rcs1->north_square.j + (rcs2->south_square.j - rcs1->north_square.j);

            for(unsigned i = rcs2->south_square.i ; i < stop_i ; i ++){
                if(!matrix->matrix[i][rcs1->north_square.j]){
                    matrix->matrix[i][rcs1->north_square.j] = 1 ; 
                }
            }
            for(unsigned j = rcs1->north_square.j ; j < stop_j +2 ; j++){
                if(j < matrix->row){
                if(!matrix->matrix[stop_i][j]){
                    matrix->matrix[stop_i][j] = 1 ; 
                }
                }
            }
        }

    }else{ //cas C et D -> traits sur les cotes (horizontaux)
        if(rcs1->west_square.j > rcs2->east_square.j){ //cas C
            unsigned stop_j = rcs2->east_square.j + (rcs1->west_square.j - rcs2->east_square.j ) ;
            
            
            for(unsigned j = rcs2->east_square.j ; j < stop_j+2 ; j++){
                if(j < matrix->row){
                if(!matrix->matrix[rcs2->east_square.i][j]){
                    matrix->matrix[rcs2->east_square.i][j] = 1 ; 
                }
                }
            }
            unsigned min_i = rcs2->east_square.i < rcs1->west_square.i ? rcs2->east_square.i : rcs2->west_square.i; 
            unsigned max_i = rcs2->east_square.i < rcs1->west_square.i ? rcs1->west_square.i : rcs2->east_square.i;

            unsigned stop_i = min_i + (max_i - min_i); 

            for(unsigned i = min_i; i < stop_i ; i++){
                if(!matrix->matrix[i][stop_j]){
                    matrix->matrix[i][stop_j] = 1 ; 
                }
            } 
        }else{ //cas D 
            unsigned stop_j = rcs1->west_square.j + (rcs2->east_square.j - rcs1->west_square.j) ;

            for(unsigned j = rcs1->west_square.j ; j < stop_j +2; j++){
                
                if(j < matrix->row){
                    if(!matrix->matrix[rcs1->west_square.i][j]){
                        matrix->matrix[rcs1->west_square.i][j] = 1 ; 
                    }
                }
            }

            unsigned min_i = rcs2->east_square.i < rcs1->west_square.i ? rcs2->east_square.i : rcs1->west_square.i; 
            unsigned max_i = rcs2->east_square.i < rcs1->west_square.i ? rcs1->west_square.i : rcs2->east_square.i;

            unsigned stop_i = min_i + (max_i - min_i); 

            for(unsigned i = min_i; i < stop_i ; i++){
                
                if(!matrix->matrix[i][stop_j]){
                    matrix->matrix[i][stop_j] = 1 ; 
                }
            } 
        }
    }

    return ;
}//fonction statique, pas utiliser 
//  oiqùjvspoimjvmodsjfm<kedovùpswkv


static void draw_path_rooms_v2(S_MATRIX * matrix, S_ROOM_CS * rcs1, S_ROOM_CS * rcs2){

    MAT_SQUARE start_1 = rcs1->start_square ; 
    MAT_SQUARE start_2 = rcs2->start_square ; 

    if(start_1.i <= start_2.i && start_1.j <= start_2.j){
        for (unsigned i = start_1.i; i < start_2.i; i++) {
            if(!matrix->matrix[i][start_1.j]){
                matrix->matrix[i][start_1.j] = 1 ;
            }
        }
        for (unsigned j = start_1.j; j < start_2.j ; j++) {
            if(!matrix->matrix[start_2.i][j]){
                matrix->matrix[start_2.i][j] = 1; 
            }
        }
    }else if(start_1.i <= start_2.i && start_1.j > start_2.j){
        for (unsigned i = start_1.i; i < start_2.i; i++) {
            if(!matrix->matrix[i][start_1.j]){
                matrix->matrix[i][start_1.j] = 1 ;
            }
        }
        for(unsigned j = start_2.j ; j < start_1.j ; j++){
            if(!matrix->matrix[start_2.i][j]){
                matrix->matrix[start_2.i][j] = 1; 
            }
        }

    }else if(start_1.i > start_2.i && start_1.j <= start_2.j){

        for(unsigned i = start_2.i ; i < start_1.i ; i++){
            if(!matrix->matrix[i][start_1.j]){
                matrix->matrix[i][start_1.j] = 1 ;
            }
        }
        for (unsigned j = start_1.j; j < start_2.j ; j++) {
            if(!matrix->matrix[start_2.i][j]){
                matrix->matrix[start_2.i][j] = 1; 
            }
        }


    }else{ //if(start_1.i > start_2.i && start_1.j > start_2.j)
        for(unsigned i = start_2.i ; i < start_1.i ; i++){
            if(!matrix->matrix[i][start_2.j]){
                matrix->matrix[i][start_2.j] = 1 ;
            }
        }
        for(unsigned j = start_2.j ; j < start_1.j ; j++){
            if(!matrix->matrix[start_2.i][j]){
                matrix->matrix[start_2.i][j] = 1; 
            }
        }
        
    }
}

//structure locale (tableau d'u32) pour union-find
typedef struct s_union_find{
    unsigned * elements ; 
    unsigned size ; 
} S_UNION_FIND;

static unsigned ufind_find(S_UNION_FIND * union_find, unsigned elem){
    
    unsigned parent = union_find->elements[elem]; 
    unsigned tmp_elem = elem; 

    while(parent != tmp_elem){
        tmp_elem = parent ; 
        parent = union_find->elements[tmp_elem]; 
    }

    return tmp_elem;
}

static void ufind_union(S_UNION_FIND * union_find , unsigned elem1, unsigned elem2){
    /*
    union basique sans compression 
    de chemin entre elem1 et elem2
    */

    unsigned father1 = ufind_find(union_find, elem1 );
    unsigned father2 = ufind_find(union_find, elem2) ; 

    union_find->elements[father1] = father2 ; 

    return ; 
}

static void connect_rooms_union_find(S_MATRIX * matrix){
    /*
    strategie de connexion : tant qu'il y a plusieurs 
    ensembles disjoints de rectangles -> fait l'union (trace un chemin) entre 2 rectangles
    */
    S_RCS_TAB * rcs_tab = create_rcs_tab(matrix); 

    S_UNION_FIND uf ; 
    uf.elements =  calloc(rcs_tab->size, sizeof(unsigned));
    uf.size = rcs_tab->size ; 

    for(unsigned i = 0 ; i < uf.size ; i++) uf.elements[i] = i ; //initialise chaque element
    //comme son propre ensemble

    unsigned nb_sets = uf.size ; 
    while(nb_sets != 1 ){

        unsigned dsjs_index = 0 ; 
        unsigned * disjoints_sets = calloc(nb_sets, sizeof(unsigned));
        //trouve la liste des elements disjoints
        for(unsigned i = 0 ; i < uf.size ; i++){
            if(uf.elements[i] == i){
                //printf("dsjs_index=%u, nb_sets=%u, i=%u\n", dsjs_index, nb_sets, i);
                disjoints_sets[dsjs_index++] = i ; 
            }
        }
        //fait l'union de deux elements disjoints aleatoires 
        unsigned set1 = disjoints_sets[rand()%dsjs_index]; 
        unsigned set2 = disjoints_sets[rand()%dsjs_index]; 

        while(set1 == set2){
            set2 = disjoints_sets[rand()%dsjs_index];
        }
        ufind_union(&uf, set1, set2);
        //trace un chemin entre les rectangles d'index choisis
        draw_path_rooms_v2(matrix, &(rcs_tab->rcs_tab[set1]),  &(rcs_tab->rcs_tab[set2]) );
        draw_path_rooms(matrix, &(rcs_tab->rcs_tab[set1]),  &(rcs_tab->rcs_tab[set2]) );

        free(disjoints_sets);
        nb_sets--; 
    }

    free(uf.elements);
    free(rcs_tab->rcs_tab); 
    free(rcs_tab);
}

S_MATRIX * generate_matrix(unsigned row, unsigned col, unsigned nb_salles, unsigned nb_petites_salles){

    S_MATRIX * ret = create_matrix(row, col);

    global_object_list.list = calloc(nb_salles + nb_petites_salles, sizeof(OBJ_INFOS));
    global_object_list.nb_objects_max = nb_petites_salles + nb_salles ; 
    global_object_list.nb_objects_cur = 0 ; 

    MAT_SQUARE * petites_salles_arr = calloc(nb_petites_salles, sizeof(MAT_SQUARE)); 
    generate_rooms(ret, nb_salles, nb_petites_salles,petites_salles_arr);
    connect_rooms_union_find(ret); 

    unsigned id_petite_salles = nb_salles + 2 ; 
    ID_SMALL_ROOM = id_petite_salles ; 

    for(unsigned salle = 0 ; salle < nb_petites_salles ; salle ++){
        for(unsigned i = petites_salles_arr[salle].i; i < petites_salles_arr[salle].i + ROOM_WIDTH_MIN;i++){
            for(unsigned j = petites_salles_arr[salle].j ; j < petites_salles_arr[salle].j + ROOM_LENGTH_MIN ; j++){

                ret->matrix[i][j] = id_petite_salles;
            }
        }
    }
    /*
    for(unsigned i = 0 ; i < global_object_list.nb_objects_cur ; i++){
        printf("id=%u posi=%u posx=%u\n",global_object_list.list[i].id ,global_object_list.list[i].i, global_object_list.list[i].j);
        printf("mat[i][j]=%u\n", ret->matrix[global_object_list.list[i].i][global_object_list.list[i].j]);
    }*/

    free(petites_salles_arr);
    return ret ; 

}//PAS ENCORE FAIT 

void fprint_matrix(FILE * flux, S_MATRIX * mat){
    for(unsigned i = 0 ; i < mat->row; i++){
        for(unsigned j = 0 ; j < mat->col ; j++){
            fprintf(flux, "%u ", mat->matrix[i][j]); 
        }
        fprintf(flux, "\n");
    }
}

void fprint_matrix_var(FILE * flux, S_MATRIX * mat){
    for(unsigned i = 0 ; i < mat->row; i++){
        for(unsigned j = 0 ; j < mat->col ; j++){
            if(mat->matrix[i][j]){
                fprintf(flux, "#"); 
            }else{
                fprintf(flux, " "); 
            }
        }
        fprintf(flux, "\n");
    }
}
