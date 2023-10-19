#include "include/raylib.h"
#include "include/player.h"
#include "include/enemy.h"
#include "include/enemy_handler.h"
#include "include/utils.h"
#include "include/tilegrid.h"
#include "include/game_map.h"
#include "include/ui_utils.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 680
#define FPS 60

#define MAP_COLUMN_COUNT 200
#define MAP_ROW_COUNT 200
#define MAP_TILE_WIDTH 16   
#define MAP_TILE_HEIGHT 16

void UpdateCameraPosition(Camera2D* camera, Vector2 target);

int main() 
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "EndlessShooter");

    SetTargetFPS(FPS);

    LoadUIResources();
    LoadGameMapResources();
    LoadPlayerResources();
    LoadEnemyResources();

    UILayer* uiLayer = CreateUILayer();

    // 1 = start zone
    // 2 = forest zone
    // 3 = mountain zone
    // 4 = swamp zone

    GameMap* gameMap = CreateGameMap(MAP_COLUMN_COUNT, MAP_ROW_COUNT, MAP_TILE_WIDTH, MAP_TILE_HEIGHT);

    int mapSeedBase[10][10] = {
        { 4, 4, 4, 4, 2, 2, 2, 2, 2, 2 },
        { 4, 4, 4, 4, 2, 2, 2, 2, 2, 2 },
        { 4, 4, 4, 2, 2, 2, 2, 2, 2, 2 },
        { 4, 4, 2, 2, 2, 2, 2, 2, 2, 2 },
        { 3, 2, 2, 2, 2, 1, 1, 2, 2, 2 },
        { 3, 3, 2, 2, 2, 1, 1, 2, 2, 2 },
        { 3, 3, 3, 2, 2, 2, 2, 2, 2, 3 },
        { 3, 3, 3, 3, 2, 2, 2, 2, 3, 3 },
        { 3, 3, 3, 3, 3, 3, 2, 3, 3, 3 },
        { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
    };

    int mapSeed[100];

    for (int i = 0; i < 10; i++) 
    {
        for (int j = 0; j < 10; j++)
        {   
            mapSeed[i * 10 + j] = mapSeedBase[i][j];
        }
    }

    Layer mapLayer = CreateMapLayerFromSeed(gameMap, &mapSeed, 10, 10);
    AddLayer(gameMap, &mapLayer);
    SetGameMap(gameMap);

    Stats playerBaseStats = {1, 1, 1, 1};
    Vector2 playerStartPosition = GetTileGridTilePosition(gameMap->mapGrid, 5, 5);
    Player* player = CreatePlayer(playerStartPosition.x, playerStartPosition.y, playerBaseStats);
    EnemySpawner* ratSpawner = CreateEnemySpawer(ENEMY_RAT, 3, 100, 350, 350);
    EnemySpawner* goblinSpawner = CreateEnemySpawer(ENEMY_GOBLIN, 5, 100, 200, 450);

    Camera2D camera = {0};
    camera.target = GetOrigin(player->transform);
    camera.offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 2.0f;

    SetCamera(&camera);

    while(!WindowShouldClose())
    {
        UpdateCameraPosition(&camera, GetOrigin(player->transform));
        UpdatePlayer(player);
        CheckEnemyCollision();
        UpdateEnemies();
        UpdateSpawner(ratSpawner);
        UpdateSpawner(goblinSpawner);
        UpdateUILayer(uiLayer);

        BeginDrawing();

            ClearBackground(WHITE);

            BeginMode2D(camera);

                DrawGameMap(gameMap);
                DrawPlayer(player);
                DrawEnemies();

            EndMode2D();

            DrawUILayer(uiLayer);

        EndDrawing();
    }

    FreeEnemySpawner(goblinSpawner);
    FreeEnemySpawner(ratSpawner);
    FreeEnemies();
    FreePlayer(player);
    FreeGameMap(gameMap);
    FreeUILayer(uiLayer);

    UnloadUIResources();
    UnloadGameMapResources();
    UnloadPlayerResources();
    UnloadEnemyResources();

    return 0;
}

void UpdateCameraPosition(Camera2D* camera, Vector2 target)
{
    float cameraWidth = camera->offset.x / camera->zoom;
    float cameraHeight = camera->offset.y / camera->zoom;

    if (target.x < cameraWidth)
    {
        target.x = cameraWidth;
    } 
    else if (GetGameMap()->width - target.x < cameraWidth)
    {
        target.x = GetGameMap()->width - cameraWidth;
    }

    if (target.y < cameraHeight)
    {
        target.y = cameraHeight;
    }
    else if (GetGameMap()->height - target.y < cameraHeight)
    {
        target.y = GetGameMap()->height - cameraHeight;
    }


    camera->target = target;
}