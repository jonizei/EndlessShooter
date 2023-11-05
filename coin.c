#include "include/coin.h"
#include<stdio.h>
#include<stdlib.h>

// CONSTANT VALUES

// GLOBAL VARIABLES

// PRIVATE FUNCTION DECLARATIONS

// PUBLIC FUNCTIONS

Coin* CreateCoin(float x, float y, int width, int height, Texture2D texture)
{
    float coinArea = width * height;
    float textureArea = texture.height * texture.width;

    Coin* coin = malloc(sizeof(Coin));
    coin->transform.position.x = x;
    coin->transform.position.y = y;
    coin->transform.size.x = width;
    coin->transform.size.y = height;
    coin->collider.x = x;
    coin->collider.y = y;
    coin->collider.width = width;
    coin->collider.height = height;
    coin->texture = texture;
    coin->transform.scale = coinArea / textureArea;

    return coin;
}

void DrawCoin(Coin* coin)
{
    DrawTextureEx(coin->texture, coin->transform.position, 0, coin->transform.scale, WHITE);
}

void FreeCoin(Coin* coin)
{
    MyFree((void**)&coin);
}

bool AddToCoinPool(Coin** pool, Coin* coin, size_t size)
{
    bool success = false;
    for(int i = 0; i < size && !success; i++)
    {
        if(pool[i] == NULL)
        {
            pool[i] = coin;
            success = true;
        }
    }

    return success;
}

bool RemoveFromCoinPool(Coin** pool, int id, size_t size)
{
    bool success = false;
    for(int i = 0; i < size && !success; i++)
    {
        if(pool[i] != NULL)
        {
            if(pool[i]->id == id) 
            {
                FreeCoin(pool[i]);
                pool[i] = NULL;
                success = true;
            }
        }
    }

    return success;
}

void DrawCoinPool(Coin** pool, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        if (pool[i] != NULL)
        {
            DrawCoin(pool[i]);
        }
    }
}

void FreeCoinPool(Coin** pool, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        if (pool[i] != NULL)
        {
            FreeCoin(pool[i]);
        }
    }
}

// PRIVATE FUNCTIONS