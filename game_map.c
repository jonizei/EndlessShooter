#include "include/game_map.h"
#include "include/raylib.h"
#include "include/utils.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TEXTURE_COUNT 5
#define EMPTY_ZONE 0
#define NEUTRAL_ZONE 1
#define FOREST_ZONE 2
#define MOUNTAIN_ZONE 3
#define SWAMP_ZONE 4

// CONSTANT VALUES

// GLOBAL VARIABLES
Image emptySpaceTile;
Image grassFloorImage;
Image forestFloorImage;
Image rockFloorImage;
Image swampFloorImage;

int layerCount = 0;

// PRIVATE FUNCTION DECLARATIONS
Texture2D* CreateGameMapTextures(GameMap* gameMap);
Layer FlattenLayers(GameMap* gameMap);
void MergeLayers(Layer* a, Layer* b);
Texture2D GetTextureByType(GameMap* gameMap, int type);

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
    gameMap->height = height * tileHeight;
    gameMap->width = width * tileWidth;
    gameMap->layers = malloc(MAX_LAYERS * sizeof(Layer));
    memset(gameMap->layers, NULL, MAX_LAYERS * sizeof(Layer));
    gameMap->textureCount = TEXTURE_COUNT;
    gameMap->textures = CreateGameMapTextures(gameMap);

    return gameMap;
}

Layer CreateMapLayerFromSeed(GameMap* gameMap, int* seed, size_t rows, size_t cols)
{
    Layer layer;
    layer.height = gameMap->mapGrid->height;
    layer.width = gameMap->mapGrid->width;
    layer.layout = malloc(layer.height * layer.width * sizeof(int));
    memset(layer.layout, 0, layer.height * layer.width * sizeof(int));

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
                    layer.layout[index] = seedType;
                }
            }
        }
    }

    return layer;
}

void FreeGameMap(GameMap* gameMap)
{
    FreeTileGrid(gameMap->mapGrid);
    free(gameMap->layers);
    free(gameMap->textures);
    MyFree(&gameMap);
}

void DrawGameMap(GameMap* gameMap)
{
    Layer flattenLayer = FlattenLayers(gameMap);
    for (int i = 0; i < gameMap->mapGrid->height; i++)
    {
        for (int j = 0; j < gameMap->mapGrid->width; j++)
        {
            int textureType = flattenLayer.layout[i * gameMap->mapGrid->width + j];
            Vector2 texturePosition = GetTileGridTilePosition(gameMap->mapGrid, j, i);
            Texture2D tileTexture = GetTextureByType(gameMap, textureType);
            DrawTextureV(tileTexture, texturePosition, WHITE);
        }
    }
}

void AddLayer(GameMap* gameMap, Layer* layer)
{
    bool hasSameHeight = gameMap->mapGrid->height == layer->height;
    bool hasSameWidth = gameMap->mapGrid->width == layer->width;

    if (layerCount < MAX_LAYERS && (hasSameHeight && hasSameWidth)) 
    {
        gameMap->layers[layerCount] = *layer;
        layerCount++;
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

Layer FlattenLayers(GameMap* gameMap)
{
    Layer flattenLayer;
    flattenLayer.height = gameMap->mapGrid->height;
    flattenLayer.width = gameMap->mapGrid->width;
    flattenLayer.layout = malloc(flattenLayer.height * flattenLayer.width * sizeof(int));
    memset(flattenLayer.layout, 0, flattenLayer.height * flattenLayer.width * sizeof(int));

    for (int i = 0; i < layerCount; i++) 
    {
        Layer layer = gameMap->layers[i];
        MergeLayers(&flattenLayer, &layer);
    }

    return flattenLayer;
}

void MergeLayers(Layer* a, Layer* b)
{
    for (int i = 0; i < a->height; i++)
    {
        for (int j = 0; j < a->width; j++)
        {
            int index = i * a->width + j;
            int value = b->layout[index];
            if (value > 0) 
            {
                a->layout[index] = value;
            }
        }
    }
}

Texture2D GetTextureByType(GameMap* gameMap, int type)
{
    Texture2D found;

    switch (type)
    {
        case NEUTRAL_ZONE:
            found = gameMap->textures[NEUTRAL_ZONE];
        break;

        case FOREST_ZONE:
            found = gameMap->textures[FOREST_ZONE];
            break;

        case MOUNTAIN_ZONE:
            found = gameMap->textures[MOUNTAIN_ZONE];
            break;

        case SWAMP_ZONE:
            found = gameMap->textures[SWAMP_ZONE];
            break;

        default:
            found = gameMap->textures[EMPTY_ZONE];
        break;
    }

    return found;
}