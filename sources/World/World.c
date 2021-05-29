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
    
    EntityId playerId = World_EntityActions_CreatePlayer(self->entities, &(Point){50, 50});
    const Components_Graphics* playerGraphics = ECS_EntityStore_GetConstComponent(self->entities, playerId, GRAPHICS_SIGNATURE);

    self->camera->x = (playerGraphics->rect.x * 1.5) - (self->camera->w / 2.0);
    self->camera->y = (playerGraphics->rect.y * 0.5) + (self->camera->h / 2.0);

    // Generate the ground:
    self->worldRect = (Rect){.x = -1000, .y = 1000, .w = 2000, .h = 2000};
    const double tileSize = 32;
    double x, y;
    const double xEnd = self->worldRect.x + self->worldRect.w;
    const double yEnd = self->worldRect.y - self->worldRect.h;
    const double worldOrigoX = self->worldRect.w / 2 + self->worldRect.x; 
    for(x = self->worldRect.x; x < xEnd; x += tileSize) {
        for(y = self->worldRect.y; y > yEnd; y -= tileSize) {
            bool isGrass = (abs(LinearGenerator(x, 0.5, worldOrigoX) - y) <= 150 || abs(LinearGenerator(-x, 0.5, worldOrigoX) - y) <= 150) &&
                           (self->worldRect.x < x && x + tileSize < xEnd);
            
            if(isGrass) {
                World_EntityActions_CreateGrassTile(self->entities, &(Point){x, y});
            } else {
                World_EntityActions_CreateVoidTile(self->entities, &(Point){x, y});
            }

            if(isGrass && rand() % 7 == 0) {
                World_EntityActions_CreateTree(self->entities, &(Point){x, y + tileSize/1.5});
            } else if(isGrass && rand() % 4 == 0) {
                World_EntityActions_CreateFlowers(self->entities, &(Point){x, y});
            }
        }
    }
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