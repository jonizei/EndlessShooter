#ifndef ENEMY_H
#define ENEMY_H

#include<stdbool.h>
#include "projectile.h"
#include "utils.h"

typedef enum {
    ENEMY_RAT,
    ENEMY_GOBLIN
} EnemyType;

typedef enum {
    ENEMY_STATE_IDLE,
    ENEMY_STATE_WANDER,
    ENEMY_STATE_PURSUE
} EnemyMovementState;

typedef enum {
    ENEMY_ATTACK_NONE,
    ENEMY_ATTACK_HIT,
    ENEMY_ATTACK_SHOOT
} EnemyAttackType;

typedef struct _enemy {
    int id;
    EnemyType type;
    Transform2D transform;
    Rectangle collider;
    Vector2 startPosition;
    Texture2D texture;
    float health;
    float damage;
    float attackSpeed;
    float lastAttack;
    EnemyMovementState defaultMovementState;
    EnemyMovementState movementState;
    EnemyAttackType attackType;
    float movementSpeed;
    float sightRadius;
    float wanderRadius;
    float lastTarget;
    float wanderTargetTime;
    Vector2 wanderTarget;
    Box2D hitArea;
} Enemy;

// GENERAL ENEMY
Enemy* CreateEnemyByType(EnemyType type, int id, int x, int y);
void DrawEnemy(Enemy* enemy);
void FreeEnemy(Enemy* enemy);
bool IsEnemyAlive(Enemy* enemy);
void UpdateEnemy(Enemy* enemy);
void LoadEnemyResources();
void UnloadEnemyResources();
Vector2 GetEnemyOrigin(Enemy* enemy);

#endif