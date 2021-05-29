#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest>

#include "ECS/ECS.h"
#include "Components/Collision.h"
#include "Components/Signatures.h"

TEST_CASE("ECS setup")
{
    ECS_ComponentData collisionData;
    collisionData.size = sizeof(Components_Collision);
    collisionData.signature = COLLISION_SIGNATURE;
    ECS_EntityStore* entities = ECS_EntityStore_Create(1, 1,
        collisionData
    );

    CHECK(true);

    ECS_EntityStore_Destroy(entities);
}

TEST_CASE("Entity creation")
{
    ECS_ComponentData collisionData;
    collisionData.size = sizeof(Components_Collision);
    collisionData.signature = COLLISION_SIGNATURE;
    ECS_EntityStore* entities = ECS_EntityStore_Create(1, 1,
        collisionData
    );

    EntityId newId;
    newId = ECS_EntityStore_CreateEntity(entities);
    CHECK(newId == 0);
    newId = ECS_EntityStore_CreateEntity(entities);
    CHECK(newId == 1);
    newId = ECS_EntityStore_CreateEntity(entities);
    CHECK(newId == 2);

    ECS_EntityStore_Destroy(entities);
}


TEST_CASE("Entity add/has/get component")
{
    ECS_ComponentData collisionData;
    collisionData.size = sizeof(Components_Collision);
    collisionData.signature = COLLISION_SIGNATURE;
    ECS_EntityStore* entities = ECS_EntityStore_Create(1, 1,
        collisionData
    );

    EntityId id = ECS_EntityStore_CreateEntity(entities);

    Components_Collision collision;
    collision.hitBox = {0, 0, 10, 10};
    collision.type = SOLID;
    ECS_EntityStore_AddComponent(entities, id, COLLISION_SIGNATURE, &collision);

    CHECK(ECS_EntityStore_HasComponents(entities, id, COLLISION_SIGNATURE));

    Components_Collision* gotCollision = (Components_Collision*)ECS_EntityStore_GetComponent(entities, id, COLLISION_SIGNATURE);

    CHECK(gotCollision->hitBox.x == collision.hitBox.x);
    CHECK(gotCollision->hitBox.y == collision.hitBox.y);
    CHECK(gotCollision->hitBox.w == collision.hitBox.w);
    CHECK(gotCollision->hitBox.h == collision.hitBox.h);
    CHECK(gotCollision->type == collision.type);

    ECS_EntityStore_Destroy(entities);
}


TEST_CASE("query ECS")
{
    ECS_ComponentData collisionData;
    collisionData.size = sizeof(Components_Collision);
    collisionData.signature = COLLISION_SIGNATURE;
    ECS_EntityStore* entities = ECS_EntityStore_Create(1, 1,
        collisionData
    );

    EntityId id1 = ECS_EntityStore_CreateEntity(entities);
    EntityId id2 = ECS_EntityStore_CreateEntity(entities);
                   ECS_EntityStore_CreateEntity(entities);
                   ECS_EntityStore_CreateEntity(entities);
    EntityId id5 = ECS_EntityStore_CreateEntity(entities);

    Components_Collision collision;
    collision.hitBox = {0, 0, 10, 10};
    collision.type = SOLID;

    ECS_EntityStore_AddComponent(entities, id1, COLLISION_SIGNATURE, &collision);

    ECS_EntityStore_AddComponent(entities, id2, COLLISION_SIGNATURE, &collision);

    ECS_EntityStore_AddComponent(entities, id5, COLLISION_SIGNATURE, &collision);

    ECS_QueryResult* query = ECS_EntityStore_Query(entities, COLLISION_SIGNATURE);

    REQUIRE(query->size == 3);
    CHECK(id1 == query->entityIdList[0]);
    CHECK(id2 == query->entityIdList[1]);
    CHECK(id5 == query->entityIdList[2]);

    ECS_EntityStore_Destroy(entities);
}