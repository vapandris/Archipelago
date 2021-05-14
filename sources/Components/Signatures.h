#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Components_Signature
{
    COLLISION_SIGNATURE =    0x0001,
    GRAPHICS_SIGNATURE  =    0x0002,
    INPUT_SIGNATURE     =    0x0004
} Components_Signature;

#ifdef __cplusplus
}
#endif