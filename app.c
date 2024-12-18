#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_TITLE "01 Open Window"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
};

bool sdl_initialize(struct Game *game);
void game_cleanup(struct Game *game, int exit_status);

int main(int argc, char** argv) {
    struct Game game = {
        .window = NULL,
        .renderer = NULL
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
                        break;
                    case SDL_SCANCODE_A:
                        printf("A pressed!\n");
                        break;
                    case SDL_SCANCODE_S:
                        printf("S pressed!\n");
                        break;
                    case SDL_SCANCODE_D:
                        printf("D pressed!\n");
                        break;
                    default:
                        break;
                }

            default:
                break;
            }
        }
        
        SDL_RenderClear(game.renderer);
        SDL_RenderPresent(game.renderer);
    }
    


    game_cleanup(&game, EXIT_SUCCESS);
    return 0;
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
        WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

    if (!game->window) {
        fprintf(stderr, "Error create window SDL: '%s'\n", SDL_GetError());
        return true;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, 0);

    if (!game->renderer) {
        fprintf(stderr, "Error create window SDL: '%s'\n", SDL_GetError());
        return true;
    }

    return false;
}