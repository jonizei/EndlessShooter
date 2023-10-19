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
const float BASE_MOVEMENT_SPEED = 1.0f;
const int BULLET_HEIGHT = 10;
const int BULLET_WIDTH = 10;
const float BASE_DAMAGE = 4.0f;
const float BASE_HEALTH = 100.0f;
const float BASE_BULLET_SPEED = 5.0f;
const float BASE_ATTACK_SPEED = 0.1f;

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
void MoveAllBullets(Player* player);
void MoveWeapon(Player* player);
void EnemyCollisionWithBullet(Enemy* enemy, Projectile* bullet);
void MoveCollider(Player* player);
bool CanPlayerMove(Player* player, Vector2 position);

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

Player* CreatePlayer(float x, float y, Stats stats)
{
    float playerArea = WIDTH * HEIGHT;

    Player* p = malloc(sizeof(Player));
    p->transform.position.x = x;
    p->transform.position.y = y;
    p->transform.size.x = WIDTH;
    p->transform.size.y = HEIGHT;
    p->collider.x = x;
    p->collider.y = y;
    p->collider.width = WIDTH;
    p->collider.height = HEIGHT;
    p->stats = stats;
    p->texture = LoadTextureFromImage(playerImage);

    p->health = BASE_HEALTH + GetStatValue(stats, STAT_STAMINA);
    p->movementSpeed = BASE_MOVEMENT_SPEED + GetStatValue(stats, STAT_MOVEMENT_SPEED);
    p->attackSpeed = BASE_ATTACK_SPEED + GetStatValue(stats, STAT_ATTACK_SPEED);
    p->damage = BASE_DAMAGE + GetStatValue(stats, STAT_STRENGTH);

    p->weapon = CreateWeapon(BASE_BULLET_SPEED, p->attackSpeed);
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
    FreeProjectilePool(projectilePool, MAX_PROJECTILE);
    FreeWeapon(player->weapon);
    MyFree(&player);
}

void UpdatePlayer(Player* player)
{
    MovePlayer(player);
    ShootPlayer(player);
    MoveAllBullets(player);
    MoveWeapon(player);
}

void DrawPlayer(Player* player)
{
    DrawTextureEx(player->texture, player->transform.position, 0, player->transform.scale, WHITE);

    DrawProjectilePool(projectilePool, MAX_PROJECTILE);
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

void TakeDamage(Player* player, float damage)
{
    player->health -= damage;
}

void UpdatePlayerStats(Player* player)
{
    player->health = BASE_HEALTH + GetStatValue(player->stats, STAT_STAMINA);
    player->movementSpeed = BASE_MOVEMENT_SPEED + GetStatValue(player->stats, STAT_MOVEMENT_SPEED);
    player->attackSpeed = BASE_ATTACK_SPEED + GetStatValue(player->stats, STAT_ATTACK_SPEED);
    player->damage = BASE_DAMAGE + GetStatValue(player->stats, STAT_STRENGTH);
}

// PRIVATE FUNCTIONS

void MovePlayer(Player* player)
{
    Vector2 nextPosition = { player->transform.position.x, player->transform.position.y };

    if (IsKeyDown(BUTTON_UP))
    {
        nextPosition.y -= player->movementSpeed;
    }
    else if (IsKeyDown(BUTTON_DOWN))
    {
        nextPosition.y += player->movementSpeed;
    }

    if (IsKeyDown(BUTTON_LEFT))
    {
        nextPosition.x -= player->movementSpeed;
    }
    else if (IsKeyDown(BUTTON_RIGHT))
    {
        nextPosition.x += player->movementSpeed;
    }

    if (CanPlayerMove(player, nextPosition))
    {
        player->transform.position = nextPosition;
    }
    else if (CanPlayerMove(player, (Vector2){ nextPosition.x, player->transform.position.y }))
    {
        nextPosition.y = player->transform.position.y;
        player->transform.position = nextPosition;
    }
    else if (CanPlayerMove(player, (Vector2){ player->transform.position.x, nextPosition.y }))
    {
        nextPosition.x = player->transform.position.x;
        player->transform.position = nextPosition;
    }

    MoveCollider(player);
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
            bullet->angle = 180 + Vector2Angle(player->weapon->transform.position, GetMouseWorldPosition());
            bullet->damage = player->damage;

            bool success = AddToProjectilePool(projectilePool, bullet, MAX_PROJECTILE);

            if (success)
            {
                projectileId++;
                projectileCount++;
                player->weapon->lastShot = GetTime();
            }
        }
    }
}

void MoveAllBullets(Player* player)
{
    for (int i = 0; i < MAX_PROJECTILE; i++)
    {
        if (projectilePool[i] != NULL) 
        {
            MoveProjectile(projectilePool[i]);

            float distance = Vector2Distance(player->transform.position, projectilePool[i]->transform.position);
            if (distance > GetScreenWidth()) 
            {
                bool success = RemoveFromProjectilePool(projectilePool, projectilePool[i]->id, MAX_PROJECTILE);
                if (success)
                {
                    projectileCount--;
                }
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

void MoveCollider(Player* player)
{
    player->collider.x = player->transform.position.x;
    player->collider.y = player->transform.position.y;
}

void EnemyCollisionWithBullet(Enemy* enemy, Projectile* bullet)
{
    bool collides = CheckCollisionRecs(bullet->collider, enemy->collider);

    if (collides)
    {
        enemy->health -= bullet->damage;
        bool success = RemoveFromProjectilePool(projectilePool, bullet->id, MAX_PROJECTILE);
        if (success)
        {
            projectileCount--;
        }
    }
}

bool CanPlayerMove(Player* player, Vector2 position)
{
    bool xInside = position.x > 0 && position.x + player->transform.size.x < GetGameMap()->width;
    bool yInside = position.y > 0 && position.y + player->transform.size.y < GetGameMap()->height;

    return xInside && yInside;
}