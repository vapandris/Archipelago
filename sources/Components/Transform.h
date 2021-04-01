#pragma once

/*typedef enum Components_Direction
{
    UNDEFINED = -1,
    LEFT,       RIGHT,
    Up,         DOWN,
    LEFT_UP,    LEFT_DOWN,
    RIGHT_UP,   RIGHT_DOWN
} Components_Direction;*/

typedef struct Components_Transform
{
    double x, y;
    //Components_Direction dir;
} Components_Transform;