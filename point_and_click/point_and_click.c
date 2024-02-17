#ifndef POINT_AND_CLICK_C
#define POINT_AND_CLICK_C

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "point_and_click.h"
#include "../timer/timer.h"

#ifndef WINDOW_SIZE
#define WINDOW_SIZE
#define WIN_X 1000
#define WIN_Y 600
#endif

char* types[] = {"big_orange_round_bottle",
"big_pink_cylinder_bottle",
"big_yellow_round_bottle",
"long_brown_cylinder_bottle",
"long_darkblue_square_bottle",
"long_darkblue_triangle_bottle",
"long_orange_cylinder_bottle",
"long_yellow_triangle_bottle",
"medium_blue_round_bottle",
"medium_blue_triangle_bottle",
"medium_green_triangle_bottle",
"medium_orange_round_box",
"medium_purple_round_box",
};

const int types_size = sizeof(types) / sizeof(char*);


char* translation(char* taille, char* couleur, char* conteneur, char* shape) {
    // taille : grand, petit
    // couleur : rouge, vert, bleu
    // conteneur : fiole, boite
    char* traduction = malloc(100);
    // print the conteneur
    //printf("%s %s %s %s\n", taille, couleur, conteneur, shape);
    if (strcmp(conteneur, "bottle") == 0) {
        strcpy(traduction, "Flacon");
    } else if (strcmp(conteneur, "box") == 0) {
        strcpy(traduction, "Boite");
    } else {
        strcpy(traduction, "Objet");
    }

    // print the taille
    if (strcmp(taille, "big") == 0) {
        strcat(traduction, " grand");
    } else if (strcmp(taille, "small") == 0) {
        strcat(traduction, " petit");
    } else if (strcmp(taille, "medium") == 0) {
        strcat(traduction, " moyen");
    } else if (strcmp(taille, "long") == 0) {
        strcat(traduction, " long");
    } else {
        strcat(traduction, " objet");
    }

    // print the couleur
    if (strcmp(couleur, "red") == 0) {
        strcat(traduction, " rouge");
    } else if (strcmp(couleur, "green") == 0) {
        strcat(traduction, " vert");
    } else if (strcmp(couleur, "blue") == 0) {
        strcat(traduction, " bleu clair");
    } else if (strcmp(couleur, "orange") == 0) {
        strcat(traduction, " orange");
    } else if (strcmp(couleur, "darkblue") == 0) {
        strcat(traduction, " bleu fonce");
    } else if (strcmp(couleur, "yellow") == 0) {
        strcat(traduction, " jaune");
    } else if (strcmp(couleur, "pink") == 0) {
        strcat(traduction, " rose");
    } else if (strcmp(couleur, "purple") == 0) {
        strcat(traduction, " violet");
    } else if (strcmp(couleur, "brown") == 0) {
        strcat(traduction, " marron");
    } else if (strcmp(couleur, "greeen") == 0) {
        strcat(traduction, " vert");
    }
    else {
        strcat(traduction, " objet");
    }

    // print the shape
    if (strcmp(shape, "square") == 0) {
        strcat(traduction, " carre");
    } else if (strcmp(shape, "round") == 0) {
        strcat(traduction, " rond");
    } else if (strcmp(shape, "triangle") == 0) {
        strcat(traduction, " triangle");
    } else if (strcmp(shape, "cylinder") == 0) {
        strcat(traduction, " cylindre");
    } 
    else {
        strcat(traduction, " objet");
    }

    return traduction;

}


ClickInfo* grab_bottle(Scene* scene, int x, int y) {
    for (int i = 0; i < scene->bottle_count; i++) {
        if (x >= scene->bottles[i].dest.x && x <= scene->bottles[i].dest.x + scene->bottles[i].dest.w &&
            y >= scene->bottles[i].dest.y && y <= scene->bottles[i].dest.y + scene->bottles[i].dest.h) {
            ClickInfo* click_info = malloc(sizeof(ClickInfo));
            click_info->click_x = x;
            click_info->click_y = y;
            click_info->bottle_grabbed = &scene->bottles[i];
            return click_info;
        }
    }
    return NULL;
}

