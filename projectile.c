#include "include/raylib.h"
#include "include/projectile.h"
#include<math.h>

// PROJECTILE CONSTANTS

// PUBLIC FUNCTIONS

Projectile* CreateProjectile(int x, int y, int width, int height, double speed, double direction, double damage)
{
    Projectile* p = malloc(sizeof(Projectile));
    p->id = -1;
    p->transform.position.x = x;
    p->transform.position.y = y;
    p->transform.size.x = width;
    p->transform.size.y = height;
    p->movementSpeed = speed;
    p->direction = 0;
    p->damage = damage;

    return p;
}

void FreeProjectile(Projectile* projectile)
{
    free(projectile);
    projectile = NULL;
}

void MoveProjectile(Projectile* projectile)
{
    projectile->transform.position.x += projectile->movementSpeed * cos(projectile->direction);
    projectile->transform.position.y += projectile->movementSpeed * sin(projectile->direction); 
}

void DrawProjectile(Projectile* projectile)
{
    DrawRectangleV(projectile->transform.position, projectile->transform.size, BLACK);
}

// PRIVATE FUNCTIONS