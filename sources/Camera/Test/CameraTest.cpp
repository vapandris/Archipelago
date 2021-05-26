#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest>

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

static bool IsNear(double d1, double d2);

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
    CHECK(rect1FromCamera1.x == 0);   CHECK(rect1FromCamera1.y == 0);
    CHECK(rect1FromCamera1.w == 0);   CHECK(rect1FromCamera1.h == 0);

    CHECK(rect2FromCamera1.x == 20); CHECK(rect2FromCamera1.y == -20);
    CHECK(rect2FromCamera1.w == 0);  CHECK(rect2FromCamera1.h == 0);

    CHECK(rect3FromCamera1.x == -20);  CHECK(rect3FromCamera1.y == 20);
    CHECK(rect3FromCamera1.w == 0);  CHECK(rect3FromCamera1.h == 0);

    // Converted using Camera2
    CHECK(rect1FromCamera2.x == -50.2);  CHECK(rect1FromCamera2.y == 100.75);
    CHECK(rect1FromCamera2.w == 0);  CHECK(rect1FromCamera2.h == 0);

    CHECK(rect2FromCamera2.x == -36.8666666667);  CHECK(rect2FromCamera2.y == 87.4166666667);
    CHECK(rect2FromCamera2.w == 0); CHECK(rect2FromCamera2.h == 0);

    CHECK(rect3FromCamera2.x == -63.5333333333); CHECK(rect3FromCamera2.y == 114.0833333333);
    CHECK(rect3FromCamera2.w == 0);  CHECK(rect3FromCamera2.h == 0);

    // Converted using Camera3
    CHECK(rect1FromCamera3.x == 250); CHECK(rect1FromCamera3.y == 0);
    CHECK(rect1FromCamera3.w == 0);   CHECK(rect1FromCamera3.h == 0);

    CHECK(rect2FromCamera3.x == 290); CHECK(rect2FromCamera3.y == -40);
    CHECK(rect2FromCamera3.w == 0);   CHECK(rect2FromCamera3.h == 0);

    CHECK(rect3FromCamera3.x == 210); CHECK(rect3FromCamera3.y == 40);
    CHECK(rect3FromCamera3.w == 0);   CHECK(rect3FromCamera3.h == 0);

/*
    std::cout << "x=" << rect1FromCamera1.x << "y=" << rect1FromCamera1.y << "w=" << rect1FromCamera1.w << "h=" << rect1FromCamera1.h <<  std::endl;
    std::cout << "x=" << rect2FromCamera1.x << "y=" << rect2FromCamera1.y << "w=" << rect2FromCamera1.w << "h=" << rect2FromCamera1.h <<  std::endl;
    std::cout << "x=" << rect3FromCamera1.x << "y=" << rect3FromCamera1.y << "w=" << rect3FromCamera1.w << "h=" << rect3FromCamera1.h <<  std::endl;

    std::cout << "x=" << rect1FromCamera2.x << "y=" << rect1FromCamera2.y << "w=" << rect1FromCamera2.w << "h=" << rect1FromCamera2.h <<  std::endl;
    std::cout << "x=" << rect2FromCamera2.x << "y=" << rect2FromCamera2.y << "w=" << rect2FromCamera2.w << "h=" << rect2FromCamera2.h <<  std::endl;
    std::cout << "x=" << rect3FromCamera2.x << "y=" << rect3FromCamera2.y << "w=" << rect3FromCamera2.w << "h=" << rect3FromCamera2.h <<  std::endl;
    
    std::cout << "x=" << rect1FromCamera3.x << "y=" << rect1FromCamera3.y << "w=" << rect1FromCamera3.w << "h=" << rect1FromCamera3.h <<  std::endl;
    std::cout << "x=" << rect2FromCamera3.x << "y=" << rect2FromCamera3.y << "w=" << rect2FromCamera3.w << "h=" << rect2FromCamera3.h <<  std::endl;
    std::cout << "x=" << rect3FromCamera3.x << "y=" << rect3FromCamera3.y << "w=" << rect3FromCamera3.w << "h=" << rect3FromCamera3.h <<  std::endl;
*/
}

TEST_CASE("Convert SDL_Rect to Rect with same sizes")
{

}

TEST_CASE("Convert SDL_Rect to Rect with different sizes")
{

}


// static functions:
static bool IsNear(double d1, double d2)
{
    
}