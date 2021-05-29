#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest>
#include <cmath>

#include "Camera/Camera.h"


// window sizes:
static const unsigned int windowWidth  = 720;
static const unsigned int windowHeight = 480;

// global cameras:
static const Camera camera1 = {
    0,
    0,
    720,
    480
};

static const Camera camera2 = {
    -50.20,
    100.75,
    480,
    320
};

static const Camera camera3 = {
    250,
    0,
    1440,
    960
};

static const Camera camera4 = {
    -100,
    20,
    50,
    2000
};

static double Round(double num);

TEST_CASE("Convert Rect to SDL_Rect with 0 sizes")
{
    // Rects to convert:
    const Rect r1 = {0,   0,   0, 0};
    const Rect r2 = {-20, -10, 0, 0};
    const Rect r3 = {+20, +10, 0, 0};

    const SDL_Rect sdlRect1FromCamera1 = Camera_CalculateSDLRectFromRect(&camera1, windowWidth, windowHeight, &r1);
    const SDL_Rect sdlRect2FromCamera1 = Camera_CalculateSDLRectFromRect(&camera1, windowWidth, windowHeight, &r2);
    const SDL_Rect sdlRect3FromCamera1 = Camera_CalculateSDLRectFromRect(&camera1, windowWidth, windowHeight, &r3);

    const SDL_Rect sdlRect1FromCamera2 = Camera_CalculateSDLRectFromRect(&camera2, windowWidth, windowHeight, &r1);
    const SDL_Rect sdlRect2FromCamera2 = Camera_CalculateSDLRectFromRect(&camera2, windowWidth, windowHeight, &r2);
    const SDL_Rect sdlRect3FromCamera2 = Camera_CalculateSDLRectFromRect(&camera2, windowWidth, windowHeight, &r3);

    const SDL_Rect sdlRect1FromCamera3 = Camera_CalculateSDLRectFromRect(&camera3, windowWidth, windowHeight, &r1);
    const SDL_Rect sdlRect2FromCamera3 = Camera_CalculateSDLRectFromRect(&camera3, windowWidth, windowHeight, &r2);
    const SDL_Rect sdlRect3FromCamera3 = Camera_CalculateSDLRectFromRect(&camera3, windowWidth, windowHeight, &r3);

    // Converted using Camera1
    CHECK(sdlRect1FromCamera1.x == 0);  CHECK(sdlRect1FromCamera1.y == 0);
    CHECK(sdlRect1FromCamera1.w == 0);  CHECK(sdlRect1FromCamera1.h == 0);

    CHECK(sdlRect2FromCamera1.x == -20);CHECK(sdlRect2FromCamera1.y == 10);
    CHECK(sdlRect2FromCamera1.w == 0);  CHECK(sdlRect2FromCamera1.h == 0);

    CHECK(sdlRect3FromCamera1.x == 20); CHECK(sdlRect3FromCamera1.y == -10);
    CHECK(sdlRect3FromCamera1.w == 0);  CHECK(sdlRect3FromCamera1.h == 0);

    // Converted using Camera2
    CHECK(sdlRect1FromCamera2.x == 76); CHECK(sdlRect1FromCamera2.y == 152);
    CHECK(sdlRect1FromCamera2.w == 0);  CHECK(sdlRect1FromCamera2.h == 0);

    CHECK(sdlRect2FromCamera2.x == 46); CHECK(sdlRect2FromCamera2.y == 167);
    CHECK(sdlRect2FromCamera2.w == 0);  CHECK(sdlRect2FromCamera2.h == 0);

    CHECK(sdlRect3FromCamera2.x == 106);CHECK(sdlRect3FromCamera2.y == 137);
    CHECK(sdlRect3FromCamera2.w == 0);  CHECK(sdlRect3FromCamera2.h == 0);

    // Converted using Camera3
    CHECK(sdlRect1FromCamera3.x == -125); CHECK(sdlRect1FromCamera3.y == 0);
    CHECK(sdlRect1FromCamera3.w == 0);    CHECK(sdlRect1FromCamera3.h == 0);

    CHECK(sdlRect2FromCamera3.x == -135); CHECK(sdlRect2FromCamera3.y == 5);
    CHECK(sdlRect2FromCamera3.w == 0);    CHECK(sdlRect2FromCamera3.h == 0);

    CHECK(sdlRect3FromCamera3.x == -115); CHECK(sdlRect3FromCamera3.y == -5);
    CHECK(sdlRect3FromCamera3.w == 0);    CHECK(sdlRect3FromCamera3.h == 0);
}

