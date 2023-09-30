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

    LoadPlayerResources();
    LoadEnemyResources();

    // Create player
    Player* player = CreatePlayer(100, 100);
    EnemySpawner* ratSpawner = CreateEnemySpawer(ENEMY_RAT, 3, 100, 350, 350);

    Camera2D camera = {0};
    camera.target = (Vector2){ player->transform.position.x + 10.0f, player->transform.position.y + 10.0f };
    camera.offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 2.0f;

    while(!WindowShouldClose())
    {
        camera.target = (Vector2){ player->transform.position.x + 20.0f, player->transform.position.y + 20.0f };
        MovePlayer(player);
        ShootPlayer(player);
        MoveBullets(SCREEN_WIDTH, SCREEN_HEIGHT);
        CheckEnemyCollision();
        UpdateEnemies();
        UpdateSpawner(ratSpawner);

        BeginDrawing();

            ClearBackground(WHITE);

            BeginMode2D(camera);

                DrawPlayer(player);
                DrawEnemies();
                DrawBullets();

            EndMode2D();

        EndDrawing();
    }

    FreeEnemySpawner(ratSpawner);
    FreeEnemies();
    FreePlayer(player);

    UnloadPlayerResources();
    UnloadEnemyResources();

    return 0;
}