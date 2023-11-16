#ifndef MERCHANT_H
#define MERCHANT_H

#include "utils.h"
#include "raylib.h"

typedef struct _merchant_item {
    StatType statType;
    int price;
    int points;
} MerchantItem;

typedef struct _merchant {
    int id;
    Transform2D transform;
    Texture2D texture;
    MerchantItem** items;
} Merchant;

void LoadMerchantResources();
void UnloadMerchantResources();
Merchant* CreateMerchant(float x, float y);
void DrawMerchant(Merchant* merchant);
void FreeMerchant(Merchant* merchant);
void UpdateMerchantItem(Merchant* merchant, StatType statType, int points);
bool AddToMerchantPool(Merchant** pool, Merchant* merchant, size_t size);
bool RemoveFromMerchantPool(Merchant** pool, int id, size_t size);
void DrawMerchantPool(Merchant** pool, size_t size);
void FreeMerchantPool(Merchant** pool, size_t size);

#endif