void render_list(SDL_Renderer* renderer, TTF_Font* font, Recipe* recipe, int recipe_size, ClickInfo* click_info, char* background_path) {

    SDL_Color black = {80, 80, 80, 255};
    SDL_Color white = {255, 255, 255, 255};

    // if hovered over the list, draw it full, otherwise only draw the top
    SDL_Surface* bg_surface = IMG_Load(background_path);
    if (bg_surface == NULL) {
        fprintf(stderr, "Error loading background : %s", SDL_GetError());
        exit(5);
    }
    SDL_Texture* bg_texture = SDL_CreateTextureFromSurface(renderer, bg_surface);
    SDL_Rect bg_dest = {WIN_X - 300, 0, 300, 200};

    // draw the word "liste" on top of the background
    SDL_Surface* liste_surface = TTF_RenderText_Solid(font, "Liste", black);
    SDL_Texture* liste_texture = SDL_CreateTextureFromSurface(renderer, liste_surface);
    SDL_Rect liste_dest = {WIN_X - 200, 0, 100, 50};

    // if you hover over the list, draw the full list
    SDL_Surface* liste_bottom_surface = IMG_Load("point_and_click/assets/liste_bottom.png");
    if (liste_bottom_surface == NULL) {
        fprintf(stderr, "Error loading liste_top : %s", SDL_GetError());
        exit(5);
    }
    SDL_Texture* liste_bottom_texture = SDL_CreateTextureFromSurface(renderer, liste_bottom_surface);
    if (liste_bottom_texture == NULL) {
        fprintf(stderr, "Error creating texture from surface : %s", SDL_GetError());
        exit(6);
    }

    static int rendered = 0; // list is not hovered over

    //printf("%d %d\n", click_info->click_x, click_info->click_y);
    //printf("%d\n", rendered);

    if (rendered == 0) {
        if (click_info->click_x >= WIN_X - 200 && click_info->click_x <= WIN_X &&
            !(click_info->click_y >= 0 && click_info->click_y <= 550)) {
            rendered = 1;
        }
    } else {
        if (click_info->click_x < WIN_X - 200 || click_info->click_x > WIN_X ||
            !(click_info->click_y < 0 || click_info->click_y > 300)) {
            rendered = 0;
        }
    }


    if (rendered == 1) {
        // draw the full list

        // draw the recipe
        SDL_Rect rest_of_list_dest = {WIN_X - 300, -540, 350, 1300};
        SDL_RenderCopy(renderer, liste_bottom_texture, NULL, &rest_of_list_dest);

        bg_dest.y = 120;
        liste_dest.y = 250;
        SDL_RenderCopy(renderer, bg_texture, NULL, &bg_dest);
        SDL_RenderCopy(renderer, liste_texture, NULL, &liste_dest);

        // draw the recipe with translated names
        int nb_zeros = 0;
        for (long unsigned int i = 0; i < recipe_size; i++) {
            //printf("%s\n", recipe[i].bottle_type);
            //printf("%s x %d\n", recipe[i].bottle_type, recipe[i].nb_bottles);
            if (recipe[i].nb_bottles == 0) {
                nb_zeros++;
                continue;
            }
            // split the bottle type name
            char* bottle_type_copy = strdup(recipe[i].bottle_type);
            char* taille = strtok(bottle_type_copy, "_");
            char* couleur = strtok(NULL, "_");
            char* shape = strtok(NULL, "_");
            char* conteneur = strtok(NULL, "_");
            //printf("%s %s %s %s\n", taille, couleur, conteneur, shape);
            //free(bottle_type_copy); // Don't forget to free the copy;

            char* translated = translation(taille, couleur, conteneur, shape);
            //printf("%s\n", translated);
            char text[100];
            sprintf(text, "%s x %d", translated, recipe[i].nb_bottles);
            SDL_Surface* text_surface = TTF_RenderText_Solid(font, text, black);
            SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            SDL_Rect text_dest = {WIN_X - 270, 310 + 20 * (i - nb_zeros), 250, 20};
            SDL_RenderCopy(renderer, text_texture, NULL, &text_dest);

            SDL_FreeSurface(text_surface);
            SDL_DestroyTexture(text_texture);
            free(translated);
            free(bottle_type_copy);

        }

    }
    else {
        // draw only the top with the word list at the bottom of the screen
        bg_dest.y = 420;
        liste_dest.y = 550;
        SDL_RenderCopy(renderer, bg_texture, NULL, &bg_dest);
        SDL_RenderCopy(renderer, liste_texture, NULL, &liste_dest);
    }

    SDL_FreeSurface(bg_surface);
    SDL_DestroyTexture(bg_texture);
    SDL_FreeSurface(liste_surface);
    SDL_DestroyTexture(liste_texture);
    SDL_FreeSurface(liste_bottom_surface);
    SDL_DestroyTexture(liste_bottom_texture);

}

