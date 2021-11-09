#include "AStar.h"

#define distance(a, b) \
    (isqrt( (a->pos.x - b->pos.x) * (a->pos.x - b->pos.x) + (a->pos.y - b->pos.y) * (a->pos.y - b->pos.y)))

#define heuristic(a, b) (distance(a, b))

struct Node nodes[NODES_W * NODES_H];

struct Node *startNode = &nodes[(NODES_H / 2) * NODES_W + 1];
struct Node *endNode = &nodes[(NODES_H / 2) * NODES_W + NODES_W - 2];

UINT8 isqrt(UINT8 num) {
    UINT8 res = 0;
    UINT8 bit = 1 << 6;
    
    while (bit > num)
        bit >>= 2;
        
    while (bit != 0) {
        if (num >= res + bit) {
            num -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

void initialize_nodes()
{
    UINT8 x, y, i;

    for (x = 0; x < NODES_W; x++)
        for (y = 0; y < NODES_H; y++)
        {
            nodes[y * NODES_W + x].pos.x = x;
            nodes[y * NODES_W + x].pos.y = y;
            nodes[y * NODES_W + x].parent = NULL;
            nodes[y * NODES_W + x].flags = 0;
        }

    for (x = 0; x < NODES_W; x++)
        for (y = 0; y < NODES_H; y++)
        {
            i = 0;
            if (y > 0)
                nodes[y * NODES_W + x].neighbors[i++] = &nodes[(y - 1) * NODES_W + (x + 0)];
            if (y < NODES_H - 1)
                nodes[y * NODES_W + x].neighbors[i++] = &nodes[(y + 1) * NODES_W + (x + 0)];
            if (x > 0)
                nodes[y * NODES_W + x].neighbors[i++] = &nodes[(y + 0) * NODES_W + (x - 1)];
            if (x < NODES_W - 1)
                nodes[y * NODES_W + x].neighbors[i++] = &nodes[(y + 0) * NODES_W + (x + 1)];

            nodes[y * NODES_W + x].neighborCount = i;
        }
}

void determine_obstacles()
{
    UINT8 x, y;

    for (x = 0; x < Floor16MapWidth; x += 2)
        for (y = 0; y < Floor16MapHeight; y += 2)
        {
            if (Floor16Map[y * Floor16MapWidth + x] == 0x04)
            {
                // Is obstacle
                nodes[(y / 2) * NODES_W + (x / 2)].flags |= OBSTACLE;
            }
            else
            {
                nodes[(y / 2) * NODES_W + (x / 2)].flags &= ~(OBSTACLE);
            }
        }
}

void AStar()
{
    struct Node *curNode, *nodesNotTested[50], *temp;
    UINT8 x, y, len, i, j, possibleLowerGoal;

    len = 0;

    for (x = 0; x < NODES_W; x++)
        for (y = 0; y < NODES_H; y++)
        {
            nodes[y * NODES_W + x].globalGoal = INFINITY;
            nodes[y * NODES_W + x].localGoal = INFINITY;
            nodes[y * NODES_W + x].parent = NULL;
        }

    startNode->localGoal = 0;
    startNode->globalGoal = heuristic(startNode, endNode);

    nodesNotTested[len++] = startNode;

    while (!(len == 0) && curNode != endNode)
    {
        // Sort nodesNotTested
        for (i = 0; i < len - 1; i++)
            for (j = 0; j < len - i - 1; j++)
            {
                if (nodesNotTested[j]->globalGoal > nodesNotTested[j + 1]->globalGoal)
                {
                    temp = nodesNotTested[j];
                    nodesNotTested[j] = nodesNotTested[j + 1];
                    nodesNotTested[j + 1] = temp;
                }
            }

        // Pop front
        while (!(len == 0) && (nodesNotTested[0]->flags & VISITED))
        {
            if (len == 1)
            {
                nodesNotTested[0] = NULL;
                len--;
            }
            else 
            {
                for (i = 1; i < len; i++)
                    nodesNotTested[i - 1] = nodesNotTested[i];
                len--;
            }
        }

        // No nodes left to test
        if (len == 0)
            break;

        curNode = nodesNotTested[0];
        curNode->flags |= VISITED;

        for (i = 0; i < curNode->neighborCount; i++)
        {
            if (
                !(curNode->neighbors[i]->flags & VISITED) && 
                !(curNode->neighbors[i]->flags & OBSTACLE)
            ) {
                nodesNotTested[len++] = curNode->neighbors[i];
            }

            possibleLowerGoal = curNode->localGoal + distance(curNode, curNode->neighbors[i]);

            if (possibleLowerGoal < curNode->neighbors[i]->localGoal)
            {
                curNode->neighbors[i]->parent = curNode;
                curNode->neighbors[i]->localGoal = possibleLowerGoal;

                curNode->neighbors[i]->globalGoal = curNode->neighbors[i]->localGoal + heuristic(curNode->neighbors[i], endNode);
            }
        }
    }
}
