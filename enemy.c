#include "include/raylib.h"
#include "include/enemy.h"
#include<stdio.h>
#include<stdlib.h>

// CONSTANT VALUES

// PRIVATE FUNCTION DECLARATIONS
Enemy* CreateEnemy(int x, int y, int width, int height);
void FreeEnemy(Enemy* enemy);

// PUBLIC FUNCTIONS

RatEnemy* CreateRatEnemy(int x, int y)
{
    RatEnemy* rat = malloc(sizeof(RatEnemy));
    rat->parent = CreateEnemy(x, y, 16, 16);

    return rat;
}

void DrawEnemy(Enemy* enemy)
{
    DrawRectangleV(enemy->transform.position, enemy->transform.size, RED);
}

void FreeRatEnemy(RatEnemy* rat)
{
    MyFree(&rat);
}

// PRIVATE FUNCTIONS

Enemy* CreateEnemy(int x, int y, int width, int height)
{
    Enemy* enemy = malloc(sizeof(Enemy));
    enemy->transform.position.x = x;
    enemy->transform.position.y = y;
    enemy->transform.size.x = width;
    enemy->transform.size.y = height;

    return enemy;
}

void FreeEnemy(Enemy* enemy)
{
    MyFree(&enemy);
}
