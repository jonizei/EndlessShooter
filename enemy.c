#include "include/raylib.h"
#include "include/enemy.h"
#include "include/utils.h"
#include "include/player.h"
#include "include/raymath.h"
#include "include/coin_handler.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_ENEMY_PROJECTILE 500

// CONSTANT VALUES
const int ENEMY_BULLET_WIDTH = 10;
const int ENEMY_BULLET_HEIGHT = 10;
const float ENEMY_BULLET_SPEED = 1.5f;

// GLOBAL VARIABLES
Projectile* enemyProjectilePool[MAX_ENEMY_PROJECTILE];
int enemyProjectileCount = 0;
int enemyProjectileId = 1;
Image ratEnemyImage;
Image enemyBulletImage;
Image goblinEnemyImage;

// PRIVATE FUNCTION DECLARATIONS
Enemy* CreateEnemy(int id, int x, int y, int width, int height);
void UpdateEnemyState(Enemy* enemy);
void MoveEnemy(Enemy* enemy);
bool IsPlayerInRange(Enemy* enemy, Player* player);
bool IsPlayerInsideHitArea(Enemy* enemy, Player* player);
Vector2 GetRandomTarget(Enemy* enemy);
void MoveHitArea(Enemy* enemy);
void EnemyAttack(Enemy* enemy);
void EnemyHitAttack(Enemy* enemy);
void EnemyShootAttack(Enemy* enemy);
void MoveEnemyCollider(Enemy* enemy);
void MoveAllEnemyBullets(Enemy* enemy);
void CheckPlayerCollisionWithBullets(Player* player);
void PlayerCollisionWithBullet(Player* player, Projectile* bullet);

// PUBLIC FUNCTIONS

void LoadEnemyResources()
{
    ratEnemyImage = LoadImage("resources/textures/rat_enemy_sprite.png");
    enemyBulletImage = LoadImage("resources/textures/bullet_sprite.png");
    goblinEnemyImage = LoadImage("resources/textures/goblin_enemy_sprite.png");
}

void UnloadEnemyResources()
{
    UnloadImage(ratEnemyImage);
    UnloadImage(enemyBulletImage);
    UnloadImage(goblinEnemyImage);
}

Enemy* CreateEnemyByType(EnemyType type, int id, int x, int y)
{
    int width = 0;
    int height = 0;
    float health = 0.0f;
    float damage = 0.0f;
    float attackSpeed = 1.0f;
    EnemyMovementState defaultMovementState = ENEMY_STATE_IDLE;
    EnemyAttackType attackType = ENEMY_ATTACK_NONE;
    float sightRadius = 200;
    float wanderRadius = 150.0;
    float wanderTargetTime = 5.0;
    float movementSpeed = 1.0;
    Texture2D texture;
    float hitAreaWidth = 1.0f;
    float hitAreaHeight = 1.0f;
    int coinDropMin = 1;
    int coinDropMax = 1;

    switch(type)
    {
        case ENEMY_RAT:
            width = 16;
            height = 16;
            health = 30.0f;
            damage = 5.0f;
            attackSpeed = 1.2f;
            defaultMovementState = ENEMY_STATE_WANDER;
            attackType = ENEMY_ATTACK_HIT;
            texture = LoadTextureFromImage(ratEnemyImage);
            hitAreaHeight = 20.0f;
            hitAreaWidth = width * 1.5f;
            coinDropMin = 1;
            coinDropMax = 3;
        break;

        case ENEMY_GOBLIN:
            width = 16;
            height = 16;
            health = 50.0f;
            damage = 5.0f;
            attackSpeed = 1.5f;
            texture = LoadTextureFromImage(goblinEnemyImage);
            defaultMovementState = ENEMY_STATE_WANDER;
            attackType = ENEMY_ATTACK_SHOOT;
            coinDropMin = 2;
            coinDropMax = 4;
        break;

    }

    float enemyArea = width * height;
    float textureArea = texture.height * texture.width;

    Enemy* enemy = CreateEnemy(id, x, y, width, height);
    enemy->type = type;
    enemy->health = health;
    enemy->damage = damage;
    enemy->attackSpeed = attackSpeed;
    enemy->lastAttack = 0.0f;
    enemy->movementState = defaultMovementState;
    enemy->defaultMovementState = defaultMovementState;
    enemy->attackType = attackType;
    enemy->movementSpeed = movementSpeed;
    enemy->sightRadius = sightRadius;
    enemy->wanderRadius = wanderRadius;
    enemy->wanderTargetTime = wanderTargetTime;
    enemy->texture = texture;
    enemy->transform.scale = enemyArea / textureArea;
    enemy->coinDropMin = coinDropMin;
    enemy->coinDropMax = coinDropMax;

    Vector2 enemyOrigin = GetOrigin(enemy->transform);
    Vector2 hitAreaPosition = {
        enemyOrigin.x - hitAreaWidth / 2,
        enemyOrigin.y
    };

    enemy->hitArea = CreateBox2D(hitAreaPosition, hitAreaWidth, hitAreaHeight, degToRad(-90));

    return enemy;
}