void render_pot(SDL_Renderer* renderer, Pot* pot) {
    SDL_RenderCopy(renderer, pot->texture, NULL, &(SDL_Rect){pot->x, pot->y, 200, 200});
}

Recipe* generate_recipe(int place_totale, int min_to_win, int* recipe_size) {
    int nb_required_to_win = min_to_win + rand() % (place_totale - min_to_win);
    //printf("nb_required_to_win : %d\n", nb_required_to_win);
    //printf("place_totale : %d\n", place_totale);
    //printf("%d\n", types_size);
    //nb_required_to_win = 18; //10,11,8,7,6,5,4
    if (nb_required_to_win < types_size) { // oops memory corruption
        nb_required_to_win = types_size;
    }
    if (nb_required_to_win > place_totale - 5) {
        nb_required_to_win = place_totale - 5;
    }
    Recipe* recipe = calloc(nb_required_to_win, sizeof(Recipe));
    //Recipe* recipe = malloc(sizeof(Recipe) * nb_required_to_win);
    for (int i = 0; i < types_size; i++) {
        recipe[i].bottle_type = types[i];
        recipe[i].nb_bottles = 0;
    }

    for (int i = 0; i < nb_required_to_win; i++) {
        int type = rand() % types_size;
        recipe[type].nb_bottles++;
    }

    //print
    //for (int i = 0; i < types_size; i++) {
        //printf("%s %d\n", recipe[i].bottle_type, recipe[i].nb_bottles);
    //}

    *recipe_size = nb_required_to_win;
    return recipe;
}

SceneInfo* generate_bottle_types(int place_totale, Recipe* recipe, int recipe_size) {
    SceneInfo* bottle_types = calloc(place_totale, sizeof(SceneInfo));
    for (int i = 0; i < types_size; i++) {
        bottle_types[i].bottle_type = types[i];
        bottle_types[i].nb_bottles = 0;
    }

    // copy the recipe
    for (int i = 0; i < recipe_size; i++) {
        bottle_types[i].bottle_type = recipe[i].bottle_type;
        bottle_types[i].nb_bottles = recipe[i].nb_bottles;
        //printf("recipe : %s %d\n", bottle_types[i].bottle_type, bottle_types[i].nb_bottles);
    }

    // fill the rest
    for (int i = recipe_size; i < place_totale; i++) {
        int type = rand() % types_size;
        bottle_types[type].nb_bottles++;
    }

    //for (int i = 0; i < types_size; i++) {
        //printf("fin : %s %d\n", bottle_types[i].bottle_type, bottle_types[i].nb_bottles);
    //}

    return bottle_types;
}

