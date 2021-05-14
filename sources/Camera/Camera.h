#pragma once

// from Base
#include "Base/Rect.h"
#include "Base/Types.h"

// from SDL2
#include <SDL2/SDL_rect.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef Rect Camera;

Rect        Camera_CalculateRectFromSDLRect(const Camera* camera, UInt16 windowWidth, UInt16 windowHeight, const SDL_Rect* rect);
SDL_Rect    Camera_CalculateSDLRectFromRect(const Camera* camera, UInt16 windowWidth, UInt16 windowHeight, const Rect* rect);

#ifdef __cplusplus
}
#endif