void DrawEnemy(Enemy* enemy)
{
    DrawTextureEx(enemy->texture, enemy->transform.position, 0, enemy->transform.scale, WHITE);
    
    // Used only for debugging
    //DrawBox2D(enemy->hitArea);

    DrawProjectilePool(enemyProjectilePool, MAX_ENEMY_PROJECTILE);
}

void FreeEnemy(Enemy* enemy)
{
    FreeProjectilePool(enemyProjectilePool, MAX_ENEMY_PROJECTILE);
    MyFree(&enemy);
}

bool IsEnemyAlive(Enemy* enemy)
{
    return enemy->health > 0;
}

void UpdateEnemy(Enemy* enemy)
{
    UpdateEnemyState(enemy);
    MoveEnemy(enemy);
    MoveHitArea(enemy);
    EnemyAttack(enemy);
    MoveAllEnemyBullets(enemy);
    CheckPlayerCollisionWithBullets(GetPlayer());
}

void EnemyDie(Enemy* enemy)
{
    int coinCount = GetRandomValue(enemy->coinDropMin, enemy->coinDropMax);
    for (int i = 0; i < coinCount; i++)
    {
        float positionX = GetRandomValue(enemy->transform.position.x, enemy->transform.position.x + enemy->transform.size.x);
        float positionY = GetRandomValue(enemy->transform.position.y, enemy->transform.position.y + enemy->transform.size.y);
        AddCoin(positionX, positionY);
    }
}

// PRIVATE FUNCTIONS

Enemy* CreateEnemy(int id, int x, int y, int width, int height)
{
    Enemy* enemy = malloc(sizeof(Enemy));
    enemy->id = id;
    enemy->transform.position.x = x;
    enemy->transform.position.y = y;
    enemy->startPosition.x = x;
    enemy->startPosition.y = y;
    enemy->transform.size.x = width;
    enemy->transform.size.y = height;
    enemy->collider.x = x;
    enemy->collider.y = y;
    enemy->collider.width = width;
    enemy->collider.height = height;
    enemy->health = 0;
    enemy->lastTarget = 0;

    return enemy;
}

void UpdateEnemyState(Enemy* enemy)
{
    bool playerInRange = IsPlayerInRange(enemy, GetPlayer());
    if (playerInRange)
    {
        enemy->movementState = ENEMY_STATE_PURSUE;
    }
    else if (playerInRange == false && enemy->movementState == ENEMY_STATE_PURSUE)
    {
        enemy->movementState = enemy->defaultMovementState;
    }
}

void MoveEnemy(Enemy* enemy)
{
    if (enemy->movementState == ENEMY_STATE_IDLE)
    {
        return;
    }

    Vector2 position;

    if (enemy->movementState == ENEMY_STATE_WANDER)
    {
        if (GetTime() - enemy->lastTarget > enemy->wanderTargetTime)
        {
            enemy->wanderTarget = GetRandomTarget(enemy);
            enemy->lastTarget = GetTime();
        }
        
        position = Vector2MoveTowards(enemy->transform.position, enemy->wanderTarget, enemy->movementSpeed);
    }
    else if (enemy->movementState == ENEMY_STATE_PURSUE)
    {
        position = Vector2MoveTowards(enemy->transform.position, GetPlayer()->transform.position, enemy->movementSpeed);
    }

    enemy->transform.position.x = position.x;
    enemy->transform.position.y = position.y;

    MoveEnemyCollider(enemy);
}

bool IsPlayerInRange(Enemy* enemy, Player* player)
{
    float distance = Vector2Distance(enemy->transform.position, player->transform.position);

    if (distance <= enemy->sightRadius)
    {
        return true;
    }

    return false;
}

bool IsPlayerInsideHitArea(Enemy* enemy, Player* player) 
{
    Vector2 topLeft = { player->collider.x, player->collider.y };
    Vector2 topRight = Vector2Add(topLeft, (Vector2){ player->collider.x, 0 });
    Vector2 bottomLeft = Vector2Add(topLeft, (Vector2){ 0, player->collider.y });
    Vector2 bottomRight = Vector2Add(topLeft, (Vector2){ player->collider.x, player->collider.y });

    bool topLeftCollision = IsInsideBox2D(enemy->hitArea, topLeft);
    bool topRightCollision = IsInsideBox2D(enemy->hitArea, topRight);
    bool bottomLeftCollision = IsInsideBox2D(enemy->hitArea, bottomLeft);
    bool bottomRightCollision = IsInsideBox2D(enemy->hitArea, bottomRight);

    return topLeftCollision || topRightCollision || bottomLeftCollision || bottomRightCollision;
}

