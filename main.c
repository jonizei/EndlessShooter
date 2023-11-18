#include "include/raylib.h"
#include "include/player.h"
#include "include/enemy.h"
#include "include/enemy_handler.h"
#include "include/utils.h"
#include "include/tilegrid.h"
#include "include/game_map.h"
#include "include/ui_utils.h"
#include "include/coin_handler.h"
#include "include/merchant_handler.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 680
#define FPS 60

#define MAP_COLUMN_COUNT 150
#define MAP_ROW_COUNT 150
#define MAP_TILE_WIDTH 16   
#define MAP_TILE_HEIGHT 16

// 1 = start zone
// 2 = forest zone
// 3 = mountain zone
// 4 = swamp zone
int defaultMapSeed[10][10] = {
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

void UpdateCameraPosition(Camera2D* camera, Vector2 target);

int main() 
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "EndlessShooter");

    SetTargetFPS(FPS);

    LoadUIResources();
    LoadGameMapResources();
    LoadPlayerResources();
    LoadEnemyResources();
    LoadCoinResources();
    LoadMerchantResources();

    UILayer* uiLayer = CreateUILayer();
    SetMainUILayer(uiLayer);

    int mapSeed[100];

    for (int i = 0; i < 10; i++) 
    {
        for (int j = 0; j < 10; j++)
        {   
            mapSeed[i * 10 + j] = defaultMapSeed[i][j];
        }
    }

    GameMap* gameMap = CreateGameMap(MAP_COLUMN_COUNT, MAP_ROW_COUNT, MAP_TILE_WIDTH, MAP_TILE_HEIGHT);
    Layer mapLayer = CreateMapLayerFromSeed(gameMap, (int*)&mapSeed, 10, 10);
    AddMapLayer(gameMap, &mapLayer);
    SetGameMap(gameMap);

    Stats playerBaseStats = {0, 0, 0, 0};
    Vector2 playerStartPosition = GetRandomPositionOnArea(gameMap, 1);
    Player* player = CreatePlayer(playerStartPosition.x, playerStartPosition.y, playerBaseStats);

    Vector2 ratSpawnerPosition = GetRandomPositionOnArea(gameMap, 4);
    EnemySpawner* ratSpawner1 = CreateEnemySpawer(ENEMY_RAT, 30, 200, ratSpawnerPosition.x, ratSpawnerPosition.y);
    ratSpawnerPosition = GetRandomPositionOnArea(gameMap, 2);
    EnemySpawner* ratSpawner2 = CreateEnemySpawer(ENEMY_RAT, 30, 200, ratSpawnerPosition.x, ratSpawnerPosition.y);
    ratSpawnerPosition = GetRandomPositionOnArea(gameMap, 2);
    EnemySpawner* ratSpawner3 = CreateEnemySpawer(ENEMY_RAT, 30, 200, ratSpawnerPosition.x, ratSpawnerPosition.y);

    ratSpawnerPosition = GetRandomPositionOnArea(gameMap, 4);
    EnemySpawner* giantRatSpawner = CreateEnemySpawer(ENEMY_GIANT_RAT, 180, 200, ratSpawnerPosition.x, ratSpawnerPosition.y);

    Vector2 goblinSpawnerPosition = GetRandomPositionOnArea(gameMap, 2);
    EnemySpawner* goblinSpawner1 = CreateEnemySpawer(ENEMY_GOBLIN, 30, 200, goblinSpawnerPosition.x, goblinSpawnerPosition.y);
    goblinSpawnerPosition = GetRandomPositionOnArea(gameMap, 2);
    EnemySpawner* goblinSpawner2 = CreateEnemySpawer(ENEMY_GOBLIN, 30, 200, goblinSpawnerPosition.x, goblinSpawnerPosition.y);

    goblinSpawnerPosition = GetRandomPositionOnArea(gameMap, 3);
    EnemySpawner* eliteGoblinSpawner = CreateEnemySpawer(ENEMY_ELITE_GOBLIN, 50, 200, goblinSpawnerPosition.x, goblinSpawnerPosition.y);
    goblinSpawnerPosition = GetRandomPositionOnArea(gameMap, 3);
    EnemySpawner* eliteGoblinSpawner2 = CreateEnemySpawer(ENEMY_ELITE_GOBLIN, 50, 200, goblinSpawnerPosition.x, goblinSpawnerPosition.y);

    Vector2 merchantPosition = GetRandomPositionOnArea(gameMap, 1);
    AddMerchant(merchantPosition.x, merchantPosition.y);
    merchantPosition = GetRandomPositionOnArea(gameMap, 2);
    AddMerchant(merchantPosition.x, merchantPosition.y);
    merchantPosition = GetRandomPositionOnArea(gameMap, 3);
    AddMerchant(merchantPosition.x, merchantPosition.y);
    merchantPosition = GetRandomPositionOnArea(gameMap, 4);
    AddMerchant(merchantPosition.x, merchantPosition.y);

    Camera2D camera = {0};
    camera.target = GetOrigin(player->transform);
    camera.offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 2.0f;

    SetCamera(&camera);

    while(!WindowShouldClose())
    {
        UpdateCameraPosition(&camera, GetOrigin(player->transform));
        UpdateUILayer(uiLayer);

        // Pause game if UI is open
        if (!IsUIOpen())
        {
            UpdatePlayer(player);
            CheckEnemyCollision();
            CheckPlayerCollisionWithCoins(player);
            UpdateEnemies();
            UpdateSpawner(ratSpawner1);
            UpdateSpawner(ratSpawner2);
            UpdateSpawner(ratSpawner3);
            UpdateSpawner(giantRatSpawner);
            UpdateSpawner(goblinSpawner1);
            UpdateSpawner(goblinSpawner2);
            UpdateSpawner(eliteGoblinSpawner);
            UpdateSpawner(eliteGoblinSpawner2);
        }

        BeginDrawing();

            ClearBackground(WHITE);

            BeginMode2D(camera);

                DrawGameMap(gameMap);
                DrawCoins();
                DrawEnemies();
                DrawMerchants();
                DrawPlayer(player);

            EndMode2D();

            DrawUILayer(uiLayer);

        EndDrawing();
    }

    FreeEnemySpawner(ratSpawner1);
    FreeEnemySpawner(ratSpawner2);
    FreeEnemySpawner(ratSpawner3);
    FreeEnemySpawner(giantRatSpawner);
    FreeEnemySpawner(goblinSpawner1);
    FreeEnemySpawner(goblinSpawner2);
    FreeEnemySpawner(eliteGoblinSpawner);
    FreeEnemySpawner(eliteGoblinSpawner2);

    FreeEnemies();
    FreePlayer(player);
    FreeGameMap(gameMap);
    FreeUILayer(uiLayer);
    FreeCoins();
    FreeMerchants();

    UnloadUIResources();
    UnloadGameMapResources();
    UnloadPlayerResources();
    UnloadEnemyResources();
    UnloadCoinResources();
    UnloadMerchantResources();

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