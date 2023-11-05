#include "include/raylib.h"
#include "include/weapon.h"

// PUBLIC FUNCTIONS

Weapon* CreateWeapon(float speed, float attackSpeed)
{
    Weapon* w = malloc(sizeof(Weapon));
    w->speed = speed;
    w->attackSpeed = attackSpeed;
    w->lastShot = 0;
    w->offset = 0;

    return w;
}

void FreeWeapon(Weapon* weapon)
{
    MyFree((void**)&weapon);
}

// PRIVATE FUNCTIONS