#include "GameView.h"

// from std
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

// from SDL2
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_stdinc.h>

// from Camera
#include "Camera/Camera.h"
#include "Camera/RenderingData.h"

// from TextureManager
#include "TextureManager/TextureManager.h"

// from World
#include "World/World.h"

#define MS_PER_UPDATE 45
#define MAX_FPS 100

struct View_GameView
{
    Camera camera;
    SDL_Window* window;
    SDL_Renderer* renderer;
    World* world;
};

static bool ProcessInput(View_GameView* self);

View_GameView* View_GameView_Create()
{
    View_GameView* result = malloc(sizeof *result);

    result->window = SDL_CreateWindow(
        "Archipelago",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        720, 480,
        SDL_WINDOW_MAXIMIZED | SDL_WINDOW_FULLSCREEN_DESKTOP
    );

    result->renderer = SDL_CreateRenderer(result->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    TextureManager_Init(result->renderer);

    int w, h;
    SDL_GetWindowSize(result->window, &w, &h);
    double screenRatio = (double)w / h;
    result->camera.w = 640.0;
    result->camera.h = result->camera.w / screenRatio;

    result->world = World_Create(&result->camera);

    return result;
}


void View_GameView_Destroy(View_GameView* self)
{
    SDL_DestroyWindow(self->window);
    SDL_DestroyRenderer(self->renderer);
    World_Destroy(self->world);
    TextureManager_Quit();
    free(self);
}

void View_GameView_Loop(View_GameView* self)
{
    srand(time(NULL));
    World_Generate(self->world, rand());

    Camera_RenderingData renderingData = {
        .camera = &self->camera,
        .renderer = self->renderer
    };

    SDL_GetWindowSize(self->window, &renderingData.windowWidth, &renderingData.windowHeight);

    bool done = false;
    Uint32 previous = SDL_GetTicks();
    Uint32 lag = 0;
    do {
        Uint32 current = SDL_GetTicks();
        Uint32 elapsed = current - previous;
        previous = current;
        lag += elapsed;

        SDL_SetRenderDrawColor(self->renderer, 94, 158, 196, 255);
        SDL_RenderClear(self->renderer);

        done = ProcessInput(self);

        while(lag >= MS_PER_UPDATE) {
            World_UpdateEntities(self->world, &self->camera);
            lag -= MS_PER_UPDATE;
        }

        World_DrawEntities(self->world, &renderingData);
        SDL_RenderPresent(self->renderer);
        //SDL_Delay();
    } while(!done);
}

// static functions:
static bool ProcessInput(View_GameView* self)
{
    assert(self != NULL);
    bool done = false;

    SDL_Event event;
    while(SDL_PollEvent(&event) == true) {
        if(event.type == SDL_WINDOWEVENT_CLOSE ||
           event.type == SDL_QUIT ||
           (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) {
            done = true;
        }
    }
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    World_ProcessInput(self->world, keyboardState);

    return done;
}