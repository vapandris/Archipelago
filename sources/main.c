#include <SDL2/SDL.h>
#include "ECS/ECS.h"

typedef struct _ {
    int a;
} _;

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    ECS_Init(1, 1,
        (ECS_ComponentData){.size = sizeof(_), .signature = 0x1}
    );
    ECS_Quit();

    SDL_Quit();
    return 0;
}