
#pragma once

// from Base
#include "Camera/Camera.h"
#include "Camera/RenderingData.h"

typedef struct World World;

World*  World_Create(Camera* camera);
void    World_Destroy(World* self);

void World_Generate(World* self, unsigned seed);

void World_DrawEntities(World* self, Camera_RenderingData* renderingData);
void World_ProcessInput(World* self, const Uint8* keyboardState);
void World_UpdateEntities(World* self, Camera* camera);