#include "ECS.h"

// from std
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>


#define MAX_TYPE_COUNT 32
#define NO_COMPONENTS  0x0000

struct ECS_EntityStore
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
};

// ==================================================
// ================== ECS funtions ==================
// ==================================================

static bool     ExactlyOneBitIsSet(ComponentSignature bits);
static bool     SignatureExists(ECS_EntityStore* self, ComponentSignature newSignature);
static uint8_t  FindIndexToSignature(ECS_EntityStore* self, ComponentSignature signature);

static void     SwapEntityId(EntityId* id1, EntityId* id2);
static uint32_t Partition(ECS_QueryResult* self, uint32_t low, uint32_t high, ECS_EntityStore_Comparator comparator);
static void     QuickSort(ECS_QueryResult* self, uint32_t low, uint32_t high, ECS_EntityStore_Comparator comparator);


ECS_EntityStore* ECS_EntityStore_Create(uint32_t capacity, uint8_t componentCount, ...)
{
    assert(componentCount < MAX_TYPE_COUNT);
    assert(capacity > 0);

    ECS_EntityStore* result = malloc(sizeof *result);
    
    size_t size = 0;
    result->dataSizes = malloc(sizeof(*result->dataSizes) * componentCount);
    result->offsetSizes = malloc(sizeof(*result->offsetSizes) * componentCount);
    result->signatures = malloc(sizeof(*result->signatures) * componentCount);

    va_list args;
    va_start(args, componentCount);
    for(uint8_t i = 0; i < componentCount; ++i) {
        ECS_ComponentData componentData = va_arg(args, ECS_ComponentData);
        result->componentCount = i;

        assert(ExactlyOneBitIsSet(componentData.signature));
        assert(!SignatureExists(result, componentData.signature));

        result->dataSizes[i] = componentData.size;
        result->signatures[i] = componentData.signature;
        result->offsetSizes[i] = size;

        size += componentData.size;
    }
    result->componentCount = componentCount;
    result->clusterSize = size;
    va_end(args);

    result->storeCapacity = capacity;
    result->storeSize = 0;
    result->storeCapacity = capacity;
    result->storeSize = 0;

    result->data = malloc(result->clusterSize * result->storeCapacity);
    result->entitySignatures = malloc(sizeof(*result->entitySignatures) * result->storeCapacity);

    return result;
}


void ECS_EntityStore_Destroy(ECS_EntityStore* self)
{
    free(self->dataSizes);
    free(self->offsetSizes);
    free(self->signatures);
    free(self->data);
    free(self->entitySignatures);
}


EntityId ECS_EntityStore_CreateEntity(ECS_EntityStore* self)
{
    EntityId newId;

    newId = self->storeSize++;
    if(self->storeCapacity == newId) {
        uint32_t newCapacity = self->storeCapacity * 2;

        ComponentSignature* newSignatureList = realloc(self->entitySignatures, newCapacity * sizeof(*newSignatureList));
        void* newData = realloc(self->data, newCapacity * self->clusterSize);

        assert(newSignatureList != NULL && newData != NULL);

        self->entitySignatures = newSignatureList;
        self->data = newData;

        self->storeCapacity = newCapacity;
    }

    self->entitySignatures[newId] = NO_COMPONENTS;

    return newId;
}


void* ECS_EntityStore_GetComponent(ECS_EntityStore* self, EntityId entityId, ComponentSignature signature)
{
    assert(entityId < self->storeSize);
    assert(ExactlyOneBitIsSet(signature));

    uint8_t componentIndex = FindIndexToSignature(self, signature);
    return (uint8_t*)self->data + (entityId * self->clusterSize + self->offsetSizes[componentIndex]);
}


const void* ECS_EntityStore_GetConstComponent(const ECS_EntityStore* self, EntityId entityId, ComponentSignature signature)
{
    return ECS_EntityStore_GetComponent((ECS_EntityStore*)self, entityId, signature);
}


void ECS_EntityStore_AddComponent(ECS_EntityStore* self, EntityId entityId, ComponentSignature signature, void* data)
{
    assert(ExactlyOneBitIsSet(signature));

    uint8_t componentIndex = FindIndexToSignature(self, signature);
    size_t size = self->dataSizes[componentIndex];
    self->entitySignatures[entityId] |= signature;
    void* component = ECS_EntityStore_GetComponent(self, entityId, signature);
    memcpy(component, data, size);
}


void ECS_EntityStore_RemoveComponent(ECS_EntityStore* self, EntityId entityId, ComponentSignature signature)
{
    assert(entityId < self->storeSize);
    assert(ExactlyOneBitIsSet(signature));

    self->entitySignatures[entityId] &= ~signature;
}


