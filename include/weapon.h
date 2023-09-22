#ifndef WEAPON_H
#define WEAPON_H

#include<stdio.h>
#include<stdlib.h>
#include "utils.h"

typedef struct _weapon {
    Transform2D transform;
    double speed;
    double attackSpeed;
    double lashShot;
} Weapon;

Weapon* CreateWeapon();
void FreeWeapon(Weapon* weapon);

#endif