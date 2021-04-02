#include "Camera.h"

// from std
#include <math.h>

Rect Camera_CalculateRectFromSDLRect(const Camera* camera, UInt16 windowWidth, UInt16 windowHeight, const SDL_Rect* rect)
{
    Rect result;

    double xDelta = windowWidth / camera->w;
    double yDelta = windowHeight / camera->h;

    result.x = (rect->x / xDelta) + camera->x;
    result.y = camera->y - (rect->y / yDelta);
    result.w = rect->w / xDelta;
    result.h = rect->h / yDelta;

    return result;
}


SDL_Rect Camera_CalculateSDLRectFromRect(const Camera* camera, UInt16 windowWidth, UInt16 windowHeight, const Rect* rect)
{
    SDL_Rect result;

    double xDelta = windowWidth / camera->w;
    double yDelta = windowHeight / camera->h;

    result.x = ceil((rect->x - camera->x) * xDelta);
    result.y = ceil((rect->y - camera->y) * -yDelta);
    result.w = ceil(rect->w * xDelta);
    result.h = ceil(rect->h * yDelta);

    return result;
}