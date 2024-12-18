#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL_image.h>

#define COLS 20
#define ROWS 20
#define WINDOW_TITLE "Snake Game"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define BLOCK_SIZE 25;
#define PESAWAT_PATH "res/img/pesawat.png"

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


bool sdl_initialize(struct Game *game);
void karakter_initialize(struct Karakter *karakter, SDL_Renderer *renderer);
void generateAmmo(struct Karakter *karakter, struct Karakter *peluru, SDL_Renderer *renderer);
void game_cleanup(struct Game *game, int exit_status);

int main(int argc, char** argv) {
    struct Game game = {
        .window = NULL,
        .renderer = NULL
    };

    struct Karakter karakter = {
        .h = 10,
        .w = 10,
        .x = 50,
        .y = 50,
        .velocity_x = 0,
        .velocity_y = 0
    };

    struct Karakter peluru = {
        .h = 100,
        .w = 10,
        .x = 50,
        .y = 50,
        .velocity_x = 0,
        .velocity_y = 0
    };
    
    if (sdl_initialize(&game)) {
        game_cleanup(&game, EXIT_FAILURE);
        printf("Masalah");
        exit(1);
    }


        bool isAmmoActive = false; 

while (true) {
    SDL_Delay(200);
    peluru.y += peluru.velocity_y * BLOCK_SIZE;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_cleanup(&game, EXIT_SUCCESS);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        game_cleanup(&game, EXIT_SUCCESS);
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                int x, y;
                SDL_GetMouseState(&x, &y);
                karakter.x = x;
                karakter.y = y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        printf("Klik kiri\n");
                        isAmmoActive = true; 
                        peluru.x = karakter.x + karakter.w / 2 - peluru.w / 2; 
                        peluru.y = karakter.y - peluru.h; 
                        peluru.velocity_x = 0;
                        peluru.velocity_y = -1;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    // Clear screen
    SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
    SDL_RenderClear(game.renderer);

    karakter_initialize(&karakter, game.renderer);

    if (isAmmoActive) {
        generateAmmo(&karakter, &peluru, game.renderer);
    }

    gameOver(&karakter, &game);

    SDL_RenderPresent(game.renderer);
}

    


    game_cleanup(&game, EXIT_SUCCESS);
    return 0;
}


void gameOver(struct Karakter *karakter, struct Game *game) {
    if (karakter->x < 0 || karakter->x >= WINDOW_WIDTH || karakter->y < 0 || karakter->y >= WINDOW_HEIGHT) {
        game_cleanup(game, EXIT_SUCCESS);
    }
}


void karakter_initialize(struct Karakter *karakter, SDL_Renderer *renderer) {

    SDL_Rect rect;
    SDL_Texture *img = IMG_LoadTexture(renderer, PESAWAT_PATH);
    SDL_QueryTexture(img, NULL, NULL, &karakter->w, &karakter->h);

    rect.h = karakter->h;
    rect.w = karakter->w;
    rect.x = karakter->x;
    rect.y = karakter->y;

    SDL_RenderCopy(renderer, img, NULL, &rect);
}


void generateAmmo(struct Karakter *karakter, struct Karakter *peluru, SDL_Renderer *renderer) {
     SDL_Rect rect;
    rect.h = peluru->h;
    rect.w = peluru->w;
    rect.x = peluru->x;
    rect.y = peluru->y;

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