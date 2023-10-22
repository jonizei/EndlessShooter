#include "include/raylib.h"
#include "include/enemy_handler.h"
#include<stdio.h>
#include<stdlib.h>

#define MAX_ENEMY 200

// CONSTANT VALUES


// GLOBAL VARIABLES
Enemy* enemyPool[MAX_ENEMY];
int enemyCount = 0;
int enemyId = 1;

// PRIVATE FUNCTION DECLARATIONS

void AddToEnemyPool(Enemy* enemy);
void RemoveFromEnemyPool(int id);

// PUBLIC FUNCTIONS

void SpawnEnemy(EnemyType type, int x, int y)
{
    Enemy* enemy = CreateEnemyByType(type, enemyId, x, y);
    AddToEnemyPool(enemy);
    enemyId++;
}

void DrawEnemies()
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (enemyPool[i] != NULL)
        {
            DrawEnemy(enemyPool[i]);
        }
    }
}

void CheckEnemyCollision()
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (enemyPool[i] != NULL)
        {
            CheckEnemyCollisionWithBullets(enemyPool[i]);
        }
    }
}

void UpdateEnemies()
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (enemyPool[i] != NULL)
        {
            if (IsEnemyAlive(enemyPool[i]))
            {
                UpdateEnemy(enemyPool[i]);
            }
            else 
            {
                EnemyDie(enemyPool[i]);
                RemoveFromEnemyPool(enemyPool[i]->id);
            }
        }
    }
}

void FreeEnemies()
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        FreeEnemy(enemyPool[i]);
    }
}

EnemySpawner* CreateEnemySpawer(EnemyType enemyType, double spawnTime, int spawnDistance, int x, int y)
{
    EnemySpawner* spawner = malloc(sizeof(EnemySpawner));
    spawner->transform.position.x = x;
    spawner->transform.position.y = y;
    spawner->transform.size.x = spawnDistance;
    spawner->transform.size.y = spawnDistance;
    spawner->enemyType = enemyType;
    spawner->spawnTime = spawnTime;
    spawner->lastSpawn = 0;

    return spawner;
}

void FreeEnemySpawner(EnemySpawner* spawner)
{
    MyFree(&spawner);
}

void UpdateSpawner(EnemySpawner* spawner)
{
    if (GetTime() - spawner->lastSpawn > spawner->spawnTime)
    {
        int x = GetRandomValue(spawner->transform.position.x, spawner->transform.position.x + spawner->transform.size.x);
        int y = GetRandomValue(spawner->transform.position.y, spawner->transform.position.y + spawner->transform.size.y);
        SpawnEnemy(spawner->enemyType, x, y);
        spawner->lastSpawn = GetTime();
    }
}

// PRIVATE FUNCTIONS)

void AddToEnemyPool(Enemy* enemy)
{
    for(int i = 0; i < MAX_ENEMY; i++)
    {
        if(enemyPool[i] == NULL)
        {
            enemyPool[i] = enemy;
            enemyCount++;
            break;
        }
    }
}

void RemoveFromEnemyPool(int id)
{
    for(int i = 0; i < MAX_ENEMY; i++)
    {
        if(enemyPool[i] != NULL)
        {
            if(enemyPool[i]->id == id) 
            {
                FreeEnemy(enemyPool[i]);
                enemyPool[i] = NULL;
                enemyCount--;
                break;
            }
        }
    }
}