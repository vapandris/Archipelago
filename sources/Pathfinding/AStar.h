#pragma once

// from DataStructures
#include "DataStructures/UnorderedArray.h"

// from Base
#include "Base/Vector.h"

// from ECS
#include "ECS/ECS.h"

#ifdef __cplusplus
extern "C" {
#endif

DataStructures_UnorderedArray* Pathfinding_SolveAStar(
    const Point* start,
    const Point* goal,
    const ECS_EntityStore* entities,
    double gridSize,
    double searchRadious
);

#ifdef __cplusplus
}
#endif