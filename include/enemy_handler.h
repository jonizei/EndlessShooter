#ifndef ENEMY_HANDLER_H
#define ENEMY_HANDLER_H

#include "enemy.h"
#include "player.h"

typedef struct _enemy_spawner {
    Transform2D transform;
    EnemyType enemyType;
    double spawnTime;
    double lastSpawn;
} EnemySpawner;

void SpawnEnemy(EnemySpawner* spawner, int x, int y);
void DrawEnemies();
void CheckEnemyCollision();
void UpdateEnemies();
void FreeEnemies();
EnemySpawner* CreateEnemySpawer(EnemyType enemyType, double spawnTime, int spawnDistance, int x, int y);
void FreeEnemySpawner(EnemySpawner* spawner);
void UpdateSpawner(EnemySpawner* spawner);

#endif