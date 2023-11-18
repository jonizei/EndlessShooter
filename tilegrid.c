#include "include/tilegrid.h"
#include "include/raylib.h"
#include "include/utils.h"

// GLOBAL VARIABLES

// PRIVATE FUNCTION DECLARATIONS

void InitializeTileGridTiles(TileGrid* grid);
void DrawTileGridTile(Rectangle tile, bool firstRow, bool firstColumn);

// PUBLIC FUNCTIONS

TileGrid* CreateTileGrid(int x, int y, int width, int height, float tileWidth, float tileHeight)
{
    TileGrid* grid = malloc(sizeof(TileGrid));
    grid->position.x = x;
    grid->position.y = y;
    grid->width = width;
    grid->height = height;
    grid->tileWidth = tileWidth;
    grid->tileHeight = tileHeight;

    grid->matrix = malloc(grid->height * sizeof(Rectangle*));

    for (int i = 0; i < grid->height; i++)
    {
        grid->matrix[i] = malloc(grid->width * sizeof(Rectangle));
    }

    InitializeTileGridTiles(grid);

    return grid;
}

void FreeTileGrid(TileGrid* grid)
{
    MyFree((void**)&grid);
}

void DrawTileGrid(TileGrid* grid)
{
    for (int i = 0; i < grid->height; i++)
    {
        for (int j = 0; j < grid->width; j++)
        {
            DrawTileGridTile(grid->matrix[i][j], i == 0, j == 0);
        }
    }
}

Rectangle* GetTileGridTile(TileGrid* grid, float x, float y)
{
    Rectangle* foundTile = NULL;
    for (int i = 0; i < grid->height; i++)
    {
        for (int j = 0; j < grid->width; j++)
        {
            bool isInside = CheckCollisionPointRec((Vector2){ x, y }, grid->matrix[i][j]);
            if (isInside) 
            {
                foundTile = &(grid->matrix[i][j]);
                break;
            }
        }
    }

    return foundTile;
}

Rectangle GetTileGridTileByCoordinates(TileGrid* grid, int x, int y)
{
    bool xInside = x >= 0 && x < grid->width;
    bool yInside = y >= 0 && y < grid->height;

    if (xInside && yInside)
    {
        return grid->matrix[y][x];
    }

    return (Rectangle){ 0, 0, 0, 0 };
}

Vector2 GetTileGridTilePosition(TileGrid* grid, int x, int y)
{
    if (x > grid->width || y > grid->height) 
    {
        return (Vector2){ 0, 0 };
    }

    return (Vector2) {
        grid->matrix[y][x].x,
        grid->matrix[y][x].y
    };
}

// PRIVATE FUNCTIONS

void InitializeTileGridTiles(TileGrid* grid)
{
    int startX = grid->position.x;
    int x = 0;
    int y = grid->position.y;

    for (int i = 0; i < grid->height; i++)
    {
        x = startX;
        for (int j = 0; j < grid->width; j++)
        {
            Rectangle* tile = &(grid->matrix[i][j]);
            tile->x = x;
            tile->y = y;
            tile->width = grid->tileWidth;
            tile->height = grid->tileHeight;

            x += grid->tileWidth;
        }
        y += grid->tileHeight;
    }
}

void DrawTileGridTile(Rectangle tile, bool firstRow, bool firstColumn)
{
    Vector2 topLeft = {
        tile.x,
        tile.y
    };
    Vector2 topRight = {
        tile.x + tile.width,
        tile.y
    };
    Vector2 bottomLeft = {
        tile.x,
        tile.y + tile.height
    };
    Vector2 bottomRight = {
        tile.x + tile.width,
        tile.y + tile.height
    };

    DrawLineEx(topRight, bottomRight, 0.5f, BLACK);
    DrawLineEx(bottomRight, bottomLeft, 0.5f, BLACK);

    if (firstRow) 
    {
        DrawLineEx(topLeft, topRight, 0.5f, BLACK);
    }
    
    if (firstColumn) 
    {
        DrawLineEx(bottomLeft, topLeft, 0.5f, BLACK);
    }
}