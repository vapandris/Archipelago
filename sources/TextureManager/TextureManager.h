#pragma once

#include <SDL2/SDL_render.h>

void    TextureManager_Init(SDL_Renderer* renderer);
void    TextureManager_Quit();

SDL_Texture* TextureManager_GetPlayerTexture();
SDL_Texture* TextureManager_GetLightGrassTexture();
SDL_Texture* TextureManager_GetSkyTexture();
SDL_Texture* TextureManager_GetPineTexture();
SDL_Texture* TextureManager_GetFlowersTexture();