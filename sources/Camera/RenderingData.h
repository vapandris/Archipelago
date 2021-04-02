#pragma once

// from SDL2
#include <SDL2/SDL_render.h>

// from Camera
#include "Camera.h"

typedef struct Camera_RenderingData
{
    SDL_Renderer* renderer;
    Camera* camera;
    int windowWidth, windowHeight;
} Camera_RenderingData;