TEST_CASE("Convert Rect to SDL_Rect with same sizes")
{
    // Rects to convert:
    const Rect r1 = {0,   0,   20, 20};
    const Rect r2 = {-20, -10, 35.75, 35.75};
    const Rect r3 = {+20, +10, -22.5, -22.5};

    const SDL_Rect sdlRect1FromCamera1 = Camera_CalculateSDLRectFromRect(&camera1, windowWidth, windowHeight, &r1);
    const SDL_Rect sdlRect2FromCamera1 = Camera_CalculateSDLRectFromRect(&camera1, windowWidth, windowHeight, &r2);
    const SDL_Rect sdlRect3FromCamera1 = Camera_CalculateSDLRectFromRect(&camera1, windowWidth, windowHeight, &r3);

    const SDL_Rect sdlRect1FromCamera2 = Camera_CalculateSDLRectFromRect(&camera2, windowWidth, windowHeight, &r1);
    const SDL_Rect sdlRect2FromCamera2 = Camera_CalculateSDLRectFromRect(&camera2, windowWidth, windowHeight, &r2);
    const SDL_Rect sdlRect3FromCamera2 = Camera_CalculateSDLRectFromRect(&camera2, windowWidth, windowHeight, &r3);

    const SDL_Rect sdlRect1FromCamera3 = Camera_CalculateSDLRectFromRect(&camera3, windowWidth, windowHeight, &r1);
    const SDL_Rect sdlRect2FromCamera3 = Camera_CalculateSDLRectFromRect(&camera3, windowWidth, windowHeight, &r2);
    const SDL_Rect sdlRect3FromCamera3 = Camera_CalculateSDLRectFromRect(&camera3, windowWidth, windowHeight, &r3);

    // Converted using Camera1
    CHECK(sdlRect1FromCamera1.x == 0);   CHECK(sdlRect1FromCamera1.y == 0);
    CHECK(sdlRect1FromCamera1.w == 20);  CHECK(sdlRect1FromCamera1.h == 20);

    CHECK(sdlRect2FromCamera1.x == -20); CHECK(sdlRect2FromCamera1.y == 10);
    CHECK(sdlRect2FromCamera1.w == 36);  CHECK(sdlRect2FromCamera1.h == 36);

    CHECK(sdlRect3FromCamera1.x == 20);  CHECK(sdlRect3FromCamera1.y == -10);
    CHECK(sdlRect3FromCamera1.w == -22); CHECK(sdlRect3FromCamera1.h == -22);

    // Converted using Camera2
    CHECK(sdlRect1FromCamera2.x == 76);  CHECK(sdlRect1FromCamera2.y == 152);
    CHECK(sdlRect1FromCamera2.w == 30);  CHECK(sdlRect1FromCamera2.h == 30);

    CHECK(sdlRect2FromCamera2.x == 46);  CHECK(sdlRect2FromCamera2.y == 167);
    CHECK(sdlRect2FromCamera2.w == 54);  CHECK(sdlRect2FromCamera2.h == 54);

    CHECK(sdlRect3FromCamera2.x == 106); CHECK(sdlRect3FromCamera2.y == 137);
    CHECK(sdlRect3FromCamera2.w == -33); CHECK(sdlRect3FromCamera2.h == -33);

    // Converted using Camera3
    CHECK(sdlRect1FromCamera3.x == -125); CHECK(sdlRect1FromCamera3.y == 0);
    CHECK(sdlRect1FromCamera3.w == 10);   CHECK(sdlRect1FromCamera3.h == 10);

    CHECK(sdlRect2FromCamera3.x == -135); CHECK(sdlRect2FromCamera3.y == 5);
    CHECK(sdlRect2FromCamera3.w == 18);   CHECK(sdlRect2FromCamera3.h == 18);

    CHECK(sdlRect3FromCamera3.x == -115); CHECK(sdlRect3FromCamera3.y == -5);
    CHECK(sdlRect3FromCamera3.w == -11);  CHECK(sdlRect3FromCamera3.h == -11);
}