Scene* load_scene(SDL_Renderer* renderer) {

    BottleSpot spots[] = {
        /*
        591 336
680 343
619 251
669 238
811 193
879 206
958 230
765 269
867 311
951 358
770 362
844 402
945 468
44 311
129 304
233 315
329 267
469 282
413 272
97 162
232 164
287 164
426 159
415 123
328 118
218 117
82 100*/
    {591, 336},
    {680, 343},
    {619, 251},
    {669, 238},
    {811, 193},
    {879, 206},
    {958, 230},
    {765, 269},
    {867, 311},
    {951, 358},
    {770, 362},
    {844, 402},
    {945, 468},
    {44, 311},
    {129, 304},
    {233, 315},
    {329, 267},
    {469, 282},
    {413, 272},
    {97, 162},
    {232, 164},
    {287, 164},
    {426, 159},
    {415, 123},
    {328, 118},
    {218, 117},
    {82, 100},
    };

    const int nb_spots = sizeof(spots) / sizeof(BottleSpot);

    for (int i = 0; i < sizeof(spots) / sizeof(BottleSpot); i++) {
        // shift to center the spots
        spots[i].x -= 25;
        spots[i].y -= 70;
    }

    int recipe_size;
    Recipe* recipe = generate_recipe(nb_spots, nb_spots / 3, &recipe_size);

    SceneInfo* bottle_types = generate_bottle_types(nb_spots, recipe, recipe_size);

    // shuffle the spots
    for (int i = 0; i < sizeof(spots) / sizeof(BottleSpot); i++) {
        int j = rand() % (i + 1);
        BottleSpot temp = spots[i];
        spots[i] = spots[j];
        spots[j] = temp;
    }

    Scene* scene = malloc(sizeof(Scene));
    scene->background = IMG_LoadTexture(renderer, "point_and_click/assets/bg.png");
    scene->dest = (SDL_Rect){0, 0, WIN_X, WIN_Y};
    scene->bottle_count = nb_spots;
    scene->bottles = malloc(sizeof(Bottle) * scene->bottle_count);

    int bottle_index = 0;
    for (int i = 0; i < types_size; i++) {
        for (int j = 0; j < bottle_types[i].nb_bottles; j++) {
            char path[100];
            sprintf(path, "point_and_click/assets/potion2/%s.png", bottle_types[i].bottle_type);
            scene->bottles[bottle_index].texture = IMG_LoadTexture(renderer, path);
            scene->bottles[bottle_index].dest = (SDL_Rect){spots[bottle_index].x, spots[bottle_index].y, 50, 80};
            scene->bottles[bottle_index].type = bottle_types[i].bottle_type;
            bottle_index++;
        }
    }

    scene->recipe = recipe;
    scene->recipe_size = types_size;

    Pot* pot = malloc(sizeof(Pot));
    pot->texture = IMG_LoadTexture(renderer, "point_and_click/assets/pot.png");
    pot->x = 450;
    pot->y = 400;
    scene->pot = pot;

    return scene;
}

void render_scene(SDL_Renderer* renderer, Scene* scene, TTF_Font* font) {
    SDL_RenderCopy(renderer, scene->background, NULL, &scene->dest);
    for (int i = 0; i < scene->bottle_count; i++) {
        // print l'emplacement de la bouteille
        //printf("%d %d %p\n", scene->bottles[i].dest.x, scene->bottles[i].dest.y, scene->bottles[i].texture);
        if (scene->bottles[i].texture != NULL) {
            SDL_RenderCopy(renderer, scene->bottles[i].texture, NULL, &scene->bottles[i].dest);
        }
    }
    //render_list(renderer, font, scene->recipe, scene->recipe_size, 
    render_pot(renderer, scene->pot);
}

