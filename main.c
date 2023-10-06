#include "include/raylib.h"
#include "include/player.h"
#include "include/enemy.h"
#include "include/enemy_handler.h"
#include "include/utils.h"

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
    EnemySpawner* goblinSpawner = CreateEnemySpawer(ENEMY_GOBLIN, 5, 100, 200, 450);

    Camera2D camera = {0};
    camera.target = (Vector2){ player->transform.position.x + 10.0f, player->transform.position.y + 10.0f };
    camera.offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 2.0f;

    SetCamera(&camera);

    while(!WindowShouldClose())
    {
        camera.target = (Vector2){ player->transform.position.x + 20.0f, player->transform.position.y + 20.0f };
        UpdatePlayer(player);
        CheckEnemyCollision();
        UpdateEnemies();
        UpdateSpawner(ratSpawner);
        UpdateSpawner(goblinSpawner);

        BeginDrawing();

            ClearBackground(WHITE);

            BeginMode2D(camera);

                DrawPlayer(player);
                DrawEnemies();

            EndMode2D();

        EndDrawing();
    }

    FreeEnemySpawner(goblinSpawner);
    FreeEnemySpawner(ratSpawner);
    FreeEnemies();
    FreePlayer(player);

    UnloadPlayerResources();
    UnloadEnemyResources();

    return 0;
}