TEST_CASE("Convert Rect to SDL_Rect with different sizes")
{
    // Rects to convert:
    const Rect r1 = {0,   0,   20, -20};
    const Rect r2 = {-20, -10, 75.35, 35.75};
    const Rect r3 = {+20, +10, -5.22, -22.5};

    const SDL_Rect sdlRect1FromCamera1 = Camera_CalculateSDLRectFromRect(&camera1, windowWidth, windowHeight, &r1);
    const SDL_Rect sdlRect2FromCamera1 = Camera_CalculateSDLRectFromRect(&camera1, windowWidth, windowHeight, &r2);
    const SDL_Rect sdlRect3FromCamera1 = Camera_CalculateSDLRectFromRect(&camera1, windowWidth, windowHeight, &r3);

    const SDL_Rect sdlRect1FromCamera2 = Camera_CalculateSDLRectFromRect(&camera2, windowWidth, windowHeight, &r1);
    const SDL_Rect sdlRect2FromCamera2 = Camera_CalculateSDLRectFromRect(&camera2, windowWidth, windowHeight, &r2);
    const SDL_Rect sdlRect3FromCamera2 = Camera_CalculateSDLRectFromRect(&camera2, windowWidth, windowHeight, &r3);

    const SDL_Rect sdlRect1FromCamera3 = Camera_CalculateSDLRectFromRect(&camera3, windowWidth, windowHeight, &r1);
    const SDL_Rect sdlRect2FromCamera3 = Camera_CalculateSDLRectFromRect(&camera3, windowWidth, windowHeight, &r2);
    const SDL_Rect sdlRect3FromCamera3 = Camera_CalculateSDLRectFromRect(&camera3, windowWidth, windowHeight, &r3);

    // Converted using Camera1
    CHECK(sdlRect1FromCamera1.x == 0);   CHECK(sdlRect1FromCamera1.y == 0);
    CHECK(sdlRect1FromCamera1.w == 20);  CHECK(sdlRect1FromCamera1.h == -20);

    CHECK(sdlRect2FromCamera1.x == -20); CHECK(sdlRect2FromCamera1.y == 10);
    CHECK(sdlRect2FromCamera1.w == 76);  CHECK(sdlRect2FromCamera1.h == 36);

    CHECK(sdlRect3FromCamera1.x == 20);  CHECK(sdlRect3FromCamera1.y == -10);
    CHECK(sdlRect3FromCamera1.w == -5);  CHECK(sdlRect3FromCamera1.h == -22);

    // Converted using Camera2
    CHECK(sdlRect1FromCamera2.x == 76);  CHECK(sdlRect1FromCamera2.y == 152);
    CHECK(sdlRect1FromCamera2.w == 30);  CHECK(sdlRect1FromCamera2.h == -30);

    CHECK(sdlRect2FromCamera2.x == 46);  CHECK(sdlRect2FromCamera2.y == 167);
    CHECK(sdlRect2FromCamera2.w == 114); CHECK(sdlRect2FromCamera2.h == 54);

    CHECK(sdlRect3FromCamera2.x == 106); CHECK(sdlRect3FromCamera2.y == 137);
    CHECK(sdlRect3FromCamera2.w == -7);  CHECK(sdlRect3FromCamera2.h == -33);

    // Converted using Camera3
    CHECK(sdlRect1FromCamera3.x == -125); CHECK(sdlRect1FromCamera3.y == 0);
    CHECK(sdlRect1FromCamera3.w == 10);   CHECK(sdlRect1FromCamera3.h == -10);

    CHECK(sdlRect2FromCamera3.x == -135); CHECK(sdlRect2FromCamera3.y == 5);
    CHECK(sdlRect2FromCamera3.w == 38);   CHECK(sdlRect2FromCamera3.h == 18);

    CHECK(sdlRect3FromCamera3.x == -115); CHECK(sdlRect3FromCamera3.y == -5);
    CHECK(sdlRect3FromCamera3.w == -2);   CHECK(sdlRect3FromCamera3.h == -11);
}

