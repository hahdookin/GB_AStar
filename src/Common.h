#ifndef COMMON_H
#define COMMON_H

#include <gb/gb.h>

#define TRUE  1
#define FALSE 0
#define NULL  0

#define FLOOR   100
#define GRAVITY 2

typedef struct Vec2i {
    INT8 x, y;
} Vec2i;

typedef struct Vec2u {
    UINT8 x, y;
} Vec2u;

typedef struct Rect {
    UINT8 x, y, w, h;
} Rect;

void performant_delay(UINT8 loops);
void animate_sprite(UINT8 sprite, INT8 movex, INT8 movey);

#endif
