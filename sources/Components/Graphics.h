#pragma once

// from Base
#include "Base/Rect.h"

// from SDL2
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Components_Layer
{
    Ground = 0,
    GroundDecorations = 1,
    AboveGround = 2
} Components_Layer;

typedef struct Components_Graphics
{
    Rect rect;
    SDL_Texture* texture;
    Components_Layer layer;
} Components_Graphics;

#ifdef __cplusplus
}
#endif