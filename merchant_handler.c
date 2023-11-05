#include "include/merchant_handler.h"
#include "include/merchant.h"
#include "include/raylib.h"
#include "include/raymath.h"
#include "include/ui_utils.h"

#define MAX_MERCHANT 4

// CONSTANT VALUES

// GLOBAL VARIABLES
Merchant* merchantPool[MAX_MERCHANT];
int merchantId = 1;
int merchantCount = 0;

// PRIVATE FUNCTION DECLARATIONS

// PUBLIC FUNCTIONS

void AddMerchant(float x, float y)
{
    Merchant* merchant = CreateMerchant(x, y);
    bool success = AddToMerchantPool(merchantPool, merchant, MAX_MERCHANT);
    if (success)
    {
        merchant->id = merchantId;
        merchantId++;
        merchantCount++;
    }
}

void RemoveMerchant(int id)
{
    bool success = RemoveFromMerchantPool(merchantPool, id, MAX_MERCHANT);
    if (success)
    {
        merchantCount--;
    }
}

void DrawMerchants()
{
    DrawMerchantPool(merchantPool, MAX_MERCHANT);
}

void FreeMerchants()
{
    FreeMerchantPool(merchantPool, MAX_MERCHANT);
}

Merchant* FindMerchantInRadius(Vector2 position, float radius)
{
    Merchant* found = NULL;
    for (int i = 0; i < MAX_MERCHANT; i++)
    {
        if (merchantPool[i] != NULL)
        {
            float distance = Vector2Distance(position, merchantPool[i]->transform.position);
            if (distance <= radius)
            {
                found = merchantPool[i];
            }
        }
    }

    return found;
}

void InteractMerchant(Merchant* merchant)
{
    OpenMerchantUI(GetMainUILayer(), merchant);
}

// PRIVATE FUNCTIONS