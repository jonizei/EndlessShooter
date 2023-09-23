#include "include/raylib.h"
#include "include/enemy.h"
#include "include/utils.h"
#include<stdio.h>
#include<stdlib.h>

// CONSTANT VALUES

// PRIVATE FUNCTION DECLARATIONS
Enemy* CreateEnemy(int id, int x, int y, int width, int height);

// PUBLIC FUNCTIONS

Enemy* CreateEnemyByType(EnemyType type, int id, int x, int y)
{
    int width = 0;
    int height = 0;
    int health = 0;

    switch(type)
    {
        case ENEMY_RAT:
            width = 16;
            height = 16;
            health = 50;
        break;
    }

    Enemy* enemy = CreateEnemy(id, x, y, width, height);
    enemy->type = type;
    enemy->health = health;

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

// PRIVATE FUNCTIONS

Enemy* CreateEnemy(int id, int x, int y, int width, int height)
{
    Enemy* enemy = malloc(sizeof(Enemy));
    enemy->id = id;
    enemy->transform.position.x = x;
    enemy->transform.position.y = y;
    enemy->transform.size.x = width;
    enemy->transform.size.y = height;
    enemy->health = 0;

    return enemy;
}
