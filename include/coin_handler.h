#ifndef COIN_HANDLER_H
#define COIN_HANDLER_H

#include "player.h"

void LoadCoinResources();
void UnloadCoinResources();
void AddCoin(float x, float y);
void RemoveCoin(int id);
void DrawCoins();
void FreeCoins();
void CheckPlayerCollisionWithCoins(Player* player);

#endif