#include "include/raylib.h"
#include "include/player.h"
#include "include/enemy.h"
#include "include/enemy_handler.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 680
#define FPS 60

int main() 
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "EndlessShooter");

    SetTargetFPS(FPS);

    // Create player
    Player* player = CreatePlayer(100, 100);
    EnemySpawner* ratSpawner = CreateEnemySpawer(ENEMY_RAT, 3, 100, 350, 350);

    while(!WindowShouldClose())
    {
        MovePlayer(player);
        ShootPlayer(player);
        MoveBullets(SCREEN_WIDTH, SCREEN_HEIGHT);
        CheckEnemyCollision();
        UpdateEnemies();
        UpdateSpawner(ratSpawner);

        BeginDrawing();

            ClearBackground(WHITE);
            DrawPlayer(player);
            DrawEnemies();
            DrawBullets();

        EndDrawing();
    }

    FreeEnemySpawner(ratSpawner);
    FreeEnemies();
    FreePlayer(player);

    return 0;
}