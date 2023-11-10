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
    int damage;
} Projectile;

Projectile* CreateProjectile(Texture2D texture, int x, int y, int width, int height, float speed, float direction);
void FreeProjectile(Projectile* projectile);
void MoveProjectile(Projectile* projectile);
void DrawProjectilePool(Projectile** pool, size_t size);
void DrawProjectile(Projectile* projectile);
bool AddToProjectilePool(Projectile** pool, Projectile* projectile, size_t size);
bool RemoveFromProjectilePool(Projectile** pool, int id, size_t size);
void FreeProjectilePool(Projectile** pool, size_t size);

#endif