bool ECS_EntityStore_HasComponents(const ECS_EntityStore* self, EntityId entityId, ComponentSignature signature)
{
    assert(entityId < self->storeSize);

    return (self->entitySignatures[entityId] & signature) == signature;
}


void ECS_EntityStore_KillEntity(ECS_EntityStore* self, EntityId entityId)
{
    assert(entityId < self->storeSize);

    uint32_t size = self->storeSize - 1;
    self->entitySignatures[entityId] = self->entitySignatures[size];

    memcpy((uint8_t*)self->data + (entityId * self->clusterSize),
           (uint8_t*)self->data + (size * self->clusterSize),
           self->clusterSize
    );
    --self->storeSize;
    --self->storeSize;
}


ECS_QueryResult* ECS_EntityStore_Query(const ECS_EntityStore* self, ComponentSignature signature)
{
    ECS_QueryResult* queryResult = malloc(sizeof *queryResult);
    queryResult->size = 0;
    queryResult->entityIdList = malloc(sizeof(*queryResult->entityIdList) * self->storeSize);
    queryResult->store = self;

    if(queryResult->entityIdList == NULL)
        return queryResult;

    for(EntityId i = 0; i < self->storeSize; ++i) {
        if((self->entitySignatures[i] & signature) == signature)
            queryResult->entityIdList[queryResult->size++] = i;
    }

    return queryResult;
}

// sorting with quicksort
void ECS_EntityStore_SortQuery(ECS_QueryResult* self, ECS_EntityStore_Comparator comparator)
{
    if(self->size == 0)
        return;
    QuickSort(self, 0, self->size - 1, comparator);
}


void ECS_QueryResult_Destroy(ECS_QueryResult* self)
{
    free(self->entityIdList);
    free(self);
}


// ======================================================
// ================== static functions ==================
// ======================================================

static bool ExactlyOneBitIsSet(ComponentSignature bits)
{
    return bits && !(bits & (bits - 1));
}


static bool SignatureExists(ECS_EntityStore* self, ComponentSignature newSignature)
{
    for(uint8_t i = 0; i < self->componentCount; ++i) {
        if(self->signatures[i] == newSignature)
            return true;
    }

    return false;
}


static uint8_t FindIndexToSignature(ECS_EntityStore* self, ComponentSignature signature)
{
    assert(SignatureExists(self, signature));

    for(uint8_t i = 0; i < self->componentCount; ++i) {
        if(self->signatures[i] == signature)
            return i;
    }

    return 0;
}


static void SwapEntityId(EntityId* id1, EntityId* id2)
{
    EntityId tmp = *id1;
    *id1 = *id2;
    *id2 = tmp;
}


static uint32_t Partition(ECS_QueryResult* self, uint32_t low, uint32_t high, ECS_EntityStore_Comparator comparator)
{
    EntityId* pivot = &self->entityIdList[high];
    /*
    if(self->size > 3) {
        EntityId first = self->entityIdList[low];
        uint32_t mid = low+(high-low)/2;
        EntityId midle = self->entityIdList[mid];
        EntityId last  = self->entityIdList[high];

        if((comparator(self->store, midle, first) && comparator(self->store, first, last)) ||
           (comparator(self->store, last, first)  && comparator(self->store, first, midle)))
           *pivot = first;
        else if((comparator(self->store, first, midle) && comparator(self->store, midle, last)) ||
                (comparator(self->store, last, midle)  && comparator(self->store, midle, first)))
            *pivot = midle;
        else
            *pivot = last;
    }
    */
    SwapEntityId(&self->entityIdList[high], pivot);
    const EntityId pi = self->entityIdList[high];

    int32_t i = low - 1;

    for(uint32_t j = low; j < high; ++j)
    {
        if(!comparator(self->store, pi, self->entityIdList[j])) {
            i += 1;
            SwapEntityId(&self->entityIdList[i], &self->entityIdList[j]);
        }
    }

    const uint32_t pivotIndex = i + 1;
    SwapEntityId(&self->entityIdList[pivotIndex], pivot);
    return pivotIndex;
}


static void QuickSort(ECS_QueryResult* self, uint32_t low, uint32_t high, ECS_EntityStore_Comparator comparator)
{
    if(low < high) {
        uint32_t pivot = Partition(self, low, high, comparator);

        QuickSort(self, low, pivot - 1, comparator);
        QuickSort(self, pivot + 1, high, comparator);
    }
}