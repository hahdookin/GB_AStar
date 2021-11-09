#ifndef ASTAR_H
#define ASTAR_H

#include <gb/gb.h>

#include "Common.h"
#include "Floor16Map.h"

#define NODES_W 10
#define NODES_H 9

#define INFINITY 0xFFU

#define OBSTACLE (1 << 0)
#define VISITED  (1 << 1)

struct Node {
    UBYTE flags;
    UINT8 globalGoal;
    UINT8 localGoal;
    Vec2u pos;

    struct Node *neighbors[4];
    UINT8 neighborCount;
    struct Node *parent;
};

extern struct Node nodes[NODES_W * NODES_H];
extern struct Node *startNode;
extern struct Node *endNode;

extern void initialize_nodes();
extern void determine_obstacles();
extern void AStar();
extern UINT8 isqrt(UINT8 num);

#endif
