#include "UnorderedArray.h"

// from std
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

struct DataStructures_UnorderedArray
{
    uint32_t capacity;
    uint32_t size;

    size_t emelSize;
    void*  array;
};


DataStructures_UnorderedArray* DataStructures_UnorderedArray_Create(size_t elemSize, uint32_t capacity)
{
    DataStructures_UnorderedArray* result = malloc(sizeof *result);
    result->capacity = (capacity > 0) ? capacity : 1;
    result->size = 0;

    result->emelSize = elemSize;
    result->array = malloc(elemSize * capacity);

    return result;
}


void DataStructures_UnorderedArray_Destroy(DataStructures_UnorderedArray* self)
{
    free(self->array);
    free(self);
}


void DataStructures_UnorderedArray_AddElem(DataStructures_UnorderedArray* self, void* elem)
{
    uint32_t newIndex = self->size++;
    if(self->size == self->capacity) {
        uint32_t newCapacity = self->capacity * 2;

        void* newArray = realloc(self->array, self->emelSize * newCapacity);

        assert(newArray != NULL);

        self->array = newArray;
        self->capacity = newCapacity;
    }

    memcpy(
        DataStructures_UnorderedArray_Get(self, newIndex),
        elem,
        self->emelSize
    );
}


void DataStructures_UnorderedArray_RemoveElem(DataStructures_UnorderedArray* self, uint32_t index)
{
    if(index >= self->size) {
        assert(false && "Overindexing!");
        return;
    }

    memcpy(
        DataStructures_UnorderedArray_Get(self, index),
        DataStructures_UnorderedArray_Get(self, --self->size),
        self->emelSize
    );
}


void* DataStructures_UnorderedArray_Get(DataStructures_UnorderedArray* self, uint32_t index)
{
    if(index >= self->size) {
        assert(false && "Overindexing!");
        return;
    }
    
    return (uint8_t*)self->array + (index * self->emelSize);
}


uint32_t DataStructures_UnorderedArray_GetSize(const DataStructures_UnorderedArray* self)
{
    return self->size;
}