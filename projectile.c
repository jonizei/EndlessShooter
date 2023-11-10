#include "include/raylib.h"
#include "include/projectile.h"
#include<math.h>

// PROJECTILE CONSTANTS

// PRIVATE FUNCTION DECLARATIONS
void MoveProjectileCollider(Projectile* projectile);

// PUBLIC FUNCTIONS

Projectile* CreateProjectile(Texture2D texture, int x, int y, int width, int height, float speed, float direction)
{
    Projectile* p = malloc(sizeof(Projectile));
    p->id = -1;
    p->transform.position.x = x;
    p->transform.position.y = y;
    p->transform.size.x = width;
    p->transform.size.y = height;
    p->collider.x = x;
    p->collider.y = y;
    p->collider.width = width;
    p->collider.height = height;
    p->texture = texture;
    p->movementSpeed = speed;
    p->direction = 0;
    p->damage = 0;

    return p;
}

void FreeProjectile(Projectile* projectile)
{
    MyFree((void**)&projectile);
}

void MoveProjectile(Projectile* projectile)
{
    Vector2 movement = GetPositionInDistance(projectile->transform.position, projectile->movementSpeed, projectile->direction);
    projectile->transform.position = movement;

    MoveProjectileCollider(projectile);
}

void DrawProjectilePool(Projectile** pool, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        if (pool[i] != NULL) 
        {
            DrawProjectile(pool[i]);
        }
    }
}

void DrawProjectile(Projectile* projectile)
{
    DrawTextureBySize(projectile->texture, projectile->transform.position, projectile->transform.size, projectile->transform.rotation, false, false);
}

bool AddToProjectilePool(Projectile** pool, Projectile* projectile, size_t size)
{
    bool success = false;
    for(int i = 0; i < size && !success; i++)
    {
        if(pool[i] == NULL)
        {
            pool[i] = projectile;
            success = true;
        }
    }

    return success;
}

bool RemoveFromProjectilePool(Projectile** pool, int id, size_t size)
{
    bool success = false;
    for(int i = 0; i < size && !success; i++)
    {
        if(pool[i] != NULL)
        {
            if(pool[i]->id == id) 
            {
                FreeProjectile(pool[i]);
                pool[i] = NULL;
                success = true;
            }
        }
    }

    return success;
}

void FreeProjectilePool(Projectile** pool, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        if (pool[i] != NULL)
        {
            FreeProjectile(pool[i]);
        }
    }
}

// PRIVATE FUNCTIONS

void MoveProjectileCollider(Projectile* projectile)
{
    projectile->collider.x = projectile->transform.position.x;
    projectile->collider.y = projectile->transform.position.y;
}