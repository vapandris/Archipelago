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
typedef uint32_t EntityId;


typedef struct ECS_QueryResult
{
    uint32_t  size;
    EntityId* entityIdList;
} ECS_QueryResult;


typedef struct ECS_ComponentData {
    size_t              size;
    ComponentSignature  signature;
} ECS_ComponentData;


typedef struct ECS_EntityStore ECS_EntityStore;


ECS_EntityStore* ECS_EntityStore_Create(uint32_t capacity, uint8_t componentCount, ...);
void             ECS_EntityStore_Destroy(ECS_EntityStore* self);

EntityId    ECS_EntityStore_CreateEntity(ECS_EntityStore* self);
void*       ECS_EntityStore_GetComponent(ECS_EntityStore* self, EntityId entityId, ComponentSignature signature);
const void* ECS_EntityStore_GetConstComponent(const ECS_EntityStore* self, EntityId entityId, ComponentSignature signature);
void        ECS_EntityStore_AddComponent(ECS_EntityStore* self, EntityId entityId, ComponentSignature signature, void* data);
void        ECS_EntityStore_RemoveComponent(ECS_EntityStore* self, EntityId entityId, ComponentSignature signature);
bool        ECS_EntityStore_HasComponents(const ECS_EntityStore* self, EntityId entityId, ComponentSignature signature);
void        ECS_EntityStore_KillEntity(ECS_EntityStore* self, EntityId entityId);

ECS_QueryResult* ECS_EntityStore_Query(const ECS_EntityStore* self, ComponentSignature signature);

void    ECS_QueryResult_Destroy(ECS_QueryResult* self);