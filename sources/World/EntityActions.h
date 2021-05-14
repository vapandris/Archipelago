#pragma once

// from SDL
#include <SDL2/SDL_stdinc.h>

// from Base
#include "Base/Point.h"

// from ECS
#include "ECS/ECS.h"

// from Camera
#include "Camera/Camera.h"
#include "Camera/RenderingData.h"

#ifdef __cplusplus
extern "C" {
#endif

void World_EntityActions_DrawEntites(ECS_EntityStore* entities, Camera_RenderingData* renderingData);
void World_EntityActions_ProcessInput(ECS_EntityStore* entities, const Uint8* keyboardState);
void World_EntityActions_UpdateEntities(ECS_EntityStore* entities, Camera* camera);

// EntityCreation functions
EntityId World_EntityActions_CreatePlayer(ECS_EntityStore* entities, Point* topLeft);
EntityId World_EntityActions_CreateGrassTile(ECS_EntityStore* entities, Point* topLeft);
EntityId World_EntityActions_CreateVoidTile(ECS_EntityStore* entities, Point* topLeft);
EntityId World_EntityActions_CreateTree(ECS_EntityStore* entities, Point* topLeft);
EntityId World_EntityActions_CreateFlowers(ECS_EntityStore* entities, Point* topLeft);

#ifdef __cplusplus
}
#endif