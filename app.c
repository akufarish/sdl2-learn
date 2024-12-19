#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#define COLS 20
#define ROWS 20
#define WINDOW_TITLE "Space Rocket"
#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 800
#define PESAWAT_PATH "res/img/pesawat.png"
#define BACKGROUND_IMG "res/img/bg.png"
#define AMMO_SOUND_EFFECT_PATH "res/audio/blaster.wav"
#define MAX_ENEMIES 10

typedef struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} Game;

typedef struct Karakter
{
    int x;
    int y;
    int w;
    int h;
    int velocity_x;
    int velocity_y;
} Karakter;

Karakter enemies[MAX_ENEMIES];


bool isAmmoActive = false;
int ammoLifeTime = 5000;
Uint16 score = 0;
Uint32 ammoSpawnTime = 0;
Mix_Chunk *ammo_sound_effect = NULL;
int enemy_count = 0;
Uint32 last_spawn_time = 0; 

bool sdl_initialize(Game *game);
void karakter_initialize(Karakter *karakter, Game *game);
void generate_ammo(Karakter *karakter, Karakter *peluru, Game *game, Karakter *musuh);
void game_cleanup(Game *game, int exit_status);
void game_over(Karakter *karakter, Game *game);
void icon_initialize( Game *game);
void load_sound_effect();
void enemy_spawn(Karakter *musuh, Game *game);
void spawn_enemy(Game *game);
void update_enemies(Game *game, Karakter *musuh);

int main(int argc, char** argv) {

    struct Game game = {
        .window = NULL,
        .renderer = NULL
    };

    Karakter karakter = {
        .h = 100,
        .w = 100,
        .x = 10,
        .y = 10,
        .velocity_x = 0,
        .velocity_y = 0
    } ;

    Karakter musuh = {
        .h = 0,
        .w = 0,
        .x = 250,
        .y = 10,
        .velocity_x = 0,
        .velocity_y = 0
    } ;

    Karakter peluru = {
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
    load_sound_effect();

    icon_initialize(&game);

    while (true) {
        SDL_ShowCursor(SDL_DISABLE);
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
                    karakter.x = x - karakter.w / 5;
                    karakter.y = y - karakter.h / 5;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch (event.button.button) {
                        case SDL_BUTTON_LEFT:
                            printf("Klik kiri\n");
                            isAmmoActive = true; 
                            ammoSpawnTime = SDL_GetTicks(); 
                            peluru.x = karakter.x + karakter.w / 4 - peluru.w / 5;
                            peluru.y = karakter.y - peluru.h; 
                            Mix_PlayChannel(-1, ammo_sound_effect, 0);
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

    SDL_Texture *bg = IMG_LoadTexture(game.renderer, BACKGROUND_IMG);
    SDL_RenderClear(game.renderer);
    SDL_RenderCopy(game.renderer, bg, NULL, NULL);

    karakter_initialize(&karakter, &game);

     if (isAmmoActive && (SDL_GetTicks() - ammoSpawnTime > ammoLifeTime)) {
        isAmmoActive = false;
    }

    if (isAmmoActive) {
        generate_ammo(&karakter, &peluru, &game, &musuh);
    }

    // enemy_spawn(&musuh, &game);

    Uint32 current_time = SDL_GetTicks();
    if (current_time > last_spawn_time + 1000) { // Spawn every 1 second
        spawn_enemy(&game);
        last_spawn_time = current_time;
    }

    update_enemies(&game, &musuh);
    game_over(&karakter, &game);
    SDL_DestroyTexture(bg);

    printf("%i\n", score);
    SDL_RenderPresent(game.renderer);

    SDL_Delay(16);
}
    game_cleanup(&game, EXIT_SUCCESS);
    return 0;
}


void game_over(Karakter *karakter, Game *game) {
  if (karakter->x < 0) {
        karakter->x = 0; 
    } else if (karakter->x > WINDOW_WIDTH - 100) { 
        karakter->x = WINDOW_WIDTH - 100; 
    }

    if (karakter->y < 0) {
        karakter->y = 0; 
    } else if (karakter->y > WINDOW_HEIGHT - 100) { 
        karakter->y = WINDOW_HEIGHT - 100; 
    }
}


void karakter_initialize(Karakter *karakter, Game *game) {
    SDL_Rect rect;
    SDL_Texture *img = IMG_LoadTexture(game->renderer, PESAWAT_PATH);
    SDL_QueryTexture(img, NULL, NULL, &karakter->w, &karakter->h);

    rect.h = karakter->h - 100;
    rect.w = karakter->w - 100;
    rect.x = karakter->x;
    rect.y = karakter->y;
    SDL_RenderCopy(game->renderer, img, NULL, &rect);
}

void enemy_spawn(Karakter *musuh, Game *game) {
    SDL_Rect rect;
    SDL_Texture *img = IMG_LoadTexture(game->renderer, PESAWAT_PATH);
    SDL_QueryTexture(img, NULL, NULL, &musuh->w, &musuh->h);

    rect.h = musuh->h - 100;
    rect.w = musuh->w - 100;
    rect.x = rand() % WINDOW_WIDTH;
    rect.y = musuh->y;
    double angle = 0.0;
    SDL_RenderCopyEx(game->renderer, img, NULL, &rect, angle, NULL, SDL_FLIP_VERTICAL);

    musuh->y += 5;

    if (musuh->y >= 800) {
        musuh->y = 0;
    }
}


void generate_ammo(Karakter *karakter, Karakter *peluru, Game *game, Karakter *musuh) {
    static bool hasHit = false; 

    SDL_Rect rect;
    rect.h = peluru->h;
    rect.w = peluru->w;
    rect.x = peluru->x;
    rect.y = peluru->y;

    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(game->renderer, &rect);

    peluru->y -= 20;

    if (
        peluru->x <= musuh->x + musuh->w &&
        peluru->x + peluru->w >= musuh->x &&
        peluru->y <= musuh->y + musuh->h &&
        peluru->y + peluru->h >= musuh->y
    ) {
        if (!hasHit) {
            printf("Hit\n");
            score++;
            hasHit = true;
        }
    } else {
        hasHit = false;
    }
}


void game_cleanup(Game *game, int exit_status) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    Mix_FreeChunk(ammo_sound_effect);
    SDL_Quit();
    exit(exit_status);
}

bool sdl_initialize(Game *game) {
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
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)) {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return true;
    }

    return false;
}