TEST_CASE("Convert SDL_Rect to Rect with 0 sizes")
{
    // SDL_Rects to convert:
    const SDL_Rect r1 = {0, 0, 0, 0};
    const SDL_Rect r2 = {20, 20, 0, 0};
    const SDL_Rect r3 = {-20, -20, 0, 0};

    const Rect rect1FromCamera1 = Camera_CalculateRectFromSDLRect(&camera1, windowWidth, windowHeight, &r1);
    const Rect rect2FromCamera1 = Camera_CalculateRectFromSDLRect(&camera1, windowWidth, windowHeight, &r2);
    const Rect rect3FromCamera1 = Camera_CalculateRectFromSDLRect(&camera1, windowWidth, windowHeight, &r3);
    
    const Rect rect1FromCamera2 = Camera_CalculateRectFromSDLRect(&camera2, windowWidth, windowHeight, &r1);
    const Rect rect2FromCamera2 = Camera_CalculateRectFromSDLRect(&camera2, windowWidth, windowHeight, &r2);
    const Rect rect3FromCamera2 = Camera_CalculateRectFromSDLRect(&camera2, windowWidth, windowHeight, &r3);

    const Rect rect1FromCamera3 = Camera_CalculateRectFromSDLRect(&camera3, windowWidth, windowHeight, &r1);
    const Rect rect2FromCamera3 = Camera_CalculateRectFromSDLRect(&camera3, windowWidth, windowHeight, &r2);
    const Rect rect3FromCamera3 = Camera_CalculateRectFromSDLRect(&camera3, windowWidth, windowHeight, &r3);

    // Converted using Camera1
    CHECK(Round(rect1FromCamera1.x) == Round(0));   CHECK(Round(rect1FromCamera1.y) == Round(0));
    CHECK(Round(rect1FromCamera1.w) == Round(0));   CHECK(Round(rect1FromCamera1.h) == Round(0));

    CHECK(Round(rect2FromCamera1.x) == Round(20)); CHECK(Round(rect2FromCamera1.y) == Round(-20));
    CHECK(Round(rect2FromCamera1.w) == Round(0));  CHECK(Round(rect2FromCamera1.h) == Round(0));

    CHECK(Round(rect3FromCamera1.x) == Round(-20));  CHECK(Round(rect3FromCamera1.y) == Round(20));
    CHECK(Round(rect3FromCamera1.w) == Round(0));  CHECK(Round(rect3FromCamera1.h) == Round(0));

    // Converted using Camera2
    CHECK(Round(rect1FromCamera2.x) == Round(-50.2));  CHECK(Round(rect1FromCamera2.y) == Round(100.75));
    CHECK(Round(rect1FromCamera2.w) == Round(0));  CHECK(Round(rect1FromCamera2.h) == Round(0));

    CHECK(Round(rect2FromCamera2.x) == Round(-36.86666));  CHECK(Round(rect2FromCamera2.y) == Round(87.41666));
    CHECK(Round(rect2FromCamera2.w) == Round(0)); CHECK(Round(rect2FromCamera2.h) == Round(0));

    CHECK(Round(rect3FromCamera2.x) == Round(-63.53333)); CHECK(Round(rect3FromCamera2.y) == Round(114.08333));
    CHECK(Round(rect3FromCamera2.w) == Round(0));  CHECK(Round(rect3FromCamera2.h) == Round(0));

    // Converted using Camera3
    CHECK(Round(rect1FromCamera3.x) == Round(250)); CHECK(Round(rect1FromCamera3.y) == Round(0));
    CHECK(Round(rect1FromCamera3.w) == Round(0));   CHECK(Round(rect1FromCamera3.h) == Round(0));

    CHECK(Round(rect2FromCamera3.x) == Round(290)); CHECK(Round(rect2FromCamera3.y) == Round(-40));
    CHECK(Round(rect2FromCamera3.w) == Round(0));   CHECK(Round(rect2FromCamera3.h) == Round(0));

    CHECK(Round(rect3FromCamera3.x) == Round(210)); CHECK(Round(rect3FromCamera3.y) == Round(40));
    CHECK(Round(rect3FromCamera3.w) == Round(0));   CHECK(Round(rect3FromCamera3.h) == Round(0));
}

