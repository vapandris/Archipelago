#include "TextureManager.h"

// from std
#include <stdbool.h>
#include <assert.h>

// from SDL2
#include <SDL2/SDL_image.h>


typedef struct TextureManager
{
    SDL_Texture* player;
    SDL_Texture* lightGrass;
    SDL_Texture* sky;
    SDL_Texture* pine;
    SDL_Texture* flowers;
} TextureManager;

static TextureManager textureManager;


void TextureManager_Init(SDL_Renderer* renderer)
{
    textureManager.player = IMG_LoadTexture(renderer, "assets/Player/Player_tmp.png");
    assert(textureManager.player != NULL);
    textureManager.lightGrass = IMG_LoadTexture(renderer, "assets/Tiles/light_grass_1.png");
    assert(textureManager.lightGrass != NULL);
    textureManager.sky = IMG_LoadTexture(renderer, "assets/Tiles/void_1.png");
    assert(textureManager.sky != NULL);
    textureManager.pine = IMG_LoadTexture(renderer, "assets/Tree_1.png");
    assert(textureManager.pine != NULL);
    textureManager.flowers = IMG_LoadTexture(renderer, "assets/Yellow_Flowers_1.png");
    assert(textureManager.flowers != NULL);
}


void TextureManager_Quit()
{
    SDL_DestroyTexture(textureManager.player);
    SDL_DestroyTexture(textureManager.lightGrass);
    SDL_DestroyTexture(textureManager.sky);
}

// =========================
// ======== Getters ========
// =========================

SDL_Texture* TextureManager_GetPlayerTexture() { return textureManager.player; }
SDL_Texture* TextureManager_GetLightGrassTexture() { return textureManager.lightGrass; }
SDL_Texture* TextureManager_GetSkyTexture() { return textureManager.sky; }
SDL_Texture* TextureManager_GetPineTexture() { return textureManager.pine; }
SDL_Texture* TextureManager_GetFlowersTexture() { return textureManager.flowers; }