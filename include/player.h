#ifndef PLAYER_H
#define PLAYER_H

#include<stdio.h>
#include<stdlib.h>
#include "utils.h"
#include "weapon.h"
#include "enemy.h"

typedef struct _player {
    Transform2D transform;
    Rectangle collider;
    float movementSpeed;
    Weapon* weapon;
    Texture2D texture;
    Box2D hitArea;
    float health;
} Player;

Player* CreatePlayer(int x, int y);
void FreePlayer(Player* player);
void UpdatePlayer(Player* player);
void DrawPlayer(Player* player);
void DrawBullets();
void CheckEnemyCollisionWithBullets(Enemy* enemy);
Player* GetPlayer();
void LoadPlayerResources();
void UnloadPlayerResources();
void TakeDamage(Player* player, float damage);

#endif