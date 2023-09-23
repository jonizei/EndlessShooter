#ifndef ENEMY_H
#define ENEMY_H

#include<stdbool.h>
#include "projectile.h"
#include "utils.h"

typedef enum {
    ENEMY_RAT
} EnemyType;

typedef struct _enemy {
    int id;
    EnemyType type;
    Transform2D transform;
    int health;
} Enemy;

// GENERAL ENEMY
Enemy* CreateEnemyByType(EnemyType type, int id, int x, int y);
void DrawEnemy(Enemy* enemy);
void FreeEnemy(Enemy* enemy);
bool IsEnemyAlive(Enemy* enemy);

#endif