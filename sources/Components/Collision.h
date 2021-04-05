#pragma once

// from Base
#include "Base/Rect.h"

typedef enum Components_CollisionType
{
    SOLID
} Components_CollisionType;

typedef struct Components_Collision
{
    Rect hitBox;
    Components_CollisionType type;
} Components_Collision;