#pragma once

// from SDL2
#include <SDL2/SDL_render.h>

// from Camera
#include "Camera.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Camera_RenderingData
{
    SDL_Renderer* renderer;
    Camera* camera;
    int windowWidth, windowHeight;
} Camera_RenderingData;

#ifdef __cplusplus
}
#endif