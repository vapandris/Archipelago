#include "World.h"

// from std
#include <stdlib.h>
#include <stdbool.h>

// from ECS
#include "ECS/ECS.h"

// from TextureManager
#include "TextureManager/TextureManager.h"

// from Components
#include "Components/Signatures.h"
#include "Components/Transform.h"
#include "Components/Graphics.h"
#include "Components/Input.h"

struct World
{
    unsigned seed;
};

World* World_Create()
{
    World* result = malloc(sizeof *result);

    ECS_Init(1, 3,
        (ECS_ComponentData){.size = sizeof(Components_Transform),   .signature = TRANSFORM_SIGNATURE},
        (ECS_ComponentData){.size = sizeof(Components_Graphics),    .signature = GRAPHICS_SIGNATURE},
        (ECS_ComponentData){.size = sizeof(Components_Input),       .signature = INPUT_SIGNATURE}
    );

    return result;
}


void World_Destroy(World* self)
{
    ECS_Quit();
    free(self);
}


void World_Generate(World* self, unsigned seed)
{
    self->seed = seed;
    uint32_t playerId = ECS_CreateEntity();
    Components_Transform transform = {.x = 50, .y = 50};
    Components_Graphics graphics = {
        .x = 50, .y = 50,
        .w = 24, .h = 48,
        .texture = TextureManager_GetPlayerTexture()
    };
    ECS_AddComponent(playerId, TRANSFORM_SIGNATURE, &transform);
    ECS_AddComponent(playerId, GRAPHICS_SIGNATURE, &graphics);
}


void World_DrawEntities(World* self, SDL_Renderer* renderer)
{
    ECS_QueryResult* query = ECS_Query(GRAPHICS_SIGNATURE);

    for(uint32_t id = 0; id < query->size; ++id) {
        Components_Graphics* graphics = ECS_GetComponent(id, GRAPHICS_SIGNATURE);
        SDL_Rect r = {.x = graphics->x, .y = graphics->y, .w = graphics->w, .h = graphics->h};
        SDL_RenderCopy(renderer, graphics->texture, NULL, &r);
    }
}