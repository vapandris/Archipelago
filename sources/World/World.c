#include "World.h"

// from std
#include <stdlib.h>
#include <stdbool.h>

// from Base
#include "Base/Types.h"

// from ECS
#include "ECS/ECS.h"

// from TextureManager
#include "TextureManager/TextureManager.h"

// from Components
#include "Components/Signatures.h"
#include "Components/Graphics.h"
#include "Components/Input.h"

// from World
#include "World/EntityActions.h"

// from SDL2
#include <SDL2/SDL_scancode.h>

struct World
{
    unsigned seed;
    Camera* camera;
    ECS_EntityStore* entities;
};

World* World_Create(Camera* camera)
{
    World* result = malloc(sizeof *result);

    result->entities = ECS_EntityStore_Create(1, 2,
        (ECS_ComponentData){.size = sizeof(Components_Graphics),    .signature = GRAPHICS_SIGNATURE},
        (ECS_ComponentData){.size = sizeof(Components_Input),       .signature = INPUT_SIGNATURE}
    );

    result->camera = camera;
    return result;
}


void World_Destroy(World* self)
{
    ECS_EntityStore_Destroy(self->entities);
    free(self);
}


void World_Generate(World* self, unsigned seed)
{
    self->seed = seed;
    EntityId playerId = ECS_EntityStore_CreateEntity(self->entities);
    Components_Graphics graphics = {
        .rect = {.x = 50, .y = 50, .w = 48, .h = 96},
        .texture = TextureManager_GetPlayerTexture()
    };
    self->camera->x = (graphics.rect.x * 1.5) - (self->camera->w / 2.0);
    self->camera->y = (graphics.rect.y * 0.5) + (self->camera->h / 2.0);

    ECS_EntityStore_AddComponent(self->entities, playerId, GRAPHICS_SIGNATURE, &graphics);
    ECS_EntityStore_AddComponent(self->entities, playerId, INPUT_SIGNATURE, &(Components_Input){.x = 0, .y = 0});
}


void World_DrawEntities(World* self, Camera_RenderingData* renderingData)
{
    World_EntityActions_DrawEntites(self->entities, renderingData);
}


void World_ProcessInput(World* self, const Uint8* keyboardState)
{
    World_EntityActions_ProcessInput(self->entities, keyboardState);
}


void World_UpdateEntities(World* self)
{
    World_EntityActions_UpdateEntities(self->entities);
}