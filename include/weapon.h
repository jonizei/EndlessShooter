#ifndef WEAPON_H
#define WEAPON_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "utils.h"

typedef struct _weapon {
    Transform2D transform;
    Texture2D texture;
    Vector2 barrelPoint;
    float offset;
    float speed;
    float attackSpeed;
    float lastShot;
    bool flipped;
} Weapon;

Weapon* CreateWeapon(Texture2D texture, int width, int height, float speed, float attackSpeed);
void DrawWeapon(Weapon* weapon);
void FreeWeapon(Weapon* weapon);

#endif