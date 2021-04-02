#pragma once

// from SDL
#include <SDL2/SDL_stdinc.h>

// from ECS
#include "ECS/ECS.h"

// from Camera
#include "Camera/RenderingData.h"

void World_EntityActions_DrawEntites(ECS_EntityStore* entities, Camera_RenderingData* renderingData);
void World_EntityActions_ProcessInput(ECS_EntityStore* entities, const Uint8* keyboardState);
void World_EntityActions_UpdateEntities(ECS_EntityStore* entities);