Vector2 GetRandomTarget(Enemy* enemy)
{
    int minX = enemy->startPosition.x - enemy->wanderRadius;
    int maxX = enemy->startPosition.x + enemy->wanderRadius;
    int minY = enemy->startPosition.y - enemy->wanderRadius;
    int maxY = enemy->startPosition.y + enemy->wanderRadius;

    int randX = GetRandomValue(minX, maxX);
    int randY = GetRandomValue(minY, maxY);

    return (Vector2){ randX, randY };
}

void MoveHitArea(Enemy* enemy)
{
    Vector2 enemyOrigin = GetOrigin(enemy->transform);
    Vector2 hitAreaPosition = GetPositionInDistance(enemyOrigin, enemy->hitArea.width / 2, enemy->hitArea.angle - degToRad(90));

    float direction = 0;

    if (enemy->movementState == ENEMY_STATE_WANDER)
    {
        direction = GetDirection(enemyOrigin, enemy->wanderTarget);
    } else if (enemy->movementState == ENEMY_STATE_PURSUE)
    {
        direction = GetDirection(enemyOrigin, GetOrigin(GetPlayer()->transform));
    }

    enemy->hitArea.angle = direction;
    enemy->hitArea = MoveBox2D(enemy->hitArea, hitAreaPosition);
}

void EnemyAttack(Enemy* enemy)
{
    if (enemy->movementState == ENEMY_STATE_PURSUE)
    {
        float elapsedTime = GetTime() - enemy->attackSpeed;
        bool canAttack = elapsedTime > enemy->lastAttack;
        
        if (enemy->attackType == ENEMY_ATTACK_HIT && canAttack)
        {
            EnemyHitAttack(enemy);
        }
        else if (enemy->attackType == ENEMY_ATTACK_SHOOT && canAttack)
        {
            EnemyShootAttack(enemy);
        }

    }
}

void EnemyHitAttack(Enemy* enemy)
{
    if (IsPlayerInsideHitArea(enemy, GetPlayer())) 
    {
        TakeDamage(GetPlayer(), enemy->damage);
        enemy->lastAttack = GetTime();
    }
}

void EnemyShootAttack(Enemy* enemy)
{
    if (IsPlayerInRange(enemy, GetPlayer())) 
    {
        Projectile* bullet = CreateProjectile(
            LoadTextureFromImage(enemyBulletImage)
            , enemy->transform.position.x
            , enemy->transform.position.y
            , ENEMY_BULLET_WIDTH
            , ENEMY_BULLET_HEIGHT
            , ENEMY_BULLET_SPEED
            , 0
        );

        bullet->id = enemyProjectileId;
        bullet->direction = GetDirection(enemy->transform.position, GetPlayer()->transform.position);
        bullet->angle = 180 + Vector2Angle(enemy->transform.position, GetPlayer()->transform.position);
        bullet->damage = 10;

        bool success = AddToProjectilePool(enemyProjectilePool, bullet, MAX_ENEMY_PROJECTILE);

        if (success) 
        {
            enemyProjectileId++;
            enemyProjectileCount++;
            enemy->lastAttack = GetTime();
        }
    }
}

void MoveEnemyCollider(Enemy* enemy)
{
    enemy->collider.x = enemy->transform.position.x;
    enemy->collider.y = enemy->transform.position.y;
}

void MoveAllEnemyBullets(Enemy* enemy)
{
    for (int i = 0; i < MAX_ENEMY_PROJECTILE; i++)
    {
        if (enemyProjectilePool[i] != NULL) 
        {
            MoveProjectile(enemyProjectilePool[i]);

            float distance = Vector2Distance(enemy->transform.position, enemyProjectilePool[i]->transform.position);
            if (distance > GetScreenWidth()) 
            {
                bool success = RemoveFromProjectilePool(enemyProjectilePool, enemyProjectilePool[i]->id, MAX_ENEMY_PROJECTILE);
                if (success)
                {
                    enemyProjectileCount--;
                }
            }
        }
    }
}

void CheckPlayerCollisionWithBullets(Player* player)
{
    for (int i = 0; i < MAX_ENEMY_PROJECTILE; i++)
    {
        if (enemyProjectilePool[i] != NULL)
        {
            PlayerCollisionWithBullet(player, enemyProjectilePool[i]);
        }
    }
}

void PlayerCollisionWithBullet(Player* player, Projectile* bullet)
{
    bool collides = CheckCollisionRecs(bullet->collider, player->collider);

    if (collides)
    {
        TakeDamage(player, bullet->damage);
        bool success = RemoveFromProjectilePool(enemyProjectilePool, bullet->id, MAX_ENEMY_PROJECTILE);
        if (success)
        {
            enemyProjectileCount--;
        }
    }
}