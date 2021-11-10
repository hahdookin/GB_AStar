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

struct Actor {
    union {
        struct {
            UINT8 TL;
            UINT8 BL;
            UINT8 TR;
            UINT8 BR;
        };
        UINT8 tiles[4];
    };
    Vec2u pos;
};

extern void performant_delay(UINT8 loops);
// Smoothly move a sprite from oldPos to newPos
extern void move_actor_smooth(struct Actor *actor, Vec2u *oldPos, Vec2u *newPos);

#endif
