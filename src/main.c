#include <gb/gb.h>

#include "Common.h"
#include "Skeleton.h"
#include "Hero.h"
#include "Floor16.h"
#include "Floor16Map.h"
#include "Cursor16.h"

#include "AStar.h"

#define WORLD_TO_MAP_X(x) ((x - 8) / 8)
#define WORLD_TO_MAP_Y(y) ((y - 16) / 8)
#define TILES_W 20
#define TILES_H 18

#define X_OFF 8
#define Y_OFF 16
#define SPRITE_SIZE 16

UINT16 idxTL;
UINT16 idxTR;
UINT16 idxBL;
UINT16 idxBR;

struct Actor {
    Vec2u pos;
};

void main() 
{
    struct Node *p;
    struct Actor cursor, skeleton, hero;

    cursor.pos.x = X_OFF;
    cursor.pos.y = Y_OFF;
    skeleton.pos.x = X_OFF + 8 * SPRITE_SIZE;
    skeleton.pos.y = Y_OFF + 4 * SPRITE_SIZE;
    hero.pos.x = X_OFF + 1 * SPRITE_SIZE;
    hero.pos.y = Y_OFF + 4 * SPRITE_SIZE;

    // Load background
    set_bkg_data(0x0, 8, Floor16);
    set_bkg_tiles(0, 0, 20, 18, Floor16Map);

    // Load cursor
    set_sprite_data(0x00, 4, Cursor16);
    set_sprite_tile(0x00, 0x00);
    set_sprite_tile(0x01, 0x01);
    set_sprite_tile(0x02, 0x02);
    set_sprite_tile(0x03, 0x03);

    // Load skeleton
    set_sprite_data(0x04, 4, Skeleton);
    set_sprite_tile(0x04, 0x04);
    set_sprite_tile(0x05, 0x05);
    set_sprite_tile(0x06, 0x06);
    set_sprite_tile(0x07, 0x07);

    // Load hero
    set_sprite_data(0x08, 4, Hero);
    set_sprite_tile(0x08, 0x08);
    set_sprite_tile(0x09, 0x09);
    set_sprite_tile(0x0a, 0x0a);
    set_sprite_tile(0x0b, 0x0b);

    startNode = &nodes[4 * NODES_W + 1];
    endNode = &nodes[4 * NODES_W + 8];

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    while (1)
    {
        if (joypad() & J_LEFT && cursor.pos.x > X_OFF) {
            cursor.pos.x -= SPRITE_SIZE;
        }
        if (joypad() & J_RIGHT && cursor.pos.x < 8 * (TILES_W - 1)) {
            cursor.pos.x += SPRITE_SIZE;
        }
        if (joypad() & J_UP && cursor.pos.y > Y_OFF) {
            cursor.pos.y -= SPRITE_SIZE;
        }
        if (joypad() & J_DOWN && cursor.pos.y < 8 * (TILES_H - 1)) {
            cursor.pos.y += SPRITE_SIZE;
        }

        // Run the A* algorithm
        if (joypad() & J_START)
        {
            // Hide cursor by moving offscreen
            move_sprite(0x00, 0, 0);
            move_sprite(0x01, 0, 0);
            move_sprite(0x02, 0, 0);
            move_sprite(0x03, 0, 0);

            initialize_nodes();
            determine_obstacles();
            AStar();

            if (endNode != NULL)
            {
                // Traverse the path discovered
                p = endNode;
                while (p->parent != NULL)
                {
                    skeleton.pos.x = X_OFF + p->pos.x * SPRITE_SIZE;
                    skeleton.pos.y = Y_OFF + p->pos.y * SPRITE_SIZE;

                    move_sprite(0x04, skeleton.pos.x, skeleton.pos.y);
                    move_sprite(0x05, skeleton.pos.x, skeleton.pos.y + 8);
                    move_sprite(0x06, skeleton.pos.x + 8, skeleton.pos.y);
                    move_sprite(0x07, skeleton.pos.x + 8, skeleton.pos.y + 8);

                    p = p->parent;
                    performant_delay(6);
                }
            }
        } 
        else if (joypad() & J_SELECT)
        {
            skeleton.pos.x = X_OFF + 8 * SPRITE_SIZE;
            skeleton.pos.y = Y_OFF + 4 * SPRITE_SIZE;
        }

        // (x, y)
        idxTL = (UINT16)WORLD_TO_MAP_Y(cursor.pos.y) * (UINT16)TILES_W + (UINT16)WORLD_TO_MAP_X(cursor.pos.x);
        // (x + 1, y)
        idxTR = (UINT16)WORLD_TO_MAP_Y(cursor.pos.y) * (UINT16)TILES_W + (UINT16)WORLD_TO_MAP_X(cursor.pos.x) + 1;
        // (x, y + 1)
        idxBL = ((UINT16)WORLD_TO_MAP_Y(cursor.pos.y) + 1) * (UINT16)TILES_W + (UINT16)WORLD_TO_MAP_X(cursor.pos.x);
        // (x + 1, y + 1)
        idxBR = ((UINT16)WORLD_TO_MAP_Y(cursor.pos.y) + 1) * (UINT16)TILES_W + (UINT16)WORLD_TO_MAP_X(cursor.pos.x) + 1;

        if (joypad() & J_A) {
            Floor16Map[idxTL] = 0x00;
            Floor16Map[idxBL] = 0x01;
            Floor16Map[idxTR] = 0x02;
            Floor16Map[idxBR] = 0x03;

            set_bkg_tiles(0, 0, 20, 18, Floor16Map);
        }
        if (joypad() & J_B) {
            Floor16Map[idxTL] = 0x04;
            Floor16Map[idxBL] = 0x05;
            Floor16Map[idxTR] = 0x06;
            Floor16Map[idxBR] = 0x07;

            set_bkg_tiles(0, 0, 20, 18, Floor16Map);
        }

        // Move cursor
        move_sprite(0x00, cursor.pos.x, cursor.pos.y);
        move_sprite(0x01, cursor.pos.x, cursor.pos.y + 8);
        move_sprite(0x02, cursor.pos.x + 8, cursor.pos.y);
        move_sprite(0x03, cursor.pos.x + 8, cursor.pos.y + 8);

        // Move skeleton
        move_sprite(0x04, skeleton.pos.x, skeleton.pos.y);
        move_sprite(0x05, skeleton.pos.x, skeleton.pos.y + 8);
        move_sprite(0x06, skeleton.pos.x + 8, skeleton.pos.y);
        move_sprite(0x07, skeleton.pos.x + 8, skeleton.pos.y + 8);

        // Move hero
        move_sprite(0x08, hero.pos.x, hero.pos.y);
        move_sprite(0x09, hero.pos.x, hero.pos.y + 8);
        move_sprite(0x0a, hero.pos.x + 8, hero.pos.y);
        move_sprite(0x0b, hero.pos.x + 8, hero.pos.y + 8);

        performant_delay(6);
    }
}
