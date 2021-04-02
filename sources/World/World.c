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
#include "Components/Graphics.h"
#include "Components/Input.h"

struct World
{
    unsigned seed;
    Camera* camera;
};

World* World_Create(Camera* camera)
{
    World* result = malloc(sizeof *result);

    ECS_Init(1, 2,
        (ECS_ComponentData){.size = sizeof(Components_Graphics),    .signature = GRAPHICS_SIGNATURE},
        (ECS_ComponentData){.size = sizeof(Components_Input),       .signature = INPUT_SIGNATURE}
    );

    result->camera = camera;
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
    Components_Graphics graphics = {
        .rect = {.x = 50, .y = 50, .w = 48, .h = 96},
        .texture = TextureManager_GetPlayerTexture()
    };
    self->camera->x = (graphics.rect.x * 1.5) - (self->camera->w / 2.0);
    self->camera->y = (graphics.rect.y * 0.5) + (self->camera->h / 2.0);

    ECS_AddComponent(playerId, GRAPHICS_SIGNATURE, &graphics);
}


void World_DrawEntities(World* self, Camera_RenderingData* renderingData)
{
    ECS_QueryResult* query = ECS_Query(GRAPHICS_SIGNATURE);

    for(uint32_t id = 0; id < query->size; ++id) {
        Components_Graphics* graphics = ECS_GetComponent(id, GRAPHICS_SIGNATURE);
        SDL_Rect r = Camera_CalculateSDLRectFromRect(self->camera, renderingData->windowWidth, renderingData->windowHeight, &graphics->rect);
        SDL_RenderCopy(renderingData->renderer, graphics->texture, NULL, &r);
    }
}