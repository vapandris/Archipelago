#pragma once

#include <SDL2/SDL_render.h>

void    TextureManager_Init(SDL_Renderer* renderer);
void    TextureManager_Quit();

SDL_Texture* TextureManager_GetPlayerTexture();