#include "include/player.h"
#include "include/raylib.h"
#include "include/projectile.h"

#define MAX_PROJECTILE 100

// KEY INPUTS
const int BUTTON_UP = KEY_W;
const int BUTTON_DOWN = KEY_S;
const int BUTTON_LEFT = KEY_A;
const int BUTTON_RIGHT = KEY_D;

// PLAYER CONSTANTS
const int HEIGHT = 50;
const int WIDTH = 50;
const double MOVEMENT_SPEED = 5.0;

// GLOBAL VARIBLES
Projectile* projectilePool[MAX_PROJECTILE];
int projectileCount = 0;
int projectileId = 1;

// PRIVATE FUNCTION DECLARATIONS
void AddToPool(Projectile* projectile);
void RemoveFromPool(int id);
void FreeBullets();

// PUBLIC FUNCTIONS

Player* CreatePlayer(int x, int y)
{
    Player* p = malloc(sizeof(Player));
    p->transform.position.x = x;
    p->transform.position.y = y;
    p->transform.size.x = WIDTH;
    p->transform.size.y = HEIGHT;
    p->movementSpeed = MOVEMENT_SPEED;

    p->weapon = CreateWeapon();

    return p;
}

void FreePlayer(Player* player)
{
    FreeBullets();
    FreeWeapon(player->weapon);
    MyFree(&player);
}

void MovePlayer(Player* player)
{
    if (IsKeyDown(BUTTON_UP))
    {
        player->transform.position.y -= player->movementSpeed;
    }
    else if (IsKeyDown(BUTTON_DOWN))
    {
        player->transform.position.y += player->movementSpeed;
    }

    if (IsKeyDown(BUTTON_LEFT))
    {
        player->transform.position.x -= player->movementSpeed;
    }
    else if (IsKeyDown(BUTTON_RIGHT))
    {
        player->transform.position.x += player->movementSpeed;
    }
}

void DrawPlayer(Player* player)
{
    DrawRectangleV(player->transform.position, player->transform.size, BLUE);
}

void ShootPlayer(Player* player)
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && projectileCount + 1 < MAX_PROJECTILE)
    {
        if (GetTime() - player->weapon->lashShot > player->weapon->attackSpeed)
        {
            Projectile* bullet = CreateProjectile(
                player->transform.position.x
                , player->transform.position.y
                , 10
                , 10
                , player->weapon->speed
                , 0
            );


            bullet->id = projectileId;
            bullet->direction = GetMouseDirection(bullet->transform.position);

            AddToPool(bullet);
            projectileId++;

            player->weapon->lashShot = GetTime();
        }
    }
}

void MoveBullets(int screenWidth, int screenHeight)
{
    for (int i = 0; i < MAX_PROJECTILE; i++)
    {
        if (projectilePool[i] != NULL) 
        {
            MoveProjectile(projectilePool[i]);

            if (!ContainsPosition(projectilePool[i]->transform.position, screenWidth, screenHeight)) 
            {
                RemoveFromPool(projectilePool[i]->id);
            }
        }
    }
}

void DrawBullets()
{
    for (int i = 0; i < MAX_PROJECTILE; i++)
    {
        if (projectilePool[i] != NULL) 
        {
            DrawProjectile(projectilePool[i]);
        }
    }
}

// PRIVATE FUNCTIONS

void AddToPool(Projectile* projectile)
{
    for(int i = 0; i < MAX_PROJECTILE; i++)
    {
        if(projectilePool[i] == NULL)
        {
            projectilePool[i] = projectile;
            projectileCount++;
            break;
        }
    }
}

void RemoveFromPool(int id)
{
    for(int i = 0; i < MAX_PROJECTILE; i++)
    {
        if(projectilePool[i] != NULL)
        {
            if(projectilePool[i]->id == id) 
            {
                FreeProjectile(projectilePool[i]);
                projectilePool[i] = NULL;
                projectileCount--;
                break;
            }
        }
    }
}

void FreeBullets()
{
    for (int i = 0; i < MAX_PROJECTILE; i++)
    {
        FreeProjectile(projectilePool[i]);
    }
}