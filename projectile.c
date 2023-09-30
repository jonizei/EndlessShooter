#include "include/raylib.h"
#include "include/projectile.h"
#include<math.h>

// PROJECTILE CONSTANTS

// PUBLIC FUNCTIONS

Projectile* CreateProjectile(Texture2D texture, int x, int y, int width, int height, float speed, float direction)
{
    float projectileArea = width * height;
    float textureArea = texture.height * texture.width;

    Projectile* p = malloc(sizeof(Projectile));
    p->id = -1;
    p->transform.position.x = x;
    p->transform.position.y = y;
    p->transform.size.x = width;
    p->transform.size.y = height;
    p->texture = texture;
    p->movementSpeed = speed;
    p->direction = 0;
    p->damage = 0;
    p->angle = 0;
    p->transform.scale = projectileArea / textureArea;

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
    DrawTextureEx(projectile->texture, projectile->transform.position, projectile->angle, projectile->transform.scale, WHITE);
}

// PRIVATE FUNCTIONS