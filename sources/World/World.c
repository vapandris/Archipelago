#include "World.h"

// from std
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// from Base
#include "Base/Types.h"

// from ECS
#include "ECS/ECS.h"

// from TextureManager
#include "TextureManager/TextureManager.h"

// from Components
#include "Components/Signatures.h"
#include "Components/Collision.h"
#include "Components/Graphics.h"
#include "Components/Input.h"

// from World
#include "World/EntityActions.h"

// from SDL2
#include <SDL2/SDL_scancode.h>

struct World
{
    unsigned seed;
    Rect worldRect;
    Camera* camera;
    ECS_EntityStore* entities;
};


static double LinearGenerator(double x, double multiplyer, double origoX);


World* World_Create(Camera* camera)
{
    World* result = malloc(sizeof *result);

    result->entities = ECS_EntityStore_Create(1, 3,
        (ECS_ComponentData){.size = sizeof(Components_Collision),   .signature = COLLISION_SIGNATURE},
        (ECS_ComponentData){.size = sizeof(Components_Graphics),    .signature = GRAPHICS_SIGNATURE},
        (ECS_ComponentData){.size = sizeof(Components_Input),       .signature = INPUT_SIGNATURE}
    );

    result->camera = camera;
    result->worldRect = (Rect){0};
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
    srand(seed);
    
    Components_Graphics graphics = {
        .rect = {.x = 50, .y = 50, .w = 24, .h = 48},
        .texture = TextureManager_GetPlayerTexture()
    };
    self->camera->x = (graphics.rect.x * 1.5) - (self->camera->w / 2.0);
    self->camera->y = (graphics.rect.y * 0.5) + (self->camera->h / 2.0);

    // Generate the ground:
    self->worldRect = (Rect){.x = -1000, .y = 1000, .w = 2000, .h = 2000};
    double tileSize = 32;
    double x, y;
    const double xEnd = self->worldRect.x + self->worldRect.w;
    const double yEnd = self->worldRect.y - self->worldRect.h;
    const double worldOrigoX = self->worldRect.w / 2 + self->worldRect.x; 
    for(x = self->worldRect.x; x < xEnd; x += tileSize) {
        for(y = self->worldRect.y; y > yEnd; y -= tileSize) {
            EntityId turfId = ECS_EntityStore_CreateEntity(self->entities);
            bool isGrass = abs(LinearGenerator(x, 0.5, worldOrigoX) - y) <= 150 || abs(LinearGenerator(-x, 0.5, worldOrigoX) - y) <= 150;
            ECS_EntityStore_AddComponent(self->entities, turfId, GRAPHICS_SIGNATURE,
                &(Components_Graphics){
                    .rect.x = x, .rect.y = y, .rect.w = tileSize, .rect.h = tileSize,
                    .texture = isGrass ? TextureManager_GetLightGrassTexture() : TextureManager_GetSkyTexture()
                }
            );
            if(!isGrass) {
                ECS_EntityStore_AddComponent(self->entities, turfId, COLLISION_SIGNATURE,
                    &(Components_Collision){
                        .hitBox.x = x, .hitBox.y = y, .hitBox.w = tileSize, .hitBox.h = tileSize,
                        .type = SOLID
                    }
                );
            }

            if(isGrass && rand() % 7 == 0) {
                EntityId pineId = ECS_EntityStore_CreateEntity(self->entities);
                ECS_EntityStore_AddComponent(self->entities, pineId, GRAPHICS_SIGNATURE,
                    &(Components_Graphics){
                        .rect.x = x, .rect.y = y + tileSize, .rect.w = 34, .rect.h = 51,
                        .texture = TextureManager_GetPineTexture()
                    }
                );
            } else if(isGrass && rand() % 4 == 0) {
                EntityId flowerId = ECS_EntityStore_CreateEntity(self->entities);
                ECS_EntityStore_AddComponent(self->entities, flowerId, GRAPHICS_SIGNATURE,
                    &(Components_Graphics){
                        .rect.x = x, .rect.y = y, .rect.w = tileSize, .rect.h = tileSize,
                        .texture = TextureManager_GetFlowersTexture()
                    }
                );
            }
        }
    }

    // Create Player:
    EntityId playerId = ECS_EntityStore_CreateEntity(self->entities);
    ECS_EntityStore_AddComponent(self->entities, playerId, GRAPHICS_SIGNATURE, &graphics);
    ECS_EntityStore_AddComponent(self->entities, playerId, COLLISION_SIGNATURE, &(Components_Collision){
        .hitBox.x = graphics.rect.x, .hitBox.y = graphics.rect.y - graphics.rect.h / 2,
        .hitBox.w = graphics.rect.w, .hitBox.h = graphics.rect.h / 2,
        .type = SOLID
    });
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


void World_UpdateEntities(World* self, Camera* camera)
{
    World_EntityActions_UpdateEntities(self->entities, camera);
}


// static functions:
static double LinearGenerator(double x, double multiplyer, double origoX)
{
    return (multiplyer * x) - origoX;
}