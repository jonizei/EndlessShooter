#ifndef PROJECTILE_H
#define PROJECTILE_H

#include<stdio.h>
#include<stdlib.h>
#include "utils.h"

typedef struct _projectile {
    int id;
    Transform2D transform;
    Transform2D collider;
    double movementSpeed;
    double direction;
} Projectile;

Projectile* CreateProjectile(int x, int y, int width, int height, double speed, double direction);
void FreeProjectile(Projectile* projectile);
void MoveProjectile(Projectile* projectile);
void DrawProjectile(Projectile* projectile);

#endif