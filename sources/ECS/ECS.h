#pragma once

// ==================================================
// ========= Simple Entity Component System =========
// ==================================================

// from std
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>

#define ANY_COMPONENTS 0x0000

typedef struct Entity
{
    uint32_t signature;
} Entity;

typedef uint32_t ComponentSignature;


typedef struct ECS_QueryResult
{
    uint32_t  size;
    uint32_t* entityIdList;
} ECS_QueryResult;


typedef struct ECS_ComponentData {
    size_t              size;
    ComponentSignature  signature;
} ECS_ComponentData;


void    ECS_Init(uint32_t capacity, uint8_t componentCount, ...);
void    ECS_Quit();

uint32_t    ECS_CreateEntity();
void*       ECS_GetComponent(uint32_t entityId, ComponentSignature signature);
void        ECS_AddComponent(uint32_t entityId, ComponentSignature signature, void* data);
void        ECS_RemoveComponent(uint32_t entityId, ComponentSignature signature);
bool        ECS_HasComponents(uint32_t entityId, ComponentSignature signature);
void        ECS_KillEntity(uint32_t entityId);

ECS_QueryResult* ECS_Query(ComponentSignature signature);