#include "AStar.h"

// from std
#include <stdlib.h>

// from DataStructures
#include "DataStructures/UnorderedArray.h"

// from Base
#include "Base/Types.h"

// from Components
#include "Components/Signatures.h"
#include "Components/Collision.h"

// from Collision
#include "Collision/CollisionDetection.h"

typedef struct Node Node;

struct Node
{
    Point coords;
    double distanceFromStart;
    double distanceToEnd;
    Node* parent;
};

static double PythagorasTheorem(double a, double b);
static double Distance(const Point* p1, const Point* p2);
static double Heuristic(const Node* node);

static bool DoesPointCollideWithEntity(const ECS_EntityStore* entities, const Point* point);

static Node* FindNodeInArray(DataStructures_UnorderedArray* array, const Point* point);

DataStructures_UnorderedArray* Pathfinding_SolveAStar(
    const Point* start,
    const Point* goal,
    const ECS_EntityStore* entities,
    double gridSize,
    double searchRadious
)
{
    if(Distance(start, goal) >= searchRadious)
        return DataStructures_UnorderedArray_Create(sizeof(Point*), 0);

    DataStructures_UnorderedArray* openNodes   = DataStructures_UnorderedArray_Create(sizeof(Node*), 32);
    DataStructures_UnorderedArray* closedNodes = DataStructures_UnorderedArray_Create(sizeof(Node*), 32);

    Node* startNode = malloc(sizeof *startNode);
    *startNode = (Node){
        .coords = {start->x, start->y},
        .distanceFromStart = 0,
        .distanceToEnd = Distance(start, goal),
        .parent = NULL
    };

    DataStructures_UnorderedArray_AddElem(openNodes, &startNode);

    bool foundPath = false;
    Node* goalNode = NULL;
    while(DataStructures_UnorderedArray_GetSize(openNodes) > 0 && !foundPath)
    {
        // find the most promissing Node
        UInt32 ind = 0;
        Node* currentNode = *(Node**)DataStructures_UnorderedArray_Get(openNodes, ind);
        for(UInt32 i = 1; i < DataStructures_UnorderedArray_GetSize(openNodes); ++i) {
            Node* node = *(Node**)DataStructures_UnorderedArray_Get(openNodes, i);
            if(Heuristic(currentNode) > Heuristic(node)) {
                ind = i;
                currentNode = node;
            }
        }

        DataStructures_UnorderedArray_RemoveElem(openNodes, ind);
        DataStructures_UnorderedArray_AddElem(closedNodes, &currentNode);
        //currentNode = *(Node**)DataStructures_UnorderedArray_Get(closedNodes, DataStructures_UnorderedArray_GetSize(closedNodes) - 1);

        if(currentNode->distanceToEnd < gridSize) {
            goalNode = currentNode;
            foundPath = true;
            continue;
        }
        
        // generate/update neighbours
        for(Int8 i = -1; i <= 1; ++i) {
            for(Int8 j = -1; j <= 1; ++j) {
                if(i == 0 && j == 0)
                    continue;

                Point point = {
                    currentNode->coords.x + gridSize*j,
                    currentNode->coords.y + gridSize*i,
                };

                if(FindNodeInArray(closedNodes, &point) == NULL && !DoesPointCollideWithEntity(entities, &point)) {
                    Node* node = FindNodeInArray(openNodes, &point);
                    bool isNodeInCorner = (i != 0 && j != 0);
                    double newDistanceFromStart = currentNode->distanceFromStart + ((isNodeInCorner) ? gridSize * sqrt(2) : gridSize);
                    if(node == NULL) { // new node
                        Node* newNode = malloc(sizeof *newNode);
                        *newNode = (Node){
                                .coords = {point.x, point.y},
                                .distanceFromStart = newDistanceFromStart,
                                .distanceToEnd = Distance(&point, goal),
                                .parent = currentNode
                        };
                        DataStructures_UnorderedArray_AddElem(openNodes, &newNode);
                    } else {
                        if(node->distanceFromStart > newDistanceFromStart) {
                            node->distanceFromStart = newDistanceFromStart;
                            node->parent = currentNode;
                        }
                    }
                }
            }
        }

    }
    
    // construct an array of Points to the goal Node

    DataStructures_UnorderedArray* pointsOfPath = DataStructures_UnorderedArray_Create(sizeof(Point), 2);
    for(Node* node = goalNode; node != NULL; node = node->parent) {
        DataStructures_UnorderedArray_AddElem(pointsOfPath, &node->coords);
    }

    for(UInt32 i = 0; i < DataStructures_UnorderedArray_GetSize(openNodes); ++i)
        free(*(Node**)DataStructures_UnorderedArray_Get(openNodes, i));

    for(UInt32 i = 0; i < DataStructures_UnorderedArray_GetSize(closedNodes); ++i)
        free(*(Node**)DataStructures_UnorderedArray_Get(closedNodes, i));
    
    DataStructures_UnorderedArray_Destroy(openNodes);
    DataStructures_UnorderedArray_Destroy(closedNodes);
    return pointsOfPath;
}


// static functions:
static double PythagorasTheorem(double a, double b)
{
    return sqrt(a*a + b*b);
}


static double Distance(const Point* p1, const Point* p2)
{
    return PythagorasTheorem(p1->x - p2->x, p1->y - p2->y);
}


static double Heuristic(const Node* node)
{
    return node->distanceFromStart + node->distanceToEnd;
}


static bool DoesPointCollideWithEntity(const ECS_EntityStore* entities, const Point* point)
{
    ECS_QueryResult* query = ECS_EntityStore_Query(entities, COLLISION_SIGNATURE);
    for(EntityId id = 0; id < query->size; ++id) {
        const Components_Collision* hitBox = ECS_EntityStore_GetConstComponent(entities, id, COLLISION_SIGNATURE);

        if(CollisionDetection_PointRect(point, &hitBox->hitBox))
            return true;
    }

    return false;
}


static Node* FindNodeInArray(DataStructures_UnorderedArray* array, const Point* point)
{
    for(UInt32 i = 0; i < DataStructures_UnorderedArray_GetSize(array); ++i) {
        Node* node = *(Node**)DataStructures_UnorderedArray_Get(array, i);
        if(node->coords.x == point->x && node->coords.y == point->y)
            return node;
    }

    return NULL;
}