// from SDL2
#include <SDL2/SDL.h>

// from ECS
#include "ECS/ECS.h"

// from View
#include "View/GameView.h"

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    View_GameView* gameView = View_GameView_Create();

    View_GameView_Loop(gameView);

    View_GameView_Destroy(gameView);

    SDL_Quit();
    return 0;
}