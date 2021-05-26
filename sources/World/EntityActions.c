#include "EntityActions.h"

// from std
#include <assert.h>

// from Base
#include "Base/Types.h"

// from SDL2
#include <SDL2/SDL_scancode.h>

// from Components
#include "Components/Signatures.h"
#include "Components/Collision.h"
#include "Components/Graphics.h"
#include "Components/Input.h"

// from TextureManager
#include "TextureManager/TextureManager.h"

// from Collision
#include "Collision/CollisionDetection.h"

// from Pathfinding
#include "Pathfinding/AStar.h"


//#define DRAW_HITBOX

static bool CompareEntities(const ECS_EntityStore* store, EntityId id1, EntityId id2);
static void DrawHitboxes(ECS_EntityStore* entities, Camera_RenderingData* renderingData);

void World_EntityActions_DrawEntites(ECS_EntityStore* entities, Camera_RenderingData* renderingData)
{
    ECS_QueryResult* query = ECS_EntityStore_Query(entities, GRAPHICS_SIGNATURE);
    if(query == NULL)
        return;

    ECS_EntityStore_SortQuery(query, CompareEntities);

    // Draw entites:
    for(UInt32 i = 0; i < query->size; ++i) {
        EntityId id = query->entityIdList[i];
        Components_Graphics* graphics = ECS_EntityStore_GetComponent(entities, id, GRAPHICS_SIGNATURE);
        SDL_Rect r = Camera_CalculateSDLRectFromRect(renderingData->camera, renderingData->windowWidth, renderingData->windowHeight, &graphics->rect);
        SDL_RenderCopy(renderingData->renderer, graphics->texture, NULL, &r);
    }

    // draw pathfinding
    /*
    Rect playerHitBox = ((const Components_Collision*)ECS_EntityStore_GetConstComponent(entities, query->size - 1, COLLISION_SIGNATURE))->hitBox;
    DataStructures_UnorderedArray* points = Pathfinding_SolveAStar(&(Point){0, 0}, &(Point){playerHitBox.x, playerHitBox.y}, entities, 6, 1000);
    if(points == NULL)
        return; // pls don't

    UInt32 size = DataStructures_UnorderedArray_GetSize(points);
    Point* prevPoint = (size != 0) ? DataStructures_UnorderedArray_Get(points, 0) : NULL;

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
    */
    #ifdef DRAW_HITBOX
    DrawHitboxes(entities, renderingData);
    #endif
    
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


void World_EntityActions_UpdateEntities(ECS_EntityStore* entities, Camera* camera)
{
    ECS_QueryResult* query = ECS_EntityStore_Query(entities, INPUT_SIGNATURE | GRAPHICS_SIGNATURE | COLLISION_SIGNATURE);

    for(UInt32 i = 0; i < query->size; ++i) {
        EntityId id = query->entityIdList[i];
        Components_Graphics* graphics = ECS_EntityStore_GetComponent(entities, id, GRAPHICS_SIGNATURE);
        Components_Collision* collision = ECS_EntityStore_GetComponent(entities, id, COLLISION_SIGNATURE);
        Components_Input* input = ECS_EntityStore_GetComponent(entities, id, INPUT_SIGNATURE);

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


// Create functions:
static const double tileSize = 32;

EntityId World_EntityActions_CreatePlayer(ECS_EntityStore* entities, Point* topLeft)
{
    EntityId newId = ECS_EntityStore_CreateEntity(entities);

    const double graphicWidth  = 24;
    const double graphicHeight = 48;
    ECS_EntityStore_AddComponent(entities, newId, GRAPHICS_SIGNATURE, &(Components_Graphics){
            .layer = AboveGround,
            .texture = TextureManager_GetPlayerTexture(),
            .rect = {.x = topLeft->x, .y = topLeft->y, .w = graphicWidth, .h = graphicHeight}
    });
    ECS_EntityStore_AddComponent(entities, newId, COLLISION_SIGNATURE, &(Components_Collision){
            .type = SOLID,
            .hitBox = {.x = topLeft->x, .y = topLeft->y - graphicHeight/2, .w = graphicWidth, .h = graphicHeight/2}
    });
    ECS_EntityStore_AddComponent(entities, newId, INPUT_SIGNATURE, &(Components_Input){
        .x = 0, .y = 0
    });
    return newId;
}


EntityId World_EntityActions_CreateGrassTile(ECS_EntityStore* entities, Point* topLeft)
{
    EntityId newId = ECS_EntityStore_CreateEntity(entities);

    ECS_EntityStore_AddComponent(entities, newId, GRAPHICS_SIGNATURE, &(Components_Graphics){
        .layer = Ground,
        .texture = TextureManager_GetLightGrassTexture(),
        .rect = {.x = topLeft->x, .y = topLeft->y, .w = tileSize, .h = tileSize}
    });

    return newId;
}


EntityId World_EntityActions_CreateVoidTile(ECS_EntityStore* entities, Point* topLeft)
{
    EntityId newId = ECS_EntityStore_CreateEntity(entities);

    Rect tileRect = {.x = topLeft->x, .y = topLeft->y, .w = tileSize, .h = tileSize};
    ECS_EntityStore_AddComponent(entities, newId, GRAPHICS_SIGNATURE, &(Components_Graphics){
        .layer = Ground,
        .texture = TextureManager_GetSkyTexture(),
        .rect = tileRect
    });
    ECS_EntityStore_AddComponent(entities, newId, COLLISION_SIGNATURE, &(Components_Collision){
        .type = SOLID,
        .hitBox = tileRect
    });

    return newId;
}


EntityId World_EntityActions_CreateTree(ECS_EntityStore* entities, Point* topLeft)
{
    EntityId newId = ECS_EntityStore_CreateEntity(entities);

    const double graphicWidth  = 34;
    const double graphicHeight = 51;
    const double hitBoxWidth  = 16;
    const double hitBoxHeight = 11;
    ECS_EntityStore_AddComponent(entities, newId, GRAPHICS_SIGNATURE, &(Components_Graphics){
        .layer = AboveGround,
        .texture = TextureManager_GetPineTexture(),
        .rect = {.x = topLeft->x, .y = topLeft->y + tileSize, .w = graphicWidth, .h = graphicHeight}
    });
    ECS_EntityStore_AddComponent(entities, newId, COLLISION_SIGNATURE, &(Components_Collision){
        .hitBox = {.x = topLeft->x + 9, .y = topLeft->y + tileSize - 36, .w = hitBoxWidth, .h = hitBoxHeight}
    });

    return newId;
}


EntityId World_EntityActions_CreateFlowers(ECS_EntityStore* entities, Point* topLeft)
{
    EntityId newId = ECS_EntityStore_CreateEntity(entities);

    ECS_EntityStore_AddComponent(entities, newId, GRAPHICS_SIGNATURE, &(Components_Graphics){
        .layer = GroundDecorations,
        .texture = TextureManager_GetFlowersTexture(),
        .rect = {.x = topLeft->x, .y = topLeft->y, .w = tileSize, .h = tileSize}
    });

    return newId;
}


// static functions:
static bool CompareEntities(const ECS_EntityStore* store, EntityId id1, EntityId id2)
{
    if(ECS_EntityStore_HasComponents(store, id1, GRAPHICS_SIGNATURE) && ECS_EntityStore_HasComponents(store, id1, GRAPHICS_SIGNATURE)) {
        const Components_Graphics* g1 = ECS_EntityStore_GetConstComponent(store, id1, GRAPHICS_SIGNATURE);
        const Components_Graphics* g2 = ECS_EntityStore_GetConstComponent(store, id2, GRAPHICS_SIGNATURE);

        if(g1->layer != g2->layer)
            return g1->layer < g2->layer;

        return g1->rect.y - g1->rect.h > g2->rect.y - g2->rect.h;
    } else {
        assert(false && "NO GRAPHICS_SIGNATURE!");
        return true;
    }
}


static void DrawHitboxes(ECS_EntityStore* entities, Camera_RenderingData* renderingData)
{
    ECS_QueryResult* query = ECS_EntityStore_Query(entities, COLLISION_SIGNATURE);
    for(UInt32 i = 0; i < query->size; ++i) {
        EntityId id = query->entityIdList[i];
        Components_Collision* collision = ECS_EntityStore_GetComponent(entities, id, COLLISION_SIGNATURE);
        SDL_Rect r = Camera_CalculateSDLRectFromRect(renderingData->camera, renderingData->windowWidth, renderingData->windowHeight, &collision->hitBox);
        SDL_RenderFillRect(renderingData->renderer, &r);
    }
    ECS_QueryResult_Destroy(query);
}