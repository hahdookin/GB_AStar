#include "Common.h"

void performant_delay(UINT8 loops)
{
    UINT8 i;
    for (i = 0; i < loops; i++)
        wait_vbl_done();
}

void animate_sprite(UINT8 sprite, INT8 movex, INT8 movey)
{
    while (movex != 0) 
    {
        scroll_sprite(sprite, movex < 0 ? -1 : 1, 0);
        movex += movex < 0 ? 1 : -1;
        wait_vbl_done();
    }
    while (movey != 0)
    {
        scroll_sprite(sprite, 0, movey < 0 ? -1 : 1);
        movey += movey < 0 ? 1 : -1;
        wait_vbl_done();
    }
}
