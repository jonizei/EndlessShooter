#include "include/merchant.h"
#include "include/ui_utils.h"
#include<stdio.h>
#include<stdlib.h>

// CONSTANT VALUES
const int MERCHANT_HEIGHT = 16;
const int MERCHANT_WIDTH = 16;

const int STRENGTH_BASE_PRICE = 5;
const int MOVEMENT_BASE_PRICE = 5;
const int ATTACK_SPEED_BASE_PRICE = 5;
const int STAMINA_BASE_PRICE = 5;

const int STRENGTH_BASE_RAISE = 7;
const int MOVEMENT_BASE_RAISE = 3;
const int ATTACK_SPEED_BASE_RAISE = 5;
const int STAMINA_BASE_RAISE = 5;

// GLOBAL VARIABLES
Image merchantImage;
Texture2D merchantTexture;

// PRIVATE FUNCTION DECLARATIONS
MerchantItem* CreateMerchantItem(StatType type, int price, int points);
void FreeMerchantItem(MerchantItem* item);
MerchantItem** CreateMerchantItems();

// PUBLIC FUNCTIONS

void LoadMerchantResources()
{
    merchantImage = LoadImage("resources/textures/merchant_sprite.png");

    merchantTexture = LoadTextureFromImage(merchantImage);
}

void UnloadMerchantResources()
{
    UnloadImage(merchantImage);
}

Merchant* CreateMerchant(float x, float y)
{
    Merchant* merchant = malloc(sizeof(Merchant));
    merchant->transform.position.x = x;
    merchant->transform.position.y = y;
    merchant->transform.size.x = MERCHANT_WIDTH;
    merchant->transform.size.y = MERCHANT_HEIGHT;
    merchant->texture = merchantTexture;
    merchant->items = CreateMerchantItems();
    return merchant;
}

void FreeMerchantItem(MerchantItem* item)
{
    MyFree((void**)&item);
}

void DrawMerchant(Merchant* merchant)
{
    DrawTextureV(merchant->texture, merchant->transform.position, WHITE);
}

void FreeMerchant(Merchant* merchant)
{
    for (int i = 0; i < 4; i++)
    {
        // For some reason this throws error on exit
        FreeMerchantItem(merchant->items[i]);
    }
    MyFree((void**)&(merchant->items));
    MyFree((void**)&merchant);
}

void UpdateMerchantItem(Merchant* merchant, StatType statType, int points)
{
    if (merchant == NULL)
    {
        return;
    }

    switch(statType)
    {
        case STAT_STRENGTH:
            if (merchant->items[0] != NULL)
            {
                merchant->items[0]->price += STRENGTH_BASE_RAISE * points;
            }
        break;

        case STAT_MOVEMENT_SPEED:
            if (merchant->items[1] != NULL)
            {
                merchant->items[1]->price += MOVEMENT_BASE_RAISE * points;
            }
        break;

        case STAT_ATTACK_SPEED:
            if (merchant->items[2] != NULL)
            {
                merchant->items[2]->price += ATTACK_SPEED_BASE_RAISE * points;
            }
        break;

        case STAT_STAMINA:
            if (merchant->items[3] != NULL)
            {
                merchant->items[3]->price += STAMINA_BASE_RAISE * points;
            }
        break;
    }
}

bool AddToMerchantPool(Merchant** pool, Merchant* merchant, size_t size)
{
    bool success = false;
    for(int i = 0; i < size && !success; i++)
    {
        if(pool[i] == NULL)
        {
            pool[i] = merchant;
            success = true;
        }
    }

    return success;
}

bool RemoveFromMerchantPool(Merchant** pool, int id, size_t size)
{
    bool success = false;
    for(int i = 0; i < size && !success; i++)
    {
        if(pool[i] != NULL)
        {
            if(pool[i]->id == id) 
            {
                FreeMerchant(pool[i]);
                pool[i] = NULL;
                success = true;
            }
        }
    }

    return success;
}

void DrawMerchantPool(Merchant** pool, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        if (pool[i] != NULL)
        {
            DrawMerchant(pool[i]);
        }
    }
}

void FreeMerchantPool(Merchant** pool, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        if (pool[i] != NULL)
        {
            FreeMerchant(pool[i]);
        }
    }
}

// PRIVATE FUNCTIONS

MerchantItem* CreateMerchantItem(StatType type, int price, int points)
{
    MerchantItem* item = malloc(sizeof(MerchantItem));
    item->statType = type;
    item->price = price;
    item->points = points;
    return item;
}

MerchantItem** CreateMerchantItems()
{
    MerchantItem** items = malloc(4 * sizeof(MerchantItem*));
    items[0] = CreateMerchantItem(STAT_STRENGTH, STRENGTH_BASE_PRICE, 1);
    items[1] = CreateMerchantItem(STAT_MOVEMENT_SPEED, MOVEMENT_BASE_PRICE, 1);
    items[2] = CreateMerchantItem(STAT_ATTACK_SPEED, ATTACK_SPEED_BASE_PRICE, 1);
    items[3] = CreateMerchantItem(STAT_STAMINA, STAMINA_BASE_PRICE, 1);
    return items;
}