int point_and_click(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, Timer* timer) {
    // Load the background
    SDL_Surface* bg_surface = IMG_Load("point_and_click/assets/bg.png");
    SDL_Texture* bg_texture = SDL_CreateTextureFromSurface(renderer, bg_surface);
    if (!bg_texture) {
        fprintf(stderr, "Error creating texture from surface : %s", SDL_GetError());
        exit(6);
    }
    
    SceneInfo* scores = calloc(types_size, sizeof(SceneInfo));
    for (int i = 0; i < types_size; i++) {
        scores[i].bottle_type = types[i];
        scores[i].nb_bottles = 0;
    }


    Scene* scene = load_scene(renderer);
    SDL_Event event;
    int running = 1;
    int fin = 0;
    const int YOU_WIN = 1;
    const int YOU_LOSE = 0;
    const int BANANA_ENDING = 2;
    ClickInfo* click_info = NULL;
    ClickInfo mouse_pos = {0, 0, NULL};
    while (running && !is_timer_finished(timer)) {
        SDL_RenderClear(renderer);

        // grab the bottle
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_KP_ENTER) {
                running = 0;
            }

            // get the mouse position
            if (event.type == SDL_MOUSEMOTION) {
                mouse_pos.click_x = event.motion.x;
                mouse_pos.click_y = event.motion.y;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                //printf("%d %d\n", event.button.x, event.button.y);
                click_info = grab_bottle(scene, event.button.x, event.button.y);
                if (click_info != NULL) {
                    click_info->bottle_grabbed->dest.x = event.button.x - click_info->bottle_grabbed->dest.w / 2;
                    click_info->bottle_grabbed->dest.y = event.button.y - click_info->bottle_grabbed->dest.h / 2;
                    int dragging = 1;
                    while (dragging) {
                        while (SDL_PollEvent(&event)) {
                            if (event.type == SDL_MOUSEBUTTONUP) {
                                dragging = 0;
                                //printf("mouse up\n");
                                //printf("%d %d\n", event.button.x, event.button.y);
                                //printf("%d %d\n", scene->pot->x, scene->pot->y);
                                // check if the bottle is in the pot
                                if (event.button.x >= scene->pot->x && event.button.x <= scene->pot->x + 200 &&
                                    event.button.y >= scene->pot->y && event.button.y <= scene->pot->y + 200) {
                                    //printf("in the pot\n");
                                    //printf("%s\n", click_info->bottle_grabbed->type);
                                    for (int i = 0; i < scene->recipe_size; i++) {
                                        //score++
                                        if (strcmp(click_info->bottle_grabbed->type, scene->recipe[i].bottle_type) == 0) {
                                            scores[i].nb_bottles++;
                                            //printf("ADDED %s %d %d\n", scene->recipe[i].bottle_type, scores[i].nb_bottles, scene->recipe[i].nb_bottles);
                                            // remove bottle from scene
                                            click_info->bottle_grabbed->texture = NULL;
                                            click_info->bottle_grabbed->dest.x = -1000;
                                            click_info->bottle_grabbed->dest.y = -1000;
                                            if (scores[i].nb_bottles > scene->recipe[i].nb_bottles) {
                                                //printf("You lose !\n");
                                                running = 0;
                                                fin = YOU_LOSE;
                                            }
                                        }

                                        // check if win
                                        int win = 1;
                                        for (int i = 0; i < scene->recipe_size; i++) {
                                            if (scores[i].nb_bottles < scene->recipe[i].nb_bottles) {
                                                //printf("%s %d %d\n", scene->recipe[i].bottle_type, scores[i].nb_bottles, scene->recipe[i].nb_bottles);
                                                win = 0;
                                            }
                                        }
                                        if (win) {
                                            //printf("You win !\n");
                                            running = 0;
                                            fin = YOU_WIN;
                                        }
                                    }
                                }
                            }
                            if (event.type == SDL_MOUSEMOTION) {
                                click_info->bottle_grabbed->dest.x = event.motion.x - click_info->bottle_grabbed->dest.w / 2;
                                click_info->bottle_grabbed->dest.y = event.motion.y - click_info->bottle_grabbed->dest.h / 2;
                            }
                        }
                        SDL_RenderClear(renderer);
                        render_scene(renderer, scene, font);
                        render_list(renderer, font, scene->recipe, scene->recipe_size, &mouse_pos, "point_and_click/assets/liste_top.png");
                        //render_pot(renderer, scene->pot);
                        render_timer(timer, window, renderer, font);
                        SDL_RenderPresent(renderer);
                    }
                    free(click_info);
                }
            }
        }
        // render the scene
        render_scene(renderer, scene, font);
        render_list(renderer, font, scene->recipe, scene->recipe_size, &mouse_pos, "point_and_click/assets/liste_top.png");

        // render the timer
        render_timer(timer, window, renderer, font);

        // render the grabbed bottle
        SDL_RenderPresent(renderer);
    }

    if (is_timer_finished(timer)) {
        banana_end(window, renderer, font);
    }

    SDL_DestroyTexture(bg_texture);
    SDL_FreeSurface(bg_surface);
    for (int i = 0; i < scene->bottle_count; i++) {
        SDL_DestroyTexture(scene->bottles[i].texture);
    }
    free(scene->bottles);
    free(scene);

    if (fin == YOU_WIN) {
        good_end(window, renderer, font);
    } else if (fin == YOU_LOSE) {
        boom_end(window, renderer, font);
    }

}

#endif