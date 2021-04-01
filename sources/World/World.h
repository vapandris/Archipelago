
#pragma once

// from SDL2
#include <SDL2/SDL_render.h>

typedef struct World World;

World*  World_Create();
void    World_Destroy(World* self);

void World_Generate(World* self, unsigned seed);
void World_DrawEntities(World* self, SDL_Renderer* renderer);
