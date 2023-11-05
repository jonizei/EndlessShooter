#ifndef UI_UTILS_H
#define UI_UTILS_H

#include "merchant.h"

typedef struct _ui_layer UILayer;

void LoadUIResources();
void UnloadUIResources();
bool IsUIOpen();
UILayer* CreateUILayer();
void SetMainUILayer();
UILayer* GetMainUILayer();
void FreeUILayer(UILayer* uiLayer);
void DrawUILayer(UILayer* uiLayer);
void UpdateUILayer(UILayer* uiLayer);
void OpenMerchantUI(UILayer* uiLayer, Merchant* merchant);
void CloseMerchantUI(UILayer* uiLayer);

#endif