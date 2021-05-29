#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest>

#include "DataStructures/UnorderedArray.h"

TEST_CASE("Int Array: create test")
{
    DataStructures_UnorderedArray* array = DataStructures_UnorderedArray_Create(sizeof(int), 1);
    CHECK(DataStructures_UnorderedArray_GetSize(array) == 0);

    for(uint32_t i = 0; i < 10; ++i) {
        DataStructures_UnorderedArray_AddElem(array, &i);
    }
    CHECK(DataStructures_UnorderedArray_GetSize(array) == 10);

    DataStructures_UnorderedArray_Destroy(array);
}

TEST_CASE("Int Array: Add + Get element")
{
    DataStructures_UnorderedArray* array = DataStructures_UnorderedArray_Create(sizeof(int), 10);

    for(uint32_t i = 0; i < 10; ++i) {
        DataStructures_UnorderedArray_AddElem(array, &i);
    }

    for(uint32_t i = 0; i < DataStructures_UnorderedArray_GetSize(array); ++i) {
        CHECK(*((int*)DataStructures_UnorderedArray_Get(array, i)) == i);
    }

    DataStructures_UnorderedArray_Destroy(array);
}


TEST_CASE("Int Array: Remove element")
{
    DataStructures_UnorderedArray* array = DataStructures_UnorderedArray_Create(sizeof(int), 10);
    for(uint32_t i = 0; i < 10; ++i) {
        DataStructures_UnorderedArray_AddElem(array, &i);
    }

    uint32_t arraySize = DataStructures_UnorderedArray_GetSize(array);
    for(uint32_t i = 0; i < arraySize; ++i) {
        uint32_t prevSize = DataStructures_UnorderedArray_GetSize(array);
        DataStructures_UnorderedArray_RemoveElem(array, 0);
        uint32_t size = DataStructures_UnorderedArray_GetSize(array);
        CHECK(size == prevSize - 1);
    }

    CHECK(DataStructures_UnorderedArray_GetSize(array) == 0);

    DataStructures_UnorderedArray_Destroy(array);
}