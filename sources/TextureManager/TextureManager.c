#include "TextureManager.h"

// from std
#include <stdbool.h>
#include <assert.h>

// from SDL2
#include <SDL2/SDL_image.h>


typedef struct TextureManager
{
    SDL_Texture* player;
} TextureManager;

static TextureManager textureManager;


void TextureManager_Init(SDL_Renderer* renderer)
{
    textureManager.player = IMG_LoadTexture(renderer, "assets/Player/Player_tmp.png");
    assert(textureManager.player != NULL);
}


void TextureManager_Quit()
{
    SDL_DestroyTexture(textureManager.player);
}

// =========================
// ======== Getters ========
// =========================

SDL_Texture* TextureManager_GetPlayerTexture() { return textureManager.player; }