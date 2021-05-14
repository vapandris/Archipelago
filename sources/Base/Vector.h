#pragma once

// from Base
#include "Base/Point.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Vector
{
    Point start;
    Point dir;
} Vector;

#ifdef __cplusplus
}
#endif