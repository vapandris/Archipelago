
#pragma once

// from Base
#include "Camera/Camera.h"
#include "Camera/RenderingData.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct World World;

World*  World_Create(Camera* camera);
void    World_Destroy(World* self);

void World_Generate(World* self, unsigned seed);

void World_DrawEntities(World* self, Camera_RenderingData* renderingData);
void World_ProcessInput(World* self, const Uint8* keyboardState);
void World_UpdateEntities(World* self, Camera* camera);

#ifdef __cplusplus
}
#endif