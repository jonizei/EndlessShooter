#include "include/player.h"
#include "include/raylib.h"
#include "include/projectile.h"
#include "include/raymath.h"

#define MAX_PROJECTILE 100

// KEY INPUTS
const int BUTTON_UP = KEY_W;
const int BUTTON_DOWN = KEY_S;
const int BUTTON_LEFT = KEY_A;
const int BUTTON_RIGHT = KEY_D;

// PLAYER CONSTANTS
const int HEIGHT = 16;
const int WIDTH = 16;
const double MOVEMENT_SPEED = 5.0;
const int BULLET_HEIGHT = 10;
const int BULLET_WIDTH = 10;

// GLOBAL VARIBLES
Projectile* projectilePool[MAX_PROJECTILE];
int projectileCount = 0;
int projectileId = 1;
Player* playerRef = NULL;
Image playerImage;
Image bulletImage;

// PRIVATE FUNCTION DECLARATIONS
void MovePlayer(Player* player);
void ShootPlayer(Player* player);
void MoveBullets();
void MoveWeapon(Player* player);
void AddToProjectilePool(Projectile* projectile);
void RemoveFromProjectilePool(int id);
void FreeBullets();
void EnemyCollisionWithBullet(Enemy* enemy, Projectile* bullet);

// PUBLIC FUNCTIONS

void LoadPlayerResources()
{
    playerImage = LoadImage("resources/textures/player_sprite.png");
    bulletImage = LoadImage("resources/textures/bullet_sprite.png");
}

void UnloadPlayerResources()
{
    UnloadImage(playerImage);
    UnloadImage(bulletImage);
}

Player* CreatePlayer(int x, int y)
{
    float playerArea = WIDTH * HEIGHT;

    Player* p = malloc(sizeof(Player));
    p->transform.position.x = x;
    p->transform.position.y = y;
    p->transform.size.x = WIDTH;
    p->transform.size.y = HEIGHT;
    p->movementSpeed = MOVEMENT_SPEED;
    p->texture = LoadTextureFromImage(playerImage);
    p->weapon = CreateWeapon();
    p->weapon->offset = WIDTH;

    float textureArea = p->texture.height * p->texture.width;
    p->transform.scale = playerArea / textureArea;

    free(playerRef);
    playerRef = p;

    return p;
}

void FreePlayer(Player* player)
{
    UnloadTexture(player->texture);
    FreeBullets();
    FreeWeapon(player->weapon);
    MyFree(&player);
}

void UpdatePlayer(Player* player)
{
    MovePlayer(player);
    ShootPlayer(player);
    MoveBullets();
    MoveWeapon(player);
}

void DrawPlayer(Player* player)
{
    DrawTextureEx(player->texture, player->transform.position, 0, player->transform.scale, WHITE);
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

bool IsEnemyCollidingWithBullet(Enemy* enemy)
{
    bool collides = false;
    for (int i = 0; i < MAX_PROJECTILE; i++)
    {
        if (projectilePool[i] != NULL)
        {
            collides = IsCollision(projectilePool[i]->transform, enemy->transform);

            if (collides) 
            {
                RemoveFromProjectilePool(projectilePool[i]->id);
                break;
            }
        }
    }

    return collides;
}

void CheckEnemyCollisionWithBullets(Enemy* enemy)
{
    for (int i = 0; i < MAX_PROJECTILE; i++)
    {
        if (projectilePool[i] != NULL)
        {
            EnemyCollisionWithBullet(enemy, projectilePool[i]);
        }
    }
}

Player* GetPlayer()
{
    return playerRef;
}

// PRIVATE FUNCTIONS

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

void ShootPlayer(Player* player)
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && projectileCount + 1 < MAX_PROJECTILE)
    {
        if (GetTime() - player->weapon->lastShot > player->weapon->attackSpeed)
        {
            Projectile* bullet = CreateProjectile(
                LoadTextureFromImage(bulletImage)
                , player->weapon->transform.position.x
                , player->weapon->transform.position.y
                , BULLET_WIDTH
                , BULLET_HEIGHT
                , player->weapon->speed
                , 0
            );

            bullet->id = projectileId;
            bullet->direction = GetMouseDirection(bullet->transform.position);
            bullet->angle = 180 + Vector2Angle(player->weapon->transform.position, GetLocalMousePosition());
            bullet->damage = 10;

            AddToProjectilePool(bullet);
            projectileId++;

            player->weapon->lastShot = GetTime();
        }
    }
}

void MoveBullets()
{
    for (int i = 0; i < MAX_PROJECTILE; i++)
    {
        if (projectilePool[i] != NULL) 
        {
            MoveProjectile(projectilePool[i]);

            if (!ContainsPosition(projectilePool[i]->transform.position, GetScreenWidth(), GetScreenHeight())) 
            {
                RemoveFromProjectilePool(projectilePool[i]->id);
            }
        }
    }
}

void MoveWeapon(Player* player)
{
    Vector2 playerOrigin = {
        player->transform.position.x + player->transform.size.x / 2,
        player->transform.position.y + player->transform.size.y / 2
    };

    float direction = GetMouseDirection(playerOrigin);
    Vector2 position = GetPositionInDistance(playerOrigin, player->weapon->offset, direction);
    player->weapon->transform.position = position;
}

void AddToProjectilePool(Projectile* projectile)
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

void RemoveFromProjectilePool(int id)
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

void EnemyCollisionWithBullet(Enemy* enemy, Projectile* bullet)
{
    bool collides = IsCollision(bullet->transform, enemy->transform);

    if (collides)
    {
        enemy->health -= bullet->damage;
        RemoveFromProjectilePool(bullet->id);
    }
}