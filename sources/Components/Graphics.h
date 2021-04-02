#pragma once

// from Base
#include "Base/Rect.h"

// from SDL2
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>

typedef struct Components_Graphics
{
    Rect rect;
    SDL_Texture* texture;
} Components_Graphics;