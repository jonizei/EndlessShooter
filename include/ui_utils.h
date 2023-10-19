#ifndef UI_UTILS_H
#define UI_UTILS_H

typedef struct _ui_layer UILayer;

void LoadUIResources();
void UnloadUIResources();
UILayer* CreateUILayer();
void FreeUILayer(UILayer* uiLayer);
void DrawUILayer(UILayer* uiLayer);
void UpdateUILayer(UILayer* uiLayer);

#endif