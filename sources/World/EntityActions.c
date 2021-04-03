#include "EntityActions.h"

// from SDL2
#include <SDL2/SDL_scancode.h>

// from Components
#include "Components/Signatures.h"
#include "Components/Graphics.h"
#include "Components/Input.h"


void World_EntityActions_DrawEntites(ECS_EntityStore* entities, Camera_RenderingData* renderingData)
{
    ECS_QueryResult* query = ECS_EntityStore_Query(entities, GRAPHICS_SIGNATURE);

    for(UInt32 id = 0; id < query->size; ++id) {
        Components_Graphics* graphics = ECS_EntityStore_GetComponent(entities, id, GRAPHICS_SIGNATURE);
        SDL_Rect r = Camera_CalculateSDLRectFromRect(renderingData->camera, renderingData->windowWidth, renderingData->windowHeight, &graphics->rect);
        SDL_RenderCopy(renderingData->renderer, graphics->texture, NULL, &r);
    }
}


void World_EntityActions_ProcessInput(ECS_EntityStore* entities, const Uint8* keyboardState)
{
    ECS_QueryResult* query = ECS_EntityStore_Query(entities, INPUT_SIGNATURE);

    for(EntityId id = 0; id < query->size; ++id) {
        Components_Input* input = ECS_EntityStore_GetComponent(entities, query->entityIdList[id], INPUT_SIGNATURE);
        if(keyboardState[SDL_SCANCODE_W])
            input->y = 5;

        if(keyboardState[SDL_SCANCODE_S])
            input->y = -5;

        if(keyboardState[SDL_SCANCODE_A])
            input->x = -5;

        if(keyboardState[SDL_SCANCODE_D])
            input->x = 5;
    }
}


void World_EntityActions_UpdateEntities(ECS_EntityStore* entities)
{
    ECS_QueryResult* query = ECS_EntityStore_Query(entities, ANY_COMPONENTS);

    for(EntityId id = 0; id < query->size; ++id) {
        if(ECS_EntityStore_HasComponents(entities, id, INPUT_SIGNATURE | GRAPHICS_SIGNATURE)) {
            Components_Graphics* graphics = ECS_EntityStore_GetComponent(entities, id, GRAPHICS_SIGNATURE);
            Components_Input* input = ECS_EntityStore_GetComponent(entities, id, INPUT_SIGNATURE);

            graphics->rect.x += input->x;
            graphics->rect.y += input->y;

            input->x = 0;
            input->y = 0;
        }
    }
}