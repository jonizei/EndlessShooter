#include "include/raylib.h"
#include "include/player.h"
#include "include/enemy.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 680
#define FPS 60

int main() 
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PlayerMechanics");

    SetTargetFPS(FPS);

    // Create player
    Player* player = CreatePlayer(100, 100);
    RatEnemy* ratEnemy = CreateRatEnemy(300, 300);

    while(!WindowShouldClose())
    {
        MovePlayer(player);
        ShootPlayer(player);
        MoveBullets(SCREEN_WIDTH, SCREEN_HEIGHT);

        BeginDrawing();

            ClearBackground(WHITE);
            DrawPlayer(player);
            DrawEnemy(ratEnemy->parent);
            DrawBullets();

        EndDrawing();
    }

    FreeRatEnemy(ratEnemy);
    FreePlayer(player);

    return 0;
}