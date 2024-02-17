#ifndef POINT_AND_CLICK_C
#define POINT_AND_CLICK_C

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef WINDOW_SIZE
#define WINDOW_SIZE
const int WIN_X = 1000;
const int WIN_Y = 600;
#endif

typedef struct {
    char* bottle_type;
    int nb_bottles;
} Recipe;

typedef struct {
    int x, y;
} BottleSpot;

typedef struct {
    SDL_Texture* texture;
    SDL_Rect dest;
    char* type;
} Bottle;

typedef struct {
    int x, y;
    SDL_Texture* texture;
} Pot;

typedef struct {
    SDL_Texture* background;
    SDL_Rect dest;
    Bottle* bottles;
    int bottle_count;
    Recipe* recipe;
    int recipe_size;
    Pot* pot;
} Scene;

typedef struct {
    char* bottle_type;
    int nb_bottles;
} SceneInfo;

typedef struct {
    int click_x, click_y;
    Bottle* bottle_grabbed;
} ClickInfo;

char* types[] = {"square", "round", "triangle"}; // A PAS MODIF
const int types_size = 3;


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

void render_list(SDL_Renderer* renderer, TTF_Font* font, Recipe* recipe, int recipe_size) {
    // print bonjour
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, "Recipe", color);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Rect text_dest = {WIN_X - 200, 20, 100, 50};
    SDL_RenderCopy(renderer, text_texture, NULL, &text_dest);
    
    for (long unsigned int i = 0; i < recipe_size; i++) {
        char text[100];
        //printf("%s\n", recipe[i].bottle_type);
        //printf("%s x %d\n", recipe[i].bottle_type, recipe[i].nb_bottles);
        sprintf(text, "%s x %d", recipe[i].bottle_type, recipe[i].nb_bottles);
        SDL_Surface* text_surface = TTF_RenderText_Solid(font, text, color);
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_Rect text_dest = {WIN_X - 200, 70 + 50 * i, 100, 50};
        SDL_RenderCopy(renderer, text_texture, NULL, &text_dest);

        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);
    }

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);

}

void render_pot(SDL_Renderer* renderer, Pot* pot) {
    SDL_RenderCopy(renderer, pot->texture, NULL, &(SDL_Rect){pot->x, pot->y, 100, 100});
}

Recipe* generate_recipe(int place_totale, int min_to_win, int* recipe_size) {
    int nb_required_to_win = rand() % (place_totale - min_to_win) + min_to_win;
    Recipe* recipe = calloc(nb_required_to_win, sizeof(Recipe));

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
        printf("recipe : %s %d\n", bottle_types[i].bottle_type, bottle_types[i].nb_bottles);
    }

    // fill the rest
    for (int i = recipe_size; i < place_totale; i++) {
        int type = rand() % types_size;
        bottle_types[type].nb_bottles++;
    }

    for (int i = 0; i < types_size; i++) {
        printf("fin : %s %d\n", bottle_types[i].bottle_type, bottle_types[i].nb_bottles);
    }

    return bottle_types;
}

Scene* load_scene(SDL_Renderer* renderer) {

    BottleSpot spots[] = {
        {10, 10},
        {20, 20},
        {30, 30},
        {40, 140},
        {50, 50},
        {60, 60},
    };

    const int nb_spots = 6;

    int recipe_size;
    Recipe* recipe = generate_recipe(nb_spots, nb_spots / 2, &recipe_size);

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
            sprintf(path, "point_and_click/assets/%s_bottle.png", bottle_types[i].bottle_type);
            scene->bottles[bottle_index].texture = IMG_LoadTexture(renderer, path);
            scene->bottles[bottle_index].dest = (SDL_Rect){spots[bottle_index].x, spots[bottle_index].y, 100, 100};
            scene->bottles[bottle_index].type = bottle_types[i].bottle_type;
            bottle_index++;
        }
    }

    scene->recipe = recipe;
    scene->recipe_size = types_size;

    Pot* pot = malloc(sizeof(Pot));
    pot->texture = IMG_LoadTexture(renderer, "point_and_click/assets/pot.png");
    pot->x = 500;
    pot->y = 500;
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
    render_list(renderer, font, scene->recipe, scene->recipe_size);
    render_pot(renderer, scene->pot);
}

void point_and_click(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font) {
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
    while (running) {
        SDL_RenderClear(renderer);

        // grab the bottle
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_KP_ENTER) {
                running = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                ClickInfo* click_info = grab_bottle(scene, event.button.x, event.button.y);
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
                                if (event.button.x >= scene->pot->x && event.button.x <= scene->pot->x + 100 &&
                                    event.button.y >= scene->pot->y && event.button.y <= scene->pot->y + 100) {
                                    //printf("in the pot\n");
                                    //printf("%s\n", click_info->bottle_grabbed->type);
                                    for (int i = 0; i < scene->recipe_size; i++) {
                                        //score++
                                        if (strcmp(click_info->bottle_grabbed->type, scene->recipe[i].bottle_type) == 0) {
                                            scores[i].nb_bottles++;
                                            // remove bottle from scene
                                            click_info->bottle_grabbed->texture = NULL;
                                            if (scores[i].nb_bottles > scene->recipe[i].nb_bottles) {
                                                printf("You dumb :-(\n");
                                                running = 0;
                                            }
                                        }

                                        // check if win
                                        int win = 1;
                                        for (int i = 0; i < scene->recipe_size; i++) {
                                            if (scores[i].nb_bottles < scene->recipe[i].nb_bottles) {
                                                win = 0;
                                            }
                                        }
                                        if (win) {
                                            printf("You win !\n");
                                            running = 0;
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
                        SDL_RenderPresent(renderer);
                    }
                    free(click_info);
                }
            }
        }
        // render the scene
        render_scene(renderer, scene, font);

        // render the grabbed bottle
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(bg_texture);
    SDL_FreeSurface(bg_surface);
    for (int i = 0; i < scene->bottle_count; i++) {
        SDL_DestroyTexture(scene->bottles[i].texture);
    }
    free(scene->bottles);
    free(scene);

}

#endif