TEST_CASE("Convert SDL_Rect to Rect with same sizes")
{
    const SDL_Rect r1 = {0, 0, 20, 20};
    const SDL_Rect r2 = {20, 20, 10, 10};
    const SDL_Rect r3 = {-20, -20, 50, 50};

    const Rect rect1FromCamera1 = Camera_CalculateRectFromSDLRect(&camera1, windowWidth, windowHeight, &r1);
    const Rect rect2FromCamera1 = Camera_CalculateRectFromSDLRect(&camera1, windowWidth, windowHeight, &r2);
    const Rect rect3FromCamera1 = Camera_CalculateRectFromSDLRect(&camera1, windowWidth, windowHeight, &r3);
    
    const Rect rect1FromCamera2 = Camera_CalculateRectFromSDLRect(&camera2, windowWidth, windowHeight, &r1);
    const Rect rect2FromCamera2 = Camera_CalculateRectFromSDLRect(&camera2, windowWidth, windowHeight, &r2);
    const Rect rect3FromCamera2 = Camera_CalculateRectFromSDLRect(&camera2, windowWidth, windowHeight, &r3);

    const Rect rect1FromCamera3 = Camera_CalculateRectFromSDLRect(&camera3, windowWidth, windowHeight, &r1);
    const Rect rect2FromCamera3 = Camera_CalculateRectFromSDLRect(&camera3, windowWidth, windowHeight, &r2);
    const Rect rect3FromCamera3 = Camera_CalculateRectFromSDLRect(&camera3, windowWidth, windowHeight, &r3);

    // Converted using Camera1
    CHECK(Round(rect1FromCamera1.x) == Round(0));   CHECK(Round(rect1FromCamera1.y) == Round(0));
    CHECK(Round(rect1FromCamera1.w) == Round(20));   CHECK(Round(rect1FromCamera1.h) == Round(20));

    CHECK(Round(rect2FromCamera1.x) == Round(20)); CHECK(Round(rect2FromCamera1.y) == Round(-20));
    CHECK(Round(rect2FromCamera1.w) == Round(10));  CHECK(Round(rect2FromCamera1.h) == Round(10));

    CHECK(Round(rect3FromCamera1.x) == Round(-20));  CHECK(Round(rect3FromCamera1.y) == Round(20));
    CHECK(Round(rect3FromCamera1.w) == Round(50));  CHECK(Round(rect3FromCamera1.h) == Round(50));

    // Converted using Camera2
    CHECK(Round(rect1FromCamera2.x) == Round(-50.2));  CHECK(Round(rect1FromCamera2.y) == Round(100.75));
    CHECK(Round(rect1FromCamera2.w) == Round(13.33333));  CHECK(Round(rect1FromCamera2.h) == Round(13.33333));

    CHECK(Round(rect2FromCamera2.x) == Round(-36.86666));  CHECK(Round(rect2FromCamera2.y) == Round(87.41666));
    CHECK(Round(rect2FromCamera2.w) == Round(6.66666)); CHECK(Round(rect2FromCamera2.h) == Round(6.66666));

    CHECK(Round(rect3FromCamera2.x) == Round(-63.53333)); CHECK(Round(rect3FromCamera2.y) == Round(114.08333));
    CHECK(Round(rect3FromCamera2.w) == Round(33.33333));  CHECK(Round(rect3FromCamera2.h) == Round(33.33333));

    // Converted using Camera3
    CHECK(Round(rect1FromCamera3.x) == Round(250)); CHECK(Round(rect1FromCamera3.y) == Round(0));
    CHECK(Round(rect1FromCamera3.w) == Round(40));   CHECK(Round(rect1FromCamera3.h) == Round(40));

    CHECK(Round(rect2FromCamera3.x) == Round(290)); CHECK(Round(rect2FromCamera3.y) == Round(-40));
    CHECK(Round(rect2FromCamera3.w) == Round(20));   CHECK(Round(rect2FromCamera3.h) == Round(20));

    CHECK(Round(rect3FromCamera3.x) == Round(210)); CHECK(Round(rect3FromCamera3.y) == Round(40));
    CHECK(Round(rect3FromCamera3.w) == Round(100));   CHECK(Round(rect3FromCamera3.h) == Round(100));
}

