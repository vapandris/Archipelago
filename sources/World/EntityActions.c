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

// from Collision
#include "Collision/CollisionDetection.h"

// from Pathfinding
#include "Pathfinding/AStar.h"


#include <stdio.h>

void World_EntityActions_DrawEntites(ECS_EntityStore* entities, Camera_RenderingData* renderingData)
{
    ECS_QueryResult* query = ECS_EntityStore_Query(entities, GRAPHICS_SIGNATURE);


    for(UInt32 i = 0; i < query->size; ++i) {
        EntityId id = query->entityIdList[i];
        Components_Graphics* graphics = ECS_EntityStore_GetComponent(entities, id, GRAPHICS_SIGNATURE);
        SDL_Rect r = Camera_CalculateSDLRectFromRect(renderingData->camera, renderingData->windowWidth, renderingData->windowHeight, &graphics->rect);
        SDL_RenderCopy(renderingData->renderer, graphics->texture, NULL, &r);
    }

    Rect playerHitBox = ((const Components_Collision*)ECS_EntityStore_GetConstComponent(entities, query->size - 1, COLLISION_SIGNATURE))->hitBox;
    DataStructures_UnorderedArray* points = Pathfinding_SolveAStar(&(Point){0, 0}, &(Point){playerHitBox.x, playerHitBox.y}, entities, 6, 2000);
    if(points == NULL)
        return; // pls don't

    UInt32 size = DataStructures_UnorderedArray_GetSize(points);
    Point* prevPoint = (size != NULL) ? DataStructures_UnorderedArray_Get(points, 0) : NULL;

    for(UInt32 i = 1; i < size; ++i) {
        Point* point = DataStructures_UnorderedArray_Get(points, i);
        SDL_Rect r1 = Camera_CalculateSDLRectFromRect(renderingData->camera, renderingData->windowWidth, renderingData->windowHeight, 
            &(Rect){.x = prevPoint->x, .y = prevPoint->y, .w = 0, .h = 0}
        );
        SDL_Rect r2 = Camera_CalculateSDLRectFromRect(renderingData->camera, renderingData->windowWidth, renderingData->windowHeight, 
            &(Rect){.x = point->x, .y = point->y, .w = 0, .h = 0}
        );
        SDL_RenderDrawLine(renderingData->renderer, r1.x, r1.y, r2.x, r2.y);
        prevPoint = point;
    }
    DataStructures_UnorderedArray_Destroy(points);
/*
    Point p = {0, 0};

    SDL_Rect r = Camera_CalculateSDLRectFromRect(renderingData->camera, renderingData->windowWidth, renderingData->windowHeight,
        &(Rect){
            .x = p.x,
            .y = p.y,
            .w = 0, .h = 0
        }
    );

    r.h = 10;
    r.w = 10;
    SDL_RenderFillRect(renderingData->renderer, &r);

    ECS_QueryResult_Destroy(query);*/
/*
    query = ECS_EntityStore_Query(entities, COLLISION_SIGNATURE);
    for(UInt32 i = 0; i < query->size; ++i) {
        EntityId id = query->entityIdList[i];
        Components_Collision* collision = ECS_EntityStore_GetComponent(entities, id, COLLISION_SIGNATURE);
        SDL_Rect r = Camera_CalculateSDLRectFromRect(renderingData->camera, renderingData->windowWidth, renderingData->windowHeight, &collision->hitBox);
        SDL_RenderFillRect(renderingData->renderer, &r);
    }
    ECS_QueryResult_Destroy(query);*/
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


void World_EntityActions_UpdateEntities(ECS_EntityStore* entities, Camera* camera)
{
    ECS_QueryResult* query = ECS_EntityStore_Query(entities, INPUT_SIGNATURE | GRAPHICS_SIGNATURE | COLLISION_SIGNATURE);

    Rect playerHitBox;

    for(UInt32 i = 0; i < query->size; ++i) {
        EntityId id = query->entityIdList[i];
        Components_Graphics* graphics = ECS_EntityStore_GetComponent(entities, id, GRAPHICS_SIGNATURE);
        Components_Collision* collision = ECS_EntityStore_GetComponent(entities, id, COLLISION_SIGNATURE);
        Components_Input* input = ECS_EntityStore_GetComponent(entities, id, INPUT_SIGNATURE);

        playerHitBox = collision->hitBox;

        if(input->x == 0 && input->y == 0)
            continue;
        

        // resolve collision:
        double xToMove = input->x;
        double yToMove = input->y;
        ECS_QueryResult* query = ECS_EntityStore_Query(entities, COLLISION_SIGNATURE);
        for(UInt32 i = 0; i < query->size; ++i) {
            EntityId id = query->entityIdList[i];
            Rect* rect = &((Components_Collision*)ECS_EntityStore_GetComponent(entities, id, COLLISION_SIGNATURE))->hitBox;

            double collisionTime;
            if(CollisionDetection_MovingRectRect(&collision->hitBox, input, rect, NULL, NULL, &collisionTime)) {
                xToMove = collisionTime * input->x;
                yToMove = collisionTime * input->y;
            }
        }
        ECS_QueryResult_Destroy(query);

        graphics->rect.x += xToMove;
        graphics->rect.y += yToMove;
        collision->hitBox.x += xToMove;
        collision->hitBox.y += yToMove;
        camera->x += xToMove;
        camera->y += yToMove;

        input->x = 0;
        input->y = 0;
    }

    ECS_QueryResult_Destroy(query);
}