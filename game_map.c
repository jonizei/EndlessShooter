#include "include/game_map.h"
#include "include/raylib.h"
#include "include/utils.h"
#include<stdio.h>
#include<stdlib.h>

// CONSTANT VALUES
const int TEXTURE_COUNT = 5;
const int EMPTY_ZONE = 0;
const int NEUTRAL_ZONE = 1;
const int FOREST_ZONE = 2;
const int MOUNTAIN_ZONE = 3;
const int SWAMP_ZONE = 4;

// GLOBAL VARIABLES
Image emptySpaceTile;
Image grassFloorImage;
Image forestFloorImage;
Image rockFloorImage;
Image swampFloorImage;

// PRIVATE FUNCTION DECLARATIONS
Texture2D* CreateGameMapTextures(GameMap* gameMap);

// PUBLIC FUNCTIONS

void LoadGameMapResources()
{
    emptySpaceTile = LoadImage("resources/textures/tiles/empty_space_tile.png");
    grassFloorImage = LoadImage("resources/textures/tiles/grass_floor_tile.png");
    forestFloorImage = LoadImage("resources/textures/tiles/forest_floor_tile.png");
    rockFloorImage = LoadImage("resources/textures/tiles/rock_floor_tile.png");
    swampFloorImage = LoadImage("resources/textures/tiles/swamp_floor_tile.png");
}

void UnloadGameMapResources()
{
    UnloadImage(emptySpaceTile);
    UnloadImage(grassFloorImage);
    UnloadImage(forestFloorImage);
    UnloadImage(rockFloorImage);
    UnloadImage(swampFloorImage);
}

GameMap* CreateGameMap(int width, int height, float tileWidth, float tileHeight)
{
    GameMap* gameMap = malloc(sizeof(GameMap));
    gameMap->mapGrid = CreateTileGrid(0, 0, width, height, tileWidth, tileHeight);
    gameMap->mapLayout = malloc((height * width) * sizeof(int));
    gameMap->textureCount = TEXTURE_COUNT;
    gameMap->textures = CreateGameMapTextures(gameMap);
    return gameMap;
}

void GenerateGameMap(GameMap* gameMap, int* seed, size_t rows, size_t cols)
{
    int sectionWidth = gameMap->mapGrid->width / cols;
    int sectionHeight = gameMap->mapGrid->height / rows;

    int seedType = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < sectionHeight; j++)
        {
            int rowIndex = i * sectionHeight;
            for (int k = 0; k < cols; k++)
            {
                int colIndex = rowIndex + j;
                seedType = seed[i * cols + k];

                for (int m = 0; m < sectionWidth; m++)
                {
                    int index = ((colIndex * cols + k) * sectionWidth) + m;
                    gameMap->mapLayout[index] = seedType;
                }
            }
        }
    }

    /*
    int seedIndex = 0;
    int sectionType = seed[seedIndex];
    int sectionWidthCounter = 0;
    int sectionHeightCounter = 0;
    for (int i = 0; i < gameMap->mapGrid->height * gameMap->mapGrid->width; i++)
    {
        gameMap->mapLayout[i] = sectionType;

        if (sectionWidthCounter == sectionWidth - 1)
        {
            seedIndex++;
            sectionType = seed[seedIndex];
            sectionWidthCounter = 0;
        }
        else
        {
            sectionWidthCounter++;
        }
    }
    */
}

void FreeGameMap(GameMap* gameMap)
{
    FreeTileGrid(gameMap->mapGrid);
    free(gameMap->mapLayout);
    free(gameMap->textures);
    MyFree(&gameMap);
}

void DrawGameMap(GameMap* gameMap)
{
    for (int i = 0; i < gameMap->mapGrid->height; i++)
    {
        for (int j = 0; j < gameMap->mapGrid->width; j++)
        {
            int textureType = gameMap->mapLayout[i * gameMap->mapGrid->width + j];
            Vector2 texturePosition = GetTileGridTilePosition(gameMap->mapGrid, j, i);
            DrawTextureV(gameMap->textures[textureType], texturePosition, WHITE);
        }
    }
}

// PRIVATE FUNCTIONS

Texture2D* CreateGameMapTextures(GameMap* gameMap)
{
    Texture2D* textures = malloc(TEXTURE_COUNT * sizeof(Texture2D));
    textures[EMPTY_ZONE] = LoadTextureFromImage(grassFloorImage);
    textures[NEUTRAL_ZONE] = LoadTextureFromImage(grassFloorImage);
    textures[FOREST_ZONE] = LoadTextureFromImage(forestFloorImage);
    textures[MOUNTAIN_ZONE] = LoadTextureFromImage(rockFloorImage);
    textures[SWAMP_ZONE] = LoadTextureFromImage(swampFloorImage);

    return textures;
}