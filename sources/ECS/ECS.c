#include "ECS.h"

// from std
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>


#define MAX_TYPE_COUNT 32


// ======================================================
// ================== Internal structs ==================
// ======================================================

typedef struct ComponentStore
{
    uint8_t typeCount;
    size_t clusterSize;
    size_t* dataSizes;
    size_t* offsetSizes;
    ComponentSignature* signatures;

    uint32_t storeCapacity;
    uint32_t storeSize;
    void* data;

} ComponentStore;


typedef struct EntityStore
{
    uint32_t storeSize;
    uint32_t storeCapacity;
    ComponentSignature* entitySignatures;
    // TODO: FLAGS (alive, type)
} EntityStore;


typedef struct State
{
    ComponentStore componentStore;
    EntityStore entityStore;
    ECS_QueryResult queryResult;
} State;

// ==================================================
// ========= State that holds every Entity ==========
// ==================================================

static State state;

// ==================================================
// ================== ECS funtions ==================
// ==================================================

static bool ExactlyOneBitIsSet(ComponentSignature bits);
static bool SignatureExists(ComponentSignature newSignature);
static uint8_t FindIndexToSignature(ComponentSignature signature);


void ECS_Init(uint32_t capacity, uint8_t componentCount, ...)
{
    assert(componentCount < MAX_TYPE_COUNT);
    assert(capacity > 0);

    ComponentStore* componentStore = &state.componentStore;
    EntityStore* entityStore = &state.entityStore;
    ECS_QueryResult* queryResult = &state.queryResult;
    
    size_t size = 0;
    componentStore->dataSizes = malloc(sizeof(*componentStore->dataSizes) * componentCount);
    componentStore->offsetSizes = malloc(sizeof(*componentStore->offsetSizes) * componentCount);
    componentStore->signatures = malloc(sizeof(*componentStore->signatures) * componentCount);

    va_list args;
    va_start(args, componentCount);
    for(uint8_t i = 0; i < componentCount; ++i) {
        ECS_ComponentData componentData = va_arg(args, ECS_ComponentData);
        componentStore->typeCount = i;

        assert(ExactlyOneBitIsSet(componentData.signature));
        assert(!SignatureExists(componentData.signature));

        componentStore->dataSizes[i] = componentData.size;
        componentStore->signatures[i] = componentData.signature;
        componentStore->offsetSizes[i] = size;

        size += componentData.size;
    }
    componentStore->typeCount = componentCount;
    componentStore->clusterSize = size;
    va_end(args);

    componentStore->storeCapacity = capacity;
    componentStore->storeSize = 0;
    entityStore->storeCapacity = capacity;
    entityStore->storeSize = 0;

    componentStore->data = malloc(componentStore->clusterSize * componentStore->storeCapacity);
    entityStore->entitySignatures = malloc(sizeof(*entityStore->entitySignatures) * entityStore->storeCapacity);
    queryResult->entityList = malloc(sizeof(*queryResult->entityList) * capacity);
}


void ECS_Quit()
{
    free(state.componentStore.dataSizes);
    free(state.componentStore.offsetSizes);
    free(state.componentStore.signatures);
    free(state.componentStore.data);
    free(state.entityStore.entitySignatures);
    free(state.queryResult.entityList);
}


Entity  ECS_CreateEntity()
{
    assert(state.entityStore.storeCapacity == state.componentStore.storeCapacity);
    assert(state.entityStore.storeSize == state.componentStore.storeSize);

    Entity result;

    result.id = state.entityStore.storeSize++;
    if(state.entityStore.storeCapacity == result.id) {
        uint32_t newCapacity = state.entityStore.storeCapacity * 2;

        uint32_t* newQuery = realloc(state.queryResult.entityList, newCapacity * sizeof(*newQuery));
        ComponentSignature* newSignatureList = realloc(state.entityStore.entitySignatures, newCapacity * sizeof(*newSignatureList));
        void* newData = realloc(state.componentStore.data, newCapacity * state.componentStore.clusterSize);

        assert(newQuery == NULL || newSignatureList == NULL || newData == NULL);

        state.queryResult.entityList = newQuery;
        state.entityStore.entitySignatures = newSignatureList;
        state.componentStore.data = newData;

        state.componentStore.storeCapacity = newCapacity;
        state.entityStore.storeCapacity = newCapacity;
    }

    state.entityStore.entitySignatures[result.id] = NO_COMPONENTS;

    return result;
}


void* ECS_GetComponent(Entity entity, ComponentSignature signature)
{
    assert(entity.id < state.componentStore.storeSize);

    uint8_t componentIndex = FindIndexToSignature(signature);
    return (uint8_t*)state.componentStore.data + (entity.id * state.componentStore.clusterSize + state.componentStore.offsetSizes[componentIndex]);
}


void ECS_AddComponent(Entity entity, ComponentSignature signature, void* data)
{
    assert(entity.id < state.componentStore.storeSize);

    uint8_t componentIndex = FindIndexToSignature(signature);
    size_t size = state.componentStore.dataSizes[componentIndex];
    state.entityStore.entitySignatures[entity.id] |= signature;
    void* component = ECS_GetComponent(entity, signature);
    memcpy(component, data, size);
}


void ECS_RemoveComponent(Entity entity, ComponentSignature signature)
{
    assert(entity.id < state.componentStore.storeSize);
    assert(ExactlyOneBitIsSet(signature));

    state.entityStore.entitySignatures[entity.id] &= ~signature;
}


bool ECS_HasComponent(Entity entity, ComponentSignature signature)
{
    assert(entity.id < state.componentStore.storeSize);
    assert(ExactlyOneBitIsSet(signature));

    return state.entityStore.entitySignatures[entity.id] & signature;
}


void ECS_KillEntity(Entity entity)
{
    assert(entity.id < state.componentStore.storeSize);

    uint32_t size = state.entityStore.storeSize - 1;
    state.entityStore.entitySignatures[entity.id] = state.entityStore.entitySignatures[size];
    --state.entityStore.storeSize;
    --state.componentStore.storeSize;
}


ECS_QueryResult* ECS_Querry(ComponentSignature signature)
{
    state.queryResult.size = 0;

    for(uint32_t i = 0; i < state.entityStore.storeSize; ++i) {
        if(state.entityStore.entitySignatures[i] & signature)
            state.queryResult.entityList[state.queryResult.size++] = i;
    }

    return &state.queryResult;
}


// ======================================================
// ================== static functions ==================
// ======================================================

static bool ExactlyOneBitIsSet(ComponentSignature bits)
{
    return bits && !(bits & (bits - 1));
}


static bool SignatureExists(ComponentSignature newSignature)
{
    for(uint8_t i = 0; i < state.componentStore.typeCount; ++i) {
        if(state.componentStore.signatures[i] == newSignature)
            return true;
    }

    return false;
}

static uint8_t FindIndexToSignature(ComponentSignature signature)
{
    assert(ExactlyOneBitIsSet(signature));
    assert(SignatureExists(signature));

    for(uint8_t i = 0; i < state.componentStore.typeCount; ++i) {
        if(state.componentStore.signatures[i] == signature)
            return i;
    }

    return 0;
}