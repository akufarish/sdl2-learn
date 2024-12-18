#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_TITLE "Testing SDL"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
};

struct Karakter
{
    int x;
    int y;
    int w;
    int h;
};


bool sdl_initialize(struct Game *game);
void karakter_initialize(struct Karakter *karakter, SDL_Renderer *renderer);
void game_cleanup(struct Game *game, int exit_status);

int main(int argc, char** argv) {
    struct Game game = {
        .window = NULL,
        .renderer = NULL
    };

    struct Karakter karakter = {
        .h = 50,
        .w = 50,
        .x = 50,
        .y = 50
    };
    
    if (sdl_initialize(&game)) {
        game_cleanup(&game, EXIT_FAILURE);
        printf("Masalah");
        exit(1);
    }


    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                game_cleanup(&game, EXIT_SUCCESS);
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                        printf("W pressed!\n");
                        karakter.y -= 5;
                        break;
                    case SDL_SCANCODE_A:
                        printf("A pressed!\n");
                        karakter.x -= 5;
                        break;
                    case SDL_SCANCODE_S:
                        printf("S pressed!\n");
                        karakter.y += 5;
                        break;
                    case SDL_SCANCODE_D:
                        printf("D pressed!\n");
                        karakter.x += 5;
                        break;
                    default:
                        break;
                }

            default:
                break;
            }
        }
        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        SDL_RenderClear(game.renderer);
        karakter_initialize(&karakter, game.renderer);
        SDL_RenderPresent(game.renderer);
    }
    


    game_cleanup(&game, EXIT_SUCCESS);
    return 0;
}

void karakter_initialize(struct Karakter *karakter, SDL_Renderer *renderer) {

    SDL_Rect rect;
    rect.h = karakter->h;
    rect.w = karakter->w;
    rect.x = karakter->x;
    rect.y = karakter->y;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void game_cleanup(struct Game *game, int exit_status) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
    exit(exit_status);
}

bool sdl_initialize(struct Game *game) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "Error initializing SDL: '%s'\n", SDL_GetError());
        return true;
    }

    game->window =  SDL_CreateWindow(
        WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (!game->window) {
        fprintf(stderr, "Error create window SDL: '%s'\n", SDL_GetError());
        return true;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);

    if (!game->renderer) {
        fprintf(stderr, "Error create window SDL: '%s'\n", SDL_GetError());
        return true;
    }


    return false;
}