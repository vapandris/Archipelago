#include "EntityActions.h"

// from Base
#include "Base/Types.h"

// from SDL2
#include <SDL2/SDL_scancode.h>

// from Components
#include "Components/Signatures.h"
#include "Components/Collision.h"
#include "Components/Graphics.h"
#include "Components/Input.h"


void World_EntityActions_DrawEntites(ECS_EntityStore* entities, Camera_RenderingData* renderingData)
{
    ECS_QueryResult* query = ECS_EntityStore_Query(entities, GRAPHICS_SIGNATURE);

    for(UInt32 i = 0; i < query->size; ++i) {
        EntityId id = query->entityIdList[i];
        Components_Graphics* graphics = ECS_EntityStore_GetComponent(entities, id, GRAPHICS_SIGNATURE);
        SDL_Rect r = Camera_CalculateSDLRectFromRect(renderingData->camera, renderingData->windowWidth, renderingData->windowHeight, &graphics->rect);
        SDL_RenderCopy(renderingData->renderer, graphics->texture, NULL, &r);
    }

    ECS_QueryResult_Destroy(query);

    query = ECS_EntityStore_Query(entities, COLLISION_SIGNATURE);
    for(UInt32 i = 0; i < query->size; ++i) {
        EntityId id = query->entityIdList[i];
        Components_Collision* collision = ECS_EntityStore_GetComponent(entities, id, COLLISION_SIGNATURE);
        SDL_Rect r = Camera_CalculateSDLRectFromRect(renderingData->camera, renderingData->windowWidth, renderingData->windowHeight, &collision->hitBox);
        SDL_RenderFillRect(renderingData->renderer, &r);
    }
    ECS_QueryResult_Destroy(query);
}


void World_EntityActions_ProcessInput(ECS_EntityStore* entities, const Uint8* keyboardState)
{
    ECS_QueryResult* query = ECS_EntityStore_Query(entities, INPUT_SIGNATURE);

    for(UInt32 i = 0; i < query->size; ++i) {
        EntityId id = query->entityIdList[i];
        Components_Input* input = ECS_EntityStore_GetComponent(entities, id, INPUT_SIGNATURE);
        if(keyboardState[SDL_SCANCODE_W])
            input->y = 5;

        if(keyboardState[SDL_SCANCODE_S])
            input->y = -5;

        if(keyboardState[SDL_SCANCODE_A])
            input->x = -5;

        if(keyboardState[SDL_SCANCODE_D])
            input->x = 5;
    }

    ECS_QueryResult_Destroy(query);
}

#include <stdio.h>
void World_EntityActions_UpdateEntities(ECS_EntityStore* entities, Camera* camera)
{
    ECS_QueryResult* query = ECS_EntityStore_Query(entities, INPUT_SIGNATURE | GRAPHICS_SIGNATURE | COLLISION_SIGNATURE);

    for(UInt32 i = 0; i < query->size; ++i) {
        EntityId id = query->entityIdList[i];
        Components_Graphics* graphics = ECS_EntityStore_GetComponent(entities, id, GRAPHICS_SIGNATURE);
        Components_Collision* collision = ECS_EntityStore_GetComponent(entities, id, COLLISION_SIGNATURE);
        Components_Input* input = ECS_EntityStore_GetComponent(entities, id, INPUT_SIGNATURE);

        graphics->rect.x += input->x;
        graphics->rect.y += input->y;

        collision->hitBox.x += input->x;
        collision->hitBox.y += input->y;

        // Doesn't seem like a bright idea:
        {
            camera->x += input->x;
            camera->y += input->y;
        }

        if(input->x != 0 || input->y != 0) {
            ECS_QueryResult* query = ECS_EntityStore_Query(entities, COLLISION_SIGNATURE);

            Rect* movedRect = &collision->hitBox;
            for(UInt32 i = 0; i < query->size; ++i) {
                EntityId otherId = query->entityIdList[i];
                if(otherId == id)
                    continue;

                Rect* otherRect = &((Components_Collision*)ECS_EntityStore_GetComponent(entities, otherId, COLLISION_SIGNATURE))->hitBox;

                if(movedRect->x < otherRect->x + otherRect->w &&
                   movedRect->x + movedRect->w > otherRect->x &&
                   movedRect->y > otherRect->y - otherRect->h &&
                   movedRect->y - movedRect->h < otherRect->y)
                {
                    if(input->y > 0 && input->x == 0) {
                        double newY = otherRect->y - otherRect->h;
                        double delta = movedRect->y - newY;

                        movedRect->y -= delta;
                        graphics->rect.y -= delta;
                        camera->y -= delta;
                    } else if(input->y < 0 && input->x == 0) {
                        double newY = otherRect->y + movedRect->h;
                        double delta = newY - movedRect->y;

                        movedRect->y += delta;
                        graphics->rect.y += delta;
                        camera->y += delta;
                    } else if(input->x > 0 && input->y == 0) {
                        double newX = otherRect->x - movedRect->w;
                        double delta = movedRect->x - newX;

                        movedRect->x -= delta;
                        graphics->rect.x -= delta;
                        camera->x -= delta;
                    } else if(input->x < 0 && input->y == 0) {
                        double newX = otherRect->x + otherRect->w;
                        double delta = newX - movedRect->x;

                        movedRect->x += delta;
                        graphics->rect.x += delta;
                        camera->x += delta;
                    } else if(input->y > 0 && input->x > 0) {

                    } else if(input->y > 0 && input->x < 0) {

                    } else if(input->y < 0 && input->x > 0) {

                    } else if(input->y < 0 && input->x < 0) {
                        
                    }
                }
            }

            ECS_QueryResult_Destroy(query);
        }

        input->x = 0;
        input->y = 0;
    }

    ECS_QueryResult_Destroy(query);
}