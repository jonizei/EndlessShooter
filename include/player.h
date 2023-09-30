#ifndef PLAYER_H
#define PLAYER_H

#include<stdio.h>
#include<stdlib.h>
#include "utils.h"
#include "weapon.h"
#include "enemy.h"

typedef struct _player {
    Transform2D transform;
    float movementSpeed;
    Weapon* weapon;
    Texture2D texture;
} Player;

Player* CreatePlayer(int x, int y);
void FreePlayer(Player* player);
void MovePlayer(Player* player);
void DrawPlayer(Player* player);
void ShootPlayer(Player* player);
void MoveBullets();
void DrawBullets();
void CheckEnemyCollisionWithBullets(Enemy* enemy);
Player* GetPlayer();
void LoadPlayerResources();
void UnloadPlayerResources();

#endif