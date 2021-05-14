#pragma once

// from std
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataStructures_UnorderedArray DataStructures_UnorderedArray;

DataStructures_UnorderedArray* DataStructures_UnorderedArray_Create(size_t elemSize, uint32_t capacity);
void DataStructures_UnorderedArray_Destroy(DataStructures_UnorderedArray* self);

void  DataStructures_UnorderedArray_AddElem(DataStructures_UnorderedArray* self, void* elem);
void  DataStructures_UnorderedArray_RemoveElem(DataStructures_UnorderedArray* self, uint32_t index);
void* DataStructures_UnorderedArray_Get(DataStructures_UnorderedArray* self, uint32_t index);

uint32_t DataStructures_UnorderedArray_GetSize(const DataStructures_UnorderedArray* self);

#ifdef __cplusplus
}
#endif