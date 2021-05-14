#pragma once

// ==================================================
// ========= Simple Entity Component System =========
// ==================================================

// from std
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ANY_COMPONENTS 0x0000

typedef uint32_t ComponentSignature;
typedef uint32_t EntityId;

typedef struct ECS_EntityStore ECS_EntityStore;

typedef struct ECS_QueryResult
{
    uint32_t  size;
    EntityId* entityIdList;
    const ECS_EntityStore* store;
} ECS_QueryResult;


typedef struct ECS_ComponentData {
    size_t              size;
    ComponentSignature  signature;
} ECS_ComponentData;

// Should return:
// We want to be able to define if id1's data is 'greater' then id2's data in result's EntityStore
// true:  if id1's data  <= id2's data
// false: if id1's data  >  id2's data
typedef bool (*ECS_EntityStore_Comparator)(const ECS_EntityStore* store, EntityId id1, EntityId id2);


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
void             ECS_EntityStore_SortQuery(ECS_QueryResult* self, ECS_EntityStore_Comparator comparator);

void    ECS_QueryResult_Destroy(ECS_QueryResult* self);

#ifdef __cplusplus
}
#endif