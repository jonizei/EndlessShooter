#ifndef WEAPON_H
#define WEAPON_H

#include<stdio.h>
#include<stdlib.h>
#include "utils.h"

typedef struct _weapon {
    Transform2D transform;
    float offset;
    float speed;
    float attackSpeed;
    float lastShot;
} Weapon;

Weapon* CreateWeapon(float speed, float attackSpeed);
void FreeWeapon(Weapon* weapon);

#endif