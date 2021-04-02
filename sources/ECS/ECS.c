#include "ECS.h"

// from std
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>


#define MAX_TYPE_COUNT 32
#define NO_COMPONENTS  0x0000


// ======================================================
// ================== Internal structs ==================
// ======================================================


typedef struct EntityStore
{
    uint8_t componentCount;
    size_t clusterSize;
    size_t* dataSizes;
    size_t* offsetSizes;
    ComponentSignature* signatures;
    void* data;
    
    ComponentSignature* entitySignatures;
    // TODO: FLAGS (alive, type)

    uint32_t storeSize;
    uint32_t storeCapacity;

    ECS_QueryResult queryResult;
} EntityStore;

// ==================================================
// ========= State that holds every Entity ==========
// ==================================================

static EntityStore state;

// ==================================================
// ================== ECS funtions ==================
// ==================================================

static bool     ExactlyOneBitIsSet(ComponentSignature bits);
static bool     SignatureExists(ComponentSignature newSignature);
static uint8_t  FindIndexToSignature(ComponentSignature signature);


void ECS_Init(uint32_t capacity, uint8_t componentCount, ...)
{
    assert(componentCount < MAX_TYPE_COUNT);
    assert(capacity > 0);
    
    size_t size = 0;
    state.dataSizes = malloc(sizeof(*state.dataSizes) * componentCount);
    state.offsetSizes = malloc(sizeof(*state.offsetSizes) * componentCount);
    state.signatures = malloc(sizeof(*state.signatures) * componentCount);

    va_list args;
    va_start(args, componentCount);
    for(uint8_t i = 0; i < componentCount; ++i) {
        ECS_ComponentData componentData = va_arg(args, ECS_ComponentData);
        state.componentCount = i;

        assert(ExactlyOneBitIsSet(componentData.signature));
        assert(!SignatureExists(componentData.signature));

        state.dataSizes[i] = componentData.size;
        state.signatures[i] = componentData.signature;
        state.offsetSizes[i] = size;

        size += componentData.size;
    }
    state.componentCount = componentCount;
    state.clusterSize = size;
    va_end(args);

    state.storeCapacity = capacity;
    state.storeSize = 0;
    state.storeCapacity = capacity;
    state.storeSize = 0;

    state.data = malloc(state.clusterSize * state.storeCapacity);
    state.entitySignatures = malloc(sizeof(*state.entitySignatures) * state.storeCapacity);
    state.queryResult.entityIdList = malloc(sizeof(*state.queryResult.entityIdList) * capacity);
}


void ECS_Quit()
{
    free(state.dataSizes);
    free(state.offsetSizes);
    free(state.signatures);
    free(state.data);
    free(state.entitySignatures);
    free(state.queryResult.entityIdList);
}


uint32_t ECS_CreateEntity()
{
    uint32_t newId;

    newId = state.storeSize++;
    if(state.storeCapacity == newId) {
        uint32_t newCapacity = state.storeCapacity * 2;

        uint32_t* newQuery = realloc(state.queryResult.entityIdList, newCapacity * sizeof(*newQuery));
        ComponentSignature* newSignatureList = realloc(state.entitySignatures, newCapacity * sizeof(*newSignatureList));
        void* newData = realloc(state.data, newCapacity * state.clusterSize);

        assert(newQuery == NULL || newSignatureList == NULL || newData == NULL);

        state.queryResult.entityIdList = newQuery;
        state.entitySignatures = newSignatureList;
        state.data = newData;

        state.storeCapacity = newCapacity;
        state.storeCapacity = newCapacity;
    }

    state.entitySignatures[newId] = NO_COMPONENTS;

    return newId;
}


void* ECS_GetComponent(uint32_t entityId, ComponentSignature signature)
{
    assert(entityId < state.storeSize);
    assert(ExactlyOneBitIsSet(signature));

    uint8_t componentIndex = FindIndexToSignature(signature);
    return (uint8_t*)state.data + (entityId * state.clusterSize + state.offsetSizes[componentIndex]);
}


void ECS_AddComponent(uint32_t entityId, ComponentSignature signature, void* data)
{
    assert(ExactlyOneBitIsSet(signature));

    uint8_t componentIndex = FindIndexToSignature(signature);
    size_t size = state.dataSizes[componentIndex];
    state.entitySignatures[entityId] |= signature;
    void* component = ECS_GetComponent(entityId, signature);
    memcpy(component, data, size);
}


void ECS_RemoveComponent(uint32_t entityId, ComponentSignature signature)
{
    assert(entityId < state.storeSize);
    assert(ExactlyOneBitIsSet(signature));

    state.entitySignatures[entityId] &= ~signature;
}


bool ECS_HasComponents(uint32_t entityId, ComponentSignature signature)
{
    assert(entityId < state.storeSize);

    return (state.entitySignatures[entityId] & signature) == signature;
}


void ECS_KillEntity(uint32_t entityId)
{
    assert(entityId < state.storeSize);

    uint32_t size = state.storeSize - 1;
    state.entitySignatures[entityId] = state.entitySignatures[size];

    memcpy((uint8_t*)state.data + (entityId * state.clusterSize),
           (uint8_t*)state.data + (size * state.clusterSize),
           state.clusterSize
    );
    --state.storeSize;
    --state.storeSize;
}


ECS_QueryResult* ECS_Query(ComponentSignature signature)
{
    state.queryResult.size = 0;

    for(uint32_t i = 0; i < state.storeSize; ++i) {
        if((state.entitySignatures[i] & signature) == signature)
            state.queryResult.entityIdList[state.queryResult.size++] = i;
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
    for(uint8_t i = 0; i < state.componentCount; ++i) {
        if(state.signatures[i] == newSignature)
            return true;
    }

    return false;
}


static uint8_t FindIndexToSignature(ComponentSignature signature)
{
    assert(ExactlyOneBitIsSet(signature));
    assert(SignatureExists(signature));

    for(uint8_t i = 0; i < state.componentCount; ++i) {
        if(state.signatures[i] == signature)
            return i;
    }

    return 0;
}