#include "CollisionDetection.h"

// from std
#include <math.h>
#include <stdlib.h>

static void swap(double* d1, double* d2);

bool CollisionDetection_PointRect(const Point* point, const Rect* rect)
{
    return (rect->x <= point->x && point->x <= rect->x + rect->w &&
            rect->y >= point->y && point->y >= rect->y - rect->h
    );
}


bool CollisionDetection_RectRect(const Rect* rect1, const Rect* rect2)
{
    return (rect1->x <= rect2->x + rect2->w && rect1->x + rect1->w >= rect2->x &&
            rect1->y >= rect2->y - rect2->h && rect1->y - rect1->h <= rect2->y);
}


bool CollisionDetection_VectorRect(
        const Vector* vector,
        const Rect* rect,
        Point* contactPoint, Point* contactNorm, double* collisionTime
)
{
    if(vector->dir.x == 0 && vector->dir.y == 0)
        return false;

    if(contactPoint != NULL)
        *contactPoint = (Point){0, 0};

    if(contactNorm != NULL)
        *contactNorm = (Point){0, 0};

    // Cache division
    Point invDir = {
        .x = (vector->dir.x != 0) ? 1.0f / vector->dir.x : INFINITY,
        .y = (vector->dir.y != 0) ? 1.0f / vector->dir.y : INFINITY
    };

    // Calculate intersections with rectangle bounding axes
    double tNEARx = (rect->x - vector->start.x) * invDir.x;
    double tNEARy = (rect->y - vector->start.y) * invDir.y;
    double tFARx  = (rect->x + rect->w - vector->start.x) * invDir.x;
    double tFARy  = (rect->y - rect->h - vector->start.y) * invDir.y;

    if(isnan(tNEARx) || isnan(tNEARy) || isnan(tFARx) || isnan(tFARy))
        return false;

    // Sort distances
    if(tNEARx > tFARx) swap(&tNEARx, &tFARx);
    if(tNEARy > tFARy) swap(&tNEARy, &tFARy);

    // Early rejection
    if(tNEARx > tFARy || tNEARy > tFARx)
        return false;

    // Closest 'time' will be the 1st contact
    double tHitNear = fmax(tNEARx, tNEARy);
    if(collisionTime != NULL)
        *collisionTime = tHitNear;

    // Furthest 'time' is contact on opposite side of target
    double tHitFar = fmin(tFARx, tFARy);

    // Reject if ray direction is pointing away from object
    if (tHitFar < 0 || tHitNear < 0 || tHitNear > 1)
        return false;

    // Contact point of collision from parametric line equation
    if(contactPoint != NULL) {
        contactPoint->x = vector->start.x + tHitNear * vector->dir.x;
        contactPoint->y = vector->start.y + tHitNear * vector->dir.y;
    }

    if(contactNorm != NULL) {
        contactNorm->x = 0;
        contactNorm->y = 0;
        if(tNEARx > tNEARy) {
            if(invDir.x < 0)
                contactNorm->x = 1;
            else
                contactNorm->x = -1;
        } else if(tNEARx < tNEARy) {
            if(invDir.y < 0)
                contactNorm->y = 1;
            else
                contactNorm->y = -1;
        }
    }

    return true;
}


bool CollisionDetection_MovingRectRect(
        const Rect* movingRect, const Point* direction,
        const Rect* rect,
        Point* contactPoint, Point* contactNorm, double* collisionTime
)
{
    if(direction->x == 0 && direction->y == 0)
        return false;

    // Vector starting from the midle of the moving rect
    Vector vector = {
        .start = {.x = movingRect->x + (movingRect->w / 2), .y = movingRect->y - (movingRect->h / 2)},
        .dir = *direction
    };

    // We expand the rect, since we start the vector from the midle of the moving rect
    Rect expandedRect = {
        .x = rect->x - (movingRect->w / 2),
        .y = rect->y + (movingRect->h / 2),
        .w = rect->w + (movingRect->w),
        .h = rect->h + (movingRect->h)
    };
    return CollisionDetection_VectorRect(
        &vector, &expandedRect, contactPoint, contactNorm, collisionTime
    );
}

// static functions:
static void swap(double* d1, double* d2)
{
    double tmp = *d1;
    *d1 = *d2;
    *d2 = tmp;
}