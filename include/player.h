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
    Stats stats;
    Weapon* weapon;
    Texture2D texture;
    Box2D hitArea;
    float maxHealth;
    float health;
    float movementSpeed;
    float damage;
    float attackSpeed;
} Player;

Player* CreatePlayer(float x, float y, Stats stats);
void FreePlayer(Player* player);
void UpdatePlayer(Player* player);
void DrawPlayer(Player* player);
void CheckEnemyCollisionWithBullets(Enemy* enemy);
Player* GetPlayer();
void LoadPlayerResources();
void UnloadPlayerResources();
void TakeDamage(Player* player, float damage);
void UpdatePlayerStats(Player* player);

#endif