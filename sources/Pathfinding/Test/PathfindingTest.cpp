#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest>

#include "Pathfinding/AStar.h"
#include "Components/Collision.h"
#include "Components/Signatures.h"

static EntityId CreateNewBoxEntity(ECS_EntityStore* store, double x, double y, double w, double h);

TEST_CASE("Solve A*")
{
    ECS_ComponentData collisionData;
    collisionData.signature = COLLISION_SIGNATURE;
    collisionData.size = sizeof(Components_Collision);
    ECS_EntityStore* entities = ECS_EntityStore_Create(3, 1, 
        collisionData
    );
    CreateNewBoxEntity(entities, 4, 4, 1, 10);
    CreateNewBoxEntity(entities, 12, 10, 12, 7);

    Point start = {0, 0};
    Point goal  = {14, 10};
    DataStructures_UnorderedArray* points = Pathfinding_SolveAStar(
        &start, &goal, entities, 4, 20
    );

    uint32_t size = DataStructures_UnorderedArray_GetSize(points);
    REQUIRE(size == 5);

    Point* p0 = (Point*)DataStructures_UnorderedArray_Get(points, 0);
    Point* p1 = (Point*)DataStructures_UnorderedArray_Get(points, 1);
    Point* p2 = (Point*)DataStructures_UnorderedArray_Get(points, 2);
    Point* p3 = (Point*)DataStructures_UnorderedArray_Get(points, 3);
    Point* p4 = (Point*)DataStructures_UnorderedArray_Get(points, 4);

    CHECK(p0->x == 12); CHECK(p0->y == 12);
    CHECK(p1->x == 8); CHECK(p1->y == 8);
    CHECK(p2->x == 4); CHECK(p2->y == 8);
    CHECK(p3->x == 0); CHECK(p3->y == 4);
    CHECK(p4->x == 0); CHECK(p4->y == 0);

    DataStructures_UnorderedArray_Destroy(points);

    ECS_EntityStore_Destroy(entities);
}


static EntityId CreateNewBoxEntity(ECS_EntityStore* store, double x, double y, double w, double h)
{
    EntityId newId = ECS_EntityStore_CreateEntity(store);

    Components_Collision collision;
    collision.hitBox = {x, y, w, h};
    collision.type = SOLID;
    ECS_EntityStore_AddComponent(store, newId, COLLISION_SIGNATURE, &collision);

    return newId;
}