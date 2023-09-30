#include "include/raylib.h"
#include "include/weapon.h"

// PUBLIC FUNCTIONS

Weapon* CreateWeapon()
{
    Weapon* w = malloc(sizeof(Weapon));
    w->speed = 10;
    w->attackSpeed = 0.1;
    w->lashShot = 0;

    return w;
}

void FreeWeapon(Weapon* weapon)
{
    MyFree(&weapon);
}

// PRIVATE FUNCTIONS