void icon_initialize(Game *game) {
    SDL_Surface *icon = IMG_Load(PESAWAT_PATH);
    SDL_SetWindowIcon(game->window, icon);
}

void load_sound_effect() {
    ammo_sound_effect = Mix_LoadWAV(AMMO_SOUND_EFFECT_PATH);

    if (ammo_sound_effect == NULL) {
        printf("Failed to load sound effect %s", Mix_GetError());
    }
}

void spawn_enemy(Game *game) {
    if (enemy_count >= MAX_ENEMIES) return;

    enemies[enemy_count] = (Karakter){
        .h = 100,
        .w = 100,
        .x = rand() % WINDOW_WIDTH,
        .y = -50,                  
        .velocity_x = 0,
        .velocity_y = 5            
    };
    enemy_count++;
}

void update_enemies(Game *game, Karakter *musuh) {
    SDL_Rect rect;
    SDL_Texture *img = IMG_LoadTexture(game->renderer, PESAWAT_PATH);
    SDL_QueryTexture(img, NULL, NULL, &musuh->w, &musuh->h);

    for (int i = 0; i < enemy_count; i++) {
        enemies[i].y += enemies[i].velocity_y;

        if (enemies[i].y < WINDOW_HEIGHT) {
            SDL_Rect rect = {enemies[i].x, enemies[i].y, enemies[i].w, enemies[i].h};
            SDL_RenderCopyEx(game->renderer, img, NULL, &rect, 0.0, NULL, SDL_FLIP_VERTICAL);
        } else {
            enemies[i] = enemies[enemy_count - 1];
            enemy_count--;
            i--; 
        }
    }
}