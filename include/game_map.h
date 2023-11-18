#ifndef GAME_MAP_H
#define GAME_MAP_H

#define MAX_LAYERS 5

#include "tilegrid.h"

typedef struct _layer {
    int* layout;
    int width;
    int height;
} Layer;

typedef struct _game_map {
    TileGrid* mapGrid;
    Layer* layers;
    float height;
    float width;
    Texture2D* textures;
    size_t textureCount;
} GameMap;

GameMap* CreateGameMap(int width, int height, float tileWidth, float tileHeight);
Layer CreateMapLayerFromSeed(GameMap* gameMap, int* seed, size_t rows, size_t cols);
Vector2 GetRandomPositionOnArea(GameMap* gameMap, int areaType);
void FreeGameMap(GameMap* gameMap);
void DrawGameMap(GameMap* gameMap);
void AddMapLayer(GameMap* gameMap, Layer* layer);
void LoadGameMapResources();
void UnloadGameMapResources();

#endif