TEST_CASE("Convert SDL_Rect to Rect with different sizes")
{
    const SDL_Rect r1 = {0, 0, 20, 50};
    const SDL_Rect r2 = {20, 20, 5, 10};
    const SDL_Rect r3 = {-20, -20, 100, 20};

    const Rect rect1FromCamera1 = Camera_CalculateRectFromSDLRect(&camera1, windowWidth, windowHeight, &r1);
    const Rect rect2FromCamera1 = Camera_CalculateRectFromSDLRect(&camera1, windowWidth, windowHeight, &r2);
    const Rect rect3FromCamera1 = Camera_CalculateRectFromSDLRect(&camera1, windowWidth, windowHeight, &r3);
    
    const Rect rect1FromCamera2 = Camera_CalculateRectFromSDLRect(&camera2, windowWidth, windowHeight, &r1);
    const Rect rect2FromCamera2 = Camera_CalculateRectFromSDLRect(&camera2, windowWidth, windowHeight, &r2);
    const Rect rect3FromCamera2 = Camera_CalculateRectFromSDLRect(&camera2, windowWidth, windowHeight, &r3);

    const Rect rect1FromCamera3 = Camera_CalculateRectFromSDLRect(&camera3, windowWidth, windowHeight, &r1);
    const Rect rect2FromCamera3 = Camera_CalculateRectFromSDLRect(&camera3, windowWidth, windowHeight, &r2);
    const Rect rect3FromCamera3 = Camera_CalculateRectFromSDLRect(&camera3, windowWidth, windowHeight, &r3);

    // Converted using Camera1
    CHECK(Round(rect1FromCamera1.x) == Round(0));   CHECK(Round(rect1FromCamera1.y) == Round(0));
    CHECK(Round(rect1FromCamera1.w) == Round(20));   CHECK(Round(rect1FromCamera1.h) == Round(50));

    CHECK(Round(rect2FromCamera1.x) == Round(20)); CHECK(Round(rect2FromCamera1.y) == Round(-20));
    CHECK(Round(rect2FromCamera1.w) == Round(5));  CHECK(Round(rect2FromCamera1.h) == Round(10));

    CHECK(Round(rect3FromCamera1.x) == Round(-20));  CHECK(Round(rect3FromCamera1.y) == Round(20));
    CHECK(Round(rect3FromCamera1.w) == Round(100));  CHECK(Round(rect3FromCamera1.h) == Round(20));

    // Converted using Camera2
    CHECK(Round(rect1FromCamera2.x) == Round(-50.2));  CHECK(Round(rect1FromCamera2.y) == Round(100.75));
    CHECK(Round(rect1FromCamera2.w) == Round(13.33333));  CHECK(Round(rect1FromCamera2.h) == Round(33.33333));

    CHECK(Round(rect2FromCamera2.x) == Round(-36.86666));  CHECK(Round(rect2FromCamera2.y) == Round(87.41666));
    CHECK(Round(rect2FromCamera2.w) == Round(3.33333)); CHECK(Round(rect2FromCamera2.h) == Round(6.66666));

    CHECK(Round(rect3FromCamera2.x) == Round(-63.53333)); CHECK(Round(rect3FromCamera2.y) == Round(114.08333));
    CHECK(Round(rect3FromCamera2.w) == Round(66.66666));  CHECK(Round(rect3FromCamera2.h) == Round(13.33333));

    // Converted using Camera3
    CHECK(Round(rect1FromCamera3.x) == Round(250)); CHECK(Round(rect1FromCamera3.y) == Round(0));
    CHECK(Round(rect1FromCamera3.w) == Round(40));   CHECK(Round(rect1FromCamera3.h) == Round(100));

    CHECK(Round(rect2FromCamera3.x) == Round(290)); CHECK(Round(rect2FromCamera3.y) == Round(-40));
    CHECK(Round(rect2FromCamera3.w) == Round(10));   CHECK(Round(rect2FromCamera3.h) == Round(20));

    CHECK(Round(rect3FromCamera3.x) == Round(210)); CHECK(Round(rect3FromCamera3.y) == Round(40));
    CHECK(Round(rect3FromCamera3.w) == Round(200));   CHECK(Round(rect3FromCamera3.h) == Round(40));
}


// static functions:
static double Round(double num)
{
    return std::ceil(num * 10000) / 10000;
}