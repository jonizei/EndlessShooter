#ifndef ENEMY_H
#define ENEMY_H

#include "utils.h"

typedef struct _enemy {
    Transform2D transform;
} Enemy;

typedef struct _rat_enemy {
    Enemy* parent;
} RatEnemy;

RatEnemy* CreateRatEnemy(int x, int y);
void DrawEnemy(Enemy* enemy);
void FreeRatEnemy(RatEnemy* rat);

#endif