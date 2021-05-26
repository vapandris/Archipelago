#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest>

#include "Collision/CollisionDetection.h"

static const double EPS = 10e-5;

TEST_CASE("Point-Rect collides")
{
    Rect rect;
    rect.x = 10;
    rect.y = -5;
    rect.w = 30;
    rect.h = 20;

    Point p1 = {10, -5};
    Point p2 = {40, -25};
    Point p3 = {40, -5};
    Point p4 = {10, -25};
    Point p5 = {30, -10};
    Point p6 = {10, -15.2};

    CHECK(CollisionDetection_PointRect(&p1, &rect));
    CHECK(CollisionDetection_PointRect(&p2, &rect));
    CHECK(CollisionDetection_PointRect(&p3, &rect));
    CHECK(CollisionDetection_PointRect(&p4, &rect));
    CHECK(CollisionDetection_PointRect(&p5, &rect));
    CHECK(CollisionDetection_PointRect(&p6, &rect));
}

TEST_CASE("Point-Rect not collides")
{
    Rect rect;
    rect.x = 10;
    rect.y = -5;
    rect.w = 30;
    rect.h = 20;

    Point p1_1 = {10 - EPS, -5};
    Point p1_2 = {10 , -5 + EPS};
    Point p1_3 = {10 - EPS, -5 + EPS};

    Point p2_1 = {40 + EPS, -25};
    Point p2_2 = {40 , -25 - EPS};
    Point p2_3 = {40 + EPS, -25 - EPS};

    Point p3_1 = {40 + EPS, -5};
    Point p3_2 = {40, -5 + EPS};
    Point p3_3 = {40 + EPS, -5 + EPS};

    Point p4_1 = {10 - EPS, -25};
    Point p4_2 = {10, -25 - EPS};
    Point p4_3 = {10 - EPS, -25 - EPS};

    CHECK_FALSE(CollisionDetection_PointRect(&p1_1, &rect));
    CHECK_FALSE(CollisionDetection_PointRect(&p1_2, &rect));
    CHECK_FALSE(CollisionDetection_PointRect(&p1_3, &rect));

    CHECK_FALSE(CollisionDetection_PointRect(&p2_1, &rect));
    CHECK_FALSE(CollisionDetection_PointRect(&p2_2, &rect));
    CHECK_FALSE(CollisionDetection_PointRect(&p2_3, &rect));

    CHECK_FALSE(CollisionDetection_PointRect(&p3_1, &rect));
    CHECK_FALSE(CollisionDetection_PointRect(&p3_2, &rect));
    CHECK_FALSE(CollisionDetection_PointRect(&p3_3, &rect));

    CHECK_FALSE(CollisionDetection_PointRect(&p4_1, &rect));
    CHECK_FALSE(CollisionDetection_PointRect(&p4_2, &rect));
    CHECK_FALSE(CollisionDetection_PointRect(&p4_3, &rect));
}

TEST_CASE("Rect-Rect collides")
{
    Rect rect;
    rect.x = 10;
    rect.y = -5;
    rect.w = 30;
    rect.h = 20;

    Rect r1 = {0, 5, 10, 10};
    Rect r2 = {12, 5, 10, 10};
    Rect r3 = {40, 5, 10, 10};
    Rect r4 = {0, 0, 10, 10};
    Rect r5 = {0, -25, 10, 10};
    Rect r6 = {12, -25, 10, 10};
    Rect r7 = {40, -25, 10, 10};
    Rect r8 = {12, -25, 10, 10};

    Rect r9  = {12, -20, 10, 10};
    Rect r10 = {12, 0, 10, 10};
    Rect r11 = {10, -5, 30, 20};

    CHECK(CollisionDetection_RectRect(&r1, &rect));
    CHECK(CollisionDetection_RectRect(&r2, &rect));
    CHECK(CollisionDetection_RectRect(&r3, &rect));
    CHECK(CollisionDetection_RectRect(&r4, &rect));
    CHECK(CollisionDetection_RectRect(&r5, &rect));
    CHECK(CollisionDetection_RectRect(&r6, &rect));
    CHECK(CollisionDetection_RectRect(&r7, &rect));
    CHECK(CollisionDetection_RectRect(&r8, &rect));
    CHECK(CollisionDetection_RectRect(&r9, &rect));
    CHECK(CollisionDetection_RectRect(&r10, &rect));
    CHECK(CollisionDetection_RectRect(&r11, &rect));
}

TEST_CASE("Rect-Rect not collides")
{
    Rect rect;
    rect.x = 10;
    rect.y = -5;
    rect.w = 30;
    rect.h = 20;

    Rect r1 = {0, 5 + EPS, 10, 10};
    Rect r2 = {12, 5 + EPS, 10, 10};
    Rect r3 = {40, 5 + EPS, 10, 10};
    Rect r4 = {0 - EPS, 0, 10, 10};
    Rect r5 = {0 - EPS, -25, 10, 10};
    Rect r6 = {12, -25 - EPS, 10, 10};

    CHECK_FALSE(CollisionDetection_RectRect(&r1, &rect));
    CHECK_FALSE(CollisionDetection_RectRect(&r2, &rect));
    CHECK_FALSE(CollisionDetection_RectRect(&r3, &rect));
    CHECK_FALSE(CollisionDetection_RectRect(&r4, &rect));
    CHECK_FALSE(CollisionDetection_RectRect(&r5, &rect));
    CHECK_FALSE(CollisionDetection_RectRect(&r6, &rect));
}

TEST_CASE("Vector-Rect collides")
{

}

TEST_CASE("Vector-Rect not collides")
{

}

TEST_CASE("Moving-Rect collides")
{

}

TEST_CASE("Moving-Rect not collides")
{

}