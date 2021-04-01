#pragma once

// from SDL2
#include <SDL2/SDL_render.h>

typedef struct Components_Graphics
{
    double x, y;
    double w, h;
    SDL_Texture* texture;
} Components_Graphics;