#include "Common.h"

void performant_delay(UINT8 loops)
{
    UINT8 i;
    for (i = 0; i < loops; i++)
        wait_vbl_done();
}

void move_actor_smooth(struct Actor *actor, Vec2u *oldPos, Vec2u *newPos)
{
    INT8 dx, dy;

    dx = newPos->x - oldPos->x;
    dy = newPos->y - oldPos->y;

    while (dx != 0) 
    {
        scroll_sprite(actor->TL, dx < 0 ? -1 : 1, 0);
        scroll_sprite(actor->BL, dx < 0 ? -1 : 1, 0);
        scroll_sprite(actor->TR, dx < 0 ? -1 : 1, 0);
        scroll_sprite(actor->BR, dx < 0 ? -1 : 1, 0);
        dx += dx < 0 ? 1 : -1;
        wait_vbl_done();
    }
    while (dy != 0)
    {
        scroll_sprite(actor->TL, 0, dy < 0 ? -1 : 1);
        scroll_sprite(actor->BL, 0, dy < 0 ? -1 : 1);
        scroll_sprite(actor->TR, 0, dy < 0 ? -1 : 1);
        scroll_sprite(actor->BR, 0, dy < 0 ? -1 : 1);
        dy += dy < 0 ? 1 : -1;
        wait_vbl_done();
    }

}
