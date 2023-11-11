#include "include/raylib.h"
#include "include/weapon.h"
#include<math.h>

// PUBLIC FUNCTIONS

Weapon* CreateWeapon(Texture2D texture, int width, int height, float speed, float attackSpeed)
{
    Weapon* w = malloc(sizeof(Weapon));
    w->transform.size.x = width;
    w->transform.size.y = height;
    w->texture = texture;
    w->speed = speed;
    w->attackSpeed = attackSpeed;
    w->lastShot = 0;
    w->offset = 0;
    w->flipped = false;

    return w;
}

void DrawWeapon(Weapon* weapon)
{
    DrawTextureBySize(weapon->texture, weapon->transform.position, weapon->transform.size, weapon->transform.rotation, false, weapon->flipped);
}

void FreeWeapon(Weapon* weapon)
{
    MyFree((void**)&weapon);
}

// PRIVATE FUNCTIONS