#ifndef PROJECTILE_H
#define PROJECTILE_H

#include<stdio.h>
#include<stdlib.h>
#include "utils.h"

typedef struct _projectile {
    int id;
    Transform2D transform;
    Rectangle collider;
    Texture2D texture;
    float movementSpeed;
    float direction;
    float angle;
    int damage;
} Projectile;

Projectile* CreateProjectile(Texture2D texture, int x, int y, int width, int height, float speed, float direction);
void FreeProjectile(Projectile* projectile);
void MoveProjectile(Projectile* projectile);
void DrawProjectile(Projectile* projectile);

#endif