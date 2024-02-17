#ifndef INTRO_C
#define INTRO_C

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef WINDOW_SIZE
#define WINDOW_SIZE
const int WIN_X = 1000;
const int WIN_Y = 600;
#endif


// Load image from file
SDL_Surface* load_image(const char* filename) {
    SDL_Surface* img = IMG_Load(filename);
    if (!img) {
        fprintf(stderr, "Error loading image %s : %s", filename, IMG_GetError());
        exit(5);
    }
    return img;
}

typedef struct {
    const char* text;
    const char* emote_path;
} Dialog;

int strlen2(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

typedef struct {
    SDL_Texture* texture;
    SDL_Rect dest;
} Image;

void display_text_image(SDL_Renderer* renderer, TTF_Font* font, const char* text, const char* emote, Mix_Music** sfx, int sfx_count, Image* bg) {

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
    sprintf(emote_path, "intro/assets/%s.png", emote);
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

    // Draw the emote above the text
    SDL_Rect emote_dest = {70, WIN_Y - text_height - 110 - 100, 150, 150};


    // while not pressed enter, keep displaying the image
    int running = 1;
    SDL_Event event;
    int nb_chars = 0;
    int frame = 0;
    while (running) {

        SDL_RenderClear(renderer);

        // Load the background²
        SDL_RenderCopy(renderer, bg->texture, NULL, &bg->dest);



        SDL_RenderCopy(renderer, emote_texture, NULL, &emote_dest);
        SDL_RenderCopy(renderer, speech_bubble_texture, NULL, &speech_bubble_dest);

        // display the characters one after the other
        frame++;
        if (frame % 2 == 0) {
            if (nb_chars % 10 == 0 && sfx_count > 0) {
                Mix_PlayMusic(sfx[rand() % sfx_count], 1);
            }
            nb_chars++;
        }
        nb_chars = nb_chars > text_length ? text_length : nb_chars;
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
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                running = 0;
            }
        }


        SDL_RenderPresent(renderer);
    }
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
void display_dialogue(SDL_Renderer* renderer, TTF_Font* font, Dialog* dialog, int dialog_count, Mix_Music** sfx, int sfx_count) {
    // Load background
    Image bg = load_background("intro/assets/background1.png", renderer);
    for (int i = 0; i < dialog_count; i++) {
        display_text_image(renderer, font, dialog[i].text, dialog[i].emote_path, sfx, sfx_count, &bg);
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
        {"Test 1", "sad"},
        {"Test 2", "shocked"},
        {"Test 3ddkjzaezhioaeioeazhioazei ioueaziuoaeziuoaezuezaiuoiu", "sad"},
    };

    display_dialogue(renderer, font, dialog, 3, pencil_sfx, 4);
    free(pencil_sfx);
}

#endif