#include "Common.h"

void performant_delay(UINT8 loops)
{
    UINT8 i;
    for (i = 0; i < loops; i++)
        wait_vbl_done();
}

void move_sprite_smooth(UINT8 sprite, UINT8 oldX, UINT8 oldY, UINT8 newX, UINT8 newY)
{
    INT8 dx, dy;

    dx = newX - oldX;
    dy = newY - oldY;

    while (dx != 0) 
    {
        scroll_sprite(sprite, dx < 0 ? -1 : 1, 0);
        dx += dx < 0 ? 1 : -1;
        wait_vbl_done();
    }
    while (dy != 0)
    {
        scroll_sprite(sprite, 0, dy < 0 ? -1 : 1);
        dy += dy < 0 ? 1 : -1;
        wait_vbl_done();
    }
}
