#include "include/coin_handler.h"
#include "include/coin.h"
#include<stdio.h>
#include<stdlib.h>

#define MAX_COINS 1000

// CONSTANT VALUES
const int COIN_WIDTH = 10;
const int COIN_HEIGHT = 10;

// GLOBAL VARIABLES
Coin* coinPool[MAX_COINS];
int coinId = 1;
int coinCount = 0;
Image coinImage;

// PRIVATE FUNCTION DECLARATIONS

// PUBLIC FUNCTIONS

void LoadCoinResources()
{
    coinImage = LoadImage("resources/textures/coin_sprite.png");
}

void UnloadCoinResources()
{
    UnloadImage(coinImage);
}

void AddCoin(float x, float y)
{
    Coin* coin = CreateCoin(x, y, COIN_WIDTH, COIN_HEIGHT, LoadTextureFromImage(coinImage));
    bool success = AddToCoinPool(coinPool, coin, MAX_COINS);
    if (success)
    {
        coin->id = coinId;
        coinId++;
        coinCount++;
    }
}

void RemoveCoin(int id)
{
    bool success = RemoveFromCoinPool(coinPool, id, MAX_COINS);
    if (success)
    {
        coinCount--;
    }
}

void DrawCoins()
{
    DrawCoinPool(coinPool, MAX_COINS);
}

void FreeCoins()
{
    FreeCoinPool(coinPool, MAX_COINS);
}

void CheckPlayerCollisionWithCoins(Player* player)
{
    for (int i = 0; i < MAX_COINS; i++)
    {
        Coin* coin = coinPool[i];
        if (coin != NULL) 
        {
            bool collides = CheckCollisionRecs(coin->collider, player->collider);

            if (collides)
            {
                RemoveCoin(coin->id);
                PlayerEarnCoins(player, 1);
            }
        }
    }
}

// PRIVATE FUNCTIONS