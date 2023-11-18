#ifndef MERCHANT_HANDLER_H
#define MERCHANT_HANDLER_H

#include "merchant.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

void AddMerchant(float x, float y);
void RemoveMerchant(int id);
void DrawMerchants();
void FreeMerchants();
Merchant* FindMerchantInRadius(Vector2 position, float radius);
void InteractMerchant(Merchant* merchant);
void UpdateMerchantItemOnAllMerchants(StatType statType, int points);

#endif