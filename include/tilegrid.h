#ifndef GRID_H
#define GRID_H

#include<stdio.h>
#include<stdlib.h>
#include "raylib.h"

typedef struct _tile_grid {
    Vector2 position;
    int width;
    int height;
    float tileWidth;
    float tileHeight;
    Rectangle** matrix;
} TileGrid;

TileGrid* CreateTileGrid(int x, int y, int width, int height, float tileWidth, float tileHeight);
void FreeTileGrid(TileGrid* grid);
void DrawTileGrid(TileGrid* grid);
Rectangle* GetTileGridTile(TileGrid* grid, float x, float y);
Vector2 GetTileGridTilePosition(TileGrid* grid, int x, int y);

#endif