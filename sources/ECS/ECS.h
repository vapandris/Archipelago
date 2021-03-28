#pragma once

// ==================================================
// ========= Simple Entity Component System =========
// ==================================================

// from std
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>

#define NO_COMPONENTS  0x0000
#define ANY_COMPONENTS 0x0000

typedef struct Entity
{
    uint32_t id;
} Entity;

typedef uint32_t ComponentSignature;


typedef struct ECS_QueryResult
{
    uint32_t  size;
    uint32_t* entityList;
} ECS_QueryResult;


typedef struct ECS_ComponentData {
    size_t              size;
    ComponentSignature  signature;
} ECS_ComponentData;


void    ECS_Init(uint32_t capacity, uint8_t componentCount, ...);
void    ECS_Quit();

Entity  ECS_CreateEntity();
void*   ECS_GetComponent(Entity entity, ComponentSignature signature);
void    ECS_AddComponent(Entity entity, ComponentSignature signature, void* data);
void    ECS_RemoveComponent(Entity entity, ComponentSignature signature);
bool    ECS_HasComponent(Entity entity, ComponentSignature signature);
void    ECS_KillEntity(Entity entity);

ECS_QueryResult* ECS_Querry(ComponentSignature signature);