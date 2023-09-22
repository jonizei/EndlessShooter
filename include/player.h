#ifndef PLAYER_H
#define PLAYER_H

#include<stdio.h>
#include<stdlib.h>
#include "utils.h"
#include "weapon.h"

typedef struct _player {
    Transform2D transform;
    double movementSpeed;
    Weapon* weapon;
} Player;

Player* CreatePlayer(int x, int y);
void FreePlayer(Player* player);
void MovePlayer(Player* player);
void DrawPlayer(Player* player);
void ShootPlayer(Player* player);
void MoveBullets(int screenWidth, int screenHeight);
void DrawBullets();

#endif