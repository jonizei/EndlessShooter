#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "tilegrid.h"

typedef struct _game_map {
    TileGrid* mapGrid;
    int* mapLayout;
    float height;
    float width;
    Texture2D* textures;
    size_t textureCount;
} GameMap;

GameMap* CreateGameMap(int width, int height, float tileWidth, float tileHeight);
void GenerateGameMap(GameMap* gameMap, int* seed, size_t rows, size_t cols);
void FreeGameMap(GameMap* gameMap);
void DrawGameMap(GameMap* gameMap);
void LoadGameMapResources();
void UnloadGameMapResources();

#endif