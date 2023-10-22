#ifndef COIN_H
#define COIN_H

#include "raylib.h"
#include "utils.h"

typedef struct _coin {
    int id;
    Transform2D transform;
    Texture2D texture;
    Rectangle collider;
} Coin;

Coin* CreateCoin(float x, float y, int width, int height, Texture2D texture);
void DrawCoin(Coin* coin);
void FreeCoin(Coin* coin);
bool AddToCoinPool(Coin** pool, Coin* coin, size_t size);
bool RemoveFromCoinPool(Coin** pool, int id, size_t size);
void DrawCoinPool(Coin** pool, size_t size);
void FreeCoinPool(Coin** pool, size_t size);

#endif