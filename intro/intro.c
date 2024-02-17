#ifndef INTRO_C
#define INTRO_C

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intro.h"
#include "../timer/timer.h"

#ifndef WINDOW_SIZE
#define WINDOW_SIZE
#define WIN_X 1000
#define WIN_Y 600
#endif

int strlen2(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

void display_text_image(SDL_Renderer* renderer, TTF_Font* font, const char* text, const char* emote, Mix_Music** sfx, int sfx_count, Image* bg, Timer* timer) {

    static int main_shift_x = 0;
    static int main_shift_y = 0;

    // Load the speech bubble
    SDL_Surface* speech_bubble_surface = load_image("intro/assets/speech_bubble.png");
    SDL_Texture* speech_bubble_texture = SDL_CreateTextureFromSurface(renderer, speech_bubble_surface);
    SDL_Rect speech_bubble_dest = {20, WIN_Y - 220, 700, 200};
    if (!speech_bubble_texture) {
        fprintf(stderr, "Error creating texture from surface : %s", SDL_GetError());
        exit(6);
    }

    // Load an image
    char emote_path[100];
    sprintf(emote_path, "intro/assets/tete/%s.png", emote);
    SDL_Surface* emote_surface = load_image(emote_path);
    SDL_Texture* emote_texture = SDL_CreateTextureFromSurface(renderer, emote_surface);
    if (!emote_texture) {
        fprintf(stderr, "Error creating texture from surface : %s", SDL_GetError());
        exit(6);
    }

    // Draw some text
    SDL_Color color = {0, 0, 0, 255};
    int text_length = strlen2(text);
    int text_width, text_height;
    TTF_SizeText(font, text, &text_width, &text_height);
    //SDL_Surface* text_surface = TTF_RenderText_Blended_Wrapped(font, text, color, WIN_X - 100);
    //SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Surface* text_surface;
    SDL_Texture* text_texture;
    SDL_Rect text_dest = {80, WIN_Y - text_height - 50, text_width, text_height};

    SDL_Surface* mains_surface = load_image("intro/assets/mains.png");
    SDL_Texture* mains_texture = SDL_CreateTextureFromSurface(renderer, mains_surface);
    SDL_Rect mains_dest = {50, 0, WIN_X, WIN_Y};

    // Draw the emote above the text
    SDL_Rect emote_dest = {70, WIN_Y - text_height - 110 - 100, 150, 150};

    SDL_Surface* enterkeysurface = load_image("intro/assets/enterkey.png");
    SDL_Texture* enterkeytexture = SDL_CreateTextureFromSurface(renderer, enterkeysurface);
    SDL_Rect enterkey_dest = {WIN_X - 370, WIN_Y - 120, 100, 100};
    // while not pressed enter, keep displaying the image
    int running = 1;
    SDL_Event event;
    int nb_chars = 0;
    int frame = 0;
    int render_enterkey = 0;
    while ((running) && (timer == NULL || !is_timer_finished(timer))) {

        //smoothly shift the hands randomly
        if (frame % 10 == 0) {
            main_shift_x = 50 + rand() % 10 - 5;
            main_shift_y = rand() % 10 - 5;
        }


        SDL_RenderClear(renderer);

        // Load the background²
        SDL_RenderCopy(renderer, bg->texture, NULL, &bg->dest);

        // render the hands
        mains_dest.x = main_shift_x;
        mains_dest.y = main_shift_y;

        SDL_RenderCopy(renderer, mains_texture, NULL, &mains_dest);

        SDL_RenderCopy(renderer, emote_texture, NULL, &emote_dest);
        SDL_RenderCopy(renderer, speech_bubble_texture, NULL, &speech_bubble_dest);

        // display the characters one after the other
        frame++;
        if (frame % 2 == 0) {
            // bop the character up and down
            if (frame % 20 < 10) {
                emote_dest.y -= 1;
            } else {
                emote_dest.y += 1;
            }
            if (nb_chars % 10 == 0 && sfx_count > 0) {
                Mix_PlayMusic(sfx[rand() % sfx_count], 1);
            }
            nb_chars++;
        }
        if (nb_chars > text_length) {
            nb_chars = text_length;
            render_enterkey = 1;
        }

        if (render_enterkey) {
            SDL_RenderCopy(renderer, enterkeytexture, NULL, &enterkey_dest);
        }
        char* text_to_display = malloc(nb_chars + 1);
        strncpy(text_to_display, text, nb_chars);
        text_to_display[nb_chars] = '\0';
        text_surface = TTF_RenderText_Blended_Wrapped(font, text_to_display, color, WIN_X - 100);
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        TTF_SizeText(font, text_to_display, &text_width, &text_height);
        text_dest.w = text_width;
        text_dest.h = text_height;

        // Draw the text
        SDL_RenderCopy(renderer, text_texture, NULL, &text_dest);
        free(text_to_display);


        while (SDL_PollEvent(&event)) {
            /*if (timer != NULL) {
                update_timer(timer);
            }*/
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                running = 0;
            }
        }

        if (timer != NULL) {
            render_timer(timer, NULL, renderer, font);
        } 


        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(speech_bubble_texture);
    SDL_FreeSurface(speech_bubble_surface);
    SDL_DestroyTexture(emote_texture);
    SDL_FreeSurface(emote_surface);
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(enterkeytexture);
    SDL_FreeSurface(enterkeysurface);
}

Image load_background(char* path, SDL_Renderer* renderer) {
    SDL_Surface* bg_surface = load_image(path);
    SDL_Texture* bg_texture = SDL_CreateTextureFromSurface(renderer, bg_surface);
    if (!bg_texture) {
        fprintf(stderr, "Error creating texture from surface : %s", SDL_GetError());
        exit(6);
    }
    Image bg = {bg_texture, {0, 0, WIN_X, WIN_Y}};
    return bg;
}

// Pour chaque dialogue, on affiche le texte et l'image et on attend que l'utilisateur appuie sur entrée
void display_dialogue(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog, int dialog_count, Mix_Music** sfx, int sfx_count, Timer* timer) {
    // Load background
    Image bg = load_background("intro/assets/background.png", renderer);
    for (int i = 0; i < dialog_count; i++) {
        display_text_image(renderer, font, dialog[i].text, dialog[i].emote_path, sfx, sfx_count, &bg, timer);
    }
}

void intro(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font) {
    // SFX mp3 
    Mix_Music** pencil_sfx = malloc(4 * sizeof(Mix_Music*));
    pencil_sfx[0] = Mix_LoadMUS("intro/assets/pencil_sfx1.mp3");
    pencil_sfx[1] = Mix_LoadMUS("intro/assets/pencil_sfx2.mp3");
    pencil_sfx[2] = Mix_LoadMUS("intro/assets/pencil_sfx3.mp3");
    pencil_sfx[3] = Mix_LoadMUS("intro/assets/pencil_sfx4.mp3");
    
    Dialog dialog[] = {
        {"Encore une belle journee ou je vais ne faire aucune betise", "heureux"},
        {"It's TIME to finir d'etudier le coeur du demon !", "assure"},
        {"...", "assure"},
        {"Ah merde, j'ai coince mon crayon de papier dedans", "oops"},
        {"Je pense c'est pas trop grave", "heureux"},
        {"...", "oops"},
        {"OH PUTAIN PUTAIN PUTAIN", "panic"},
        {"J'ai casse le crayon de papier !", "panic"},
        {"...", "oops"},
        {"Attends c'est pas radioactif deja?", "oops"},
        {"...", "panic"},
        {"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", "panic"},
        {"VITE JE DOIS TROUVER LA RECETTE POUR L'ANTIDOTE AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", "panic"},
    };
    int dialog_count = sizeof(dialog) / sizeof(Dialog);

    display_dialogue(renderer, font, dialog, dialog_count, pencil_sfx, 4, NULL);
    free(pencil_sfx);
}

void intro_point_and_click(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, Timer* timer) {
    // SFX mp3 
    Mix_Music** pencil_sfx = malloc(4 * sizeof(Mix_Music*));
    pencil_sfx[0] = Mix_LoadMUS("intro/assets/pencil_sfx1.mp3");
    pencil_sfx[1] = Mix_LoadMUS("intro/assets/pencil_sfx2.mp3");
    pencil_sfx[2] = Mix_LoadMUS("intro/assets/pencil_sfx3.mp3");
    pencil_sfx[3] = Mix_LoadMUS("intro/assets/pencil_sfx4.mp3");
    
    Dialog dialog[] = {
        {"CHOSIS LES BONNES POTIONS", "panic"},
        {"SINON TU MEURTS", "panic"},
    };

    display_dialogue(renderer, font, dialog, 2, pencil_sfx, 4, timer);
    free(pencil_sfx);
}

#endif