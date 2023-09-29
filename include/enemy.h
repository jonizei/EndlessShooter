#ifndef ENEMY_H
#define ENEMY_H

#include<stdbool.h>
#include "projectile.h"
#include "utils.h"

typedef enum {
    ENEMY_RAT
} EnemyType;

typedef enum {
    ENEMY_STATE_IDLE,
    ENEMY_STATE_WANDER,
    ENEMY_STATE_PURSUE
} EnemyState;

typedef struct _enemy {
    int id;
    EnemyType type;
    Transform2D transform;
    Vector2 startPosition;
    int health;
    EnemyState defaultState;
    EnemyState state;
    float movementSpeed;
    float sightRadius;
    float wanderRadius;
    float lastTarget;
    float wanderTargetTime;
    Vector2 wanderTarget;
} Enemy;

// GENERAL ENEMY
Enemy* CreateEnemyByType(EnemyType type, int id, int x, int y);
void DrawEnemy(Enemy* enemy);
void FreeEnemy(Enemy* enemy);
bool IsEnemyAlive(Enemy* enemy);
void UpdateEnemy(Enemy* enemy);

#endif