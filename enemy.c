#include "include/raylib.h"
#include "include/enemy.h"
#include "include/utils.h"
#include "include/player.h"
#include "include/raymath.h"
#include<stdio.h>
#include<stdlib.h>

// CONSTANT VALUES

// PRIVATE FUNCTION DECLARATIONS
Enemy* CreateEnemy(int id, int x, int y, int width, int height);
void UpdateEnemyState(Enemy* enemy);
void MoveEnemy(Enemy* enemy);
bool IsPlayerInRange(Enemy* enemy, Player* player);
Vector2 GetRandomTarget(Enemy* enemy);

// PUBLIC FUNCTIONS

Enemy* CreateEnemyByType(EnemyType type, int id, int x, int y)
{
    int width = 0;
    int height = 0;
    int health = 0;
    EnemyState defaultState = ENEMY_STATE_IDLE;
    float sightRadius = 200;
    float wanderRadius = 150.0;
    float wanderTargetTime = 5.0;
    float movementSpeed = 1.0;

    switch(type)
    {
        case ENEMY_RAT:
            width = 16;
            height = 16;
            health = 50;
            defaultState = ENEMY_STATE_WANDER;
        break;
    }

    Enemy* enemy = CreateEnemy(id, x, y, width, height);
    enemy->type = type;
    enemy->health = health;
    enemy->state = defaultState;
    enemy->defaultState = defaultState;
    enemy->movementSpeed = movementSpeed;
    enemy->sightRadius = sightRadius;
    enemy->wanderRadius = wanderRadius;
    enemy->wanderTargetTime = wanderTargetTime;

    return enemy;
}

void DrawEnemy(Enemy* enemy)
{
    DrawRectangleV(enemy->transform.position, enemy->transform.size, RED);
}

void FreeEnemy(Enemy* enemy)
{
    MyFree(&enemy);
}

bool IsEnemyAlive(Enemy* enemy)
{
    return enemy->health > 0;
}

void UpdateEnemy(Enemy* enemy)
{
    UpdateEnemyState(enemy);
    MoveEnemy(enemy);
}

// PRIVATE FUNCTIONS

Enemy* CreateEnemy(int id, int x, int y, int width, int height)
{
    Enemy* enemy = malloc(sizeof(Enemy));
    enemy->id = id;
    enemy->transform.position.x = x;
    enemy->transform.position.y = y;
    enemy->startPosition.x = x;
    enemy->startPosition.y = y;
    enemy->transform.size.x = width;
    enemy->transform.size.y = height;
    enemy->health = 0;
    enemy->lastTarget = 0;

    return enemy;
}

void UpdateEnemyState(Enemy* enemy)
{
    bool playerInRange = IsPlayerInRange(enemy, GetPlayer());
    if (playerInRange)
    {
        enemy->state = ENEMY_STATE_PURSUE;
    }
    else if (playerInRange == false && enemy->state == ENEMY_STATE_PURSUE)
    {
        enemy->state = enemy->defaultState;
    }
}

void MoveEnemy(Enemy* enemy)
{
    if (enemy->state == ENEMY_STATE_IDLE)
    {
        return;
    }

    Vector2 position;

    if (enemy->state == ENEMY_STATE_WANDER)
    {
        if (GetTime() - enemy->lastTarget > enemy->wanderTargetTime)
        {
            enemy->wanderTarget = GetRandomTarget(enemy);
            enemy->lastTarget = GetTime();
        }
        
        position = Vector2MoveTowards(enemy->transform.position, enemy->wanderTarget, enemy->movementSpeed);
    }
    else if (enemy->state == ENEMY_STATE_PURSUE)
    {
        position = Vector2MoveTowards(enemy->transform.position, GetPlayer()->transform.position, enemy->movementSpeed);
    }

    enemy->transform.position.x = position.x;
    enemy->transform.position.y = position.y;

}

bool IsPlayerInRange(Enemy* enemy, Player* player)
{
    float distance = Vector2Distance(enemy->transform.position, player->transform.position);

    if (distance <= enemy->sightRadius)
    {
        return true;
    }

    return false;
}

Vector2 GetRandomTarget(Enemy* enemy)
{
    int minX = enemy->startPosition.x - enemy->wanderRadius;
    int maxX = enemy->startPosition.x + enemy->wanderRadius;
    int minY = enemy->startPosition.y - enemy->wanderRadius;
    int maxY = enemy->startPosition.y + enemy->wanderRadius;

    int randX = GetRandomValue(minX, maxX);
    int randY = GetRandomValue(minY, maxY);

    return (Vector2){ randX, randY };
}