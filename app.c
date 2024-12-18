#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define COLS 20
#define ROWS 20
#define WINDOW_TITLE "Snake Game"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define BLOCK_SIZE 25;


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
    int velocity_x;
    int velocity_y;
};

struct Food
{
    int x;
    int y;
    SDL_Rect rect;
};



bool sdl_initialize(struct Game *game);
void karakter_initialize(struct Karakter *karakter, SDL_Renderer *renderer);
void food_initialize(struct Food *food, SDL_Renderer *renderer);
void game_cleanup(struct Game *game, int exit_status);
void is_eat(struct Karakter *karakter, struct Food *food, SDL_Renderer *renderer);
void gameOver(struct Karakter *karakter, struct Game *game);

int main(int argc, char** argv) {
    struct Game game = {
        .window = NULL,
        .renderer = NULL
    };

    struct Karakter karakter = {
        .h = 50,
        .w = 50,
        .x = 50,
        .y = 50,
        .velocity_x = 0,
        .velocity_y = 0
    };

    struct Food food = {
        .x = 25 * 10,
        .y = 25 * 10,
        .rect = NULL
    };
    
    
    if (sdl_initialize(&game)) {
        game_cleanup(&game, EXIT_FAILURE);
        printf("Masalah");
        exit(1);
    }


    while (true)
    {
        SDL_Delay(200);
        karakter.x += karakter.velocity_x * BLOCK_SIZE;
        karakter.y += karakter.velocity_y * BLOCK_SIZE;

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
                        karakter.velocity_x = 0;
                        karakter.velocity_y = -1;
                        break;
                    case SDL_SCANCODE_A:
                        printf("A pressed!\n");
                        karakter.velocity_x = -1;
                        karakter.velocity_y = 0;
                        break;
                    case SDL_SCANCODE_S:
                        printf("S pressed!\n");
                        karakter.velocity_x = 0;
                        karakter.velocity_y = 1;
                        break;
                    case SDL_SCANCODE_D:
                        printf("D pressed!\n");
                        karakter.velocity_x = 1;
                        karakter.velocity_y = 0;
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
        food_initialize(&food, game.renderer);
        is_eat(&karakter, &food, game.renderer);
        gameOver(&karakter, &game);
        SDL_RenderPresent(game.renderer);
    }
    


    game_cleanup(&game, EXIT_SUCCESS);
    return 0;
}

void is_eat(struct Karakter *karakter, struct Food *food, SDL_Renderer *renderer) {
    if (karakter->x == food->x &&  karakter->y == food->y) {
        printf("Mati");
        SDL_RenderClear(renderer);
    }
}

void gameOver(struct Karakter *karakter, struct Game *game) {
    if (karakter->x < 0 || karakter->x >= WINDOW_WIDTH || karakter->y < 0 || karakter->y >= WINDOW_HEIGHT) {
        game_cleanup(game, EXIT_SUCCESS);
    }
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

void food_initialize(struct Food *food, SDL_Renderer *renderer) {

    // food->x = (rand() % (WINDOW_WIDTH / 5)) * 5;
    // food->y = (rand() % (WINDOW_HEIGHT / 5)) * 5;


    SDL_Rect rect;
    rect.h = 50;
    rect.w = 50;
    rect.x = food->x;
    rect.y = food->y;

    food->rect = rect;

    SDL_SetRenderDrawColor(renderer, 0, 128, 0, 0);
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