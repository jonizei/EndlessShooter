#include "include/ui_utils.h"
#include "include/raylib.h"
#include "include/utils.h"
#include "include/player.h"
#include "include/raymath.h"
#include<stdio.h>
#include<stdlib.h>

// CONSTANT VALUES
typedef struct _ui_healthbar {
    Transform2D transform;
    Texture2D background;
    Texture2D fill;
    Texture2D border;
    float fillValue;
} UIHealthbar;

typedef struct _ui_coins {
    Transform2D transform;
    Texture2D icon;
    int count;
} UICoins;

typedef struct _player_ui {
    UIHealthbar* healthbar;
    UICoins* coins;
} PlayerUI;

struct _ui_layer {
    PlayerUI* playerUi;
};

// GLOBAL VARIABLES
Image healthbarBackground;
Image healthbarFill;
Image healthbarBorder;
Image coinUIImage;

// PRIVATE FUNCTION DECLARATIONS
UIHealthbar* CreateUIHealthbar(float x, float y);
void DrawUIHealthbar(UIHealthbar* healthbar);
void DrawUIHealthbarFill(UIHealthbar* healthbar);
UICoins* CreateUICoins(float x, float y);
void DrawUICoins(UICoins* uiCoins);
PlayerUI* CreatePlayerUI();
void DrawPlayerUI(PlayerUI* playerUi);
void FreePlayerUI(PlayerUI* playerUi);
void UpdatePlayerUI(PlayerUI* playerUi);

// PUBLIC FUNCTIONS
void LoadUIResources()
{
    healthbarBackground = LoadImage("resources/textures/ui/healthbar_background_sprite.png");
    healthbarFill = LoadImage("resources/textures/ui/healthbar_fill_sprite.png");
    healthbarBorder = LoadImage("resources/textures/ui/healthbar_border_sprite.png");
    coinUIImage = LoadImage("resources/textures/ui/coin_sprite.png");
}

void UnloadUIResources()
{
    UnloadImage(healthbarBackground);
    UnloadImage(healthbarFill);
    UnloadImage(healthbarBorder);
    UnloadImage(coinUIImage);
}

UILayer* CreateUILayer()
{
    UILayer* uiLayer = (UILayer*)malloc(sizeof(UILayer));
    uiLayer->playerUi = CreatePlayerUI();
    return uiLayer;
}

void UpdateUILayer(UILayer* uiLayer)
{
    UpdatePlayerUI(uiLayer->playerUi);
}

void DrawUILayer(UILayer* uiLayer)
{
    DrawPlayerUI(uiLayer->playerUi);
}

void FreeUILayer(UILayer* uiLayer)
{
    FreePlayerUI(uiLayer->playerUi);
    MyFree((void**)(&uiLayer));
}

// PRIVATE FUNCTIONS
UIHealthbar* CreateUIHealthbar(float x, float y)
{
    UIHealthbar* healthbar = (UIHealthbar*)malloc(sizeof(UIHealthbar));
    healthbar->transform.position.x = x;
    healthbar->transform.position.y = y;
    healthbar->fillValue = 1.0f;
    
    // Textures
    healthbar->background = LoadTextureFromImage(healthbarBackground);
    healthbar->fill = LoadTextureFromImage(healthbarFill);
    healthbar->border = LoadTextureFromImage(healthbarBorder);

    healthbar->transform.size.x = healthbar->background.width;
    healthbar->transform.size.y = healthbar->background.height;

    return healthbar;
}

void DrawUIHealthbar(UIHealthbar* healthbar)
{
    DrawTextureV(healthbar->background, healthbar->transform.position, WHITE);
    DrawUIHealthbarFill(healthbar);
    DrawTextureV(healthbar->border, healthbar->transform.position, WHITE);
}

void DrawUIHealthbarFill(UIHealthbar* healthbar)
{
    Vector2 offset = {2, 2};

    Rectangle fillSource = {
        0,
        0,
        healthbar->transform.size.x,
        healthbar->transform.size.y
    };

    Rectangle fillDest = {
        healthbar->transform.position.x + offset.x,
        healthbar->transform.position.y + offset.y,
        (healthbar->transform.size.x - offset.x) * healthbar->fillValue,
        healthbar->transform.size.y - offset.y
    };

    DrawTexturePro(healthbar->fill, fillSource, fillDest, (Vector2){0, 0}, 0, WHITE);
}

UICoins* CreateUICoins(float x, float y)
{
    UICoins* uiCoins = malloc(sizeof(UICoins));
    uiCoins->transform.position.x = x;
    uiCoins->transform.position.y = y;
    uiCoins->icon = LoadTextureFromImage(coinUIImage);
    uiCoins->count = 0;

    return uiCoins;
}

void DrawUICoins(UICoins* uiCoins)
{
    char* coinText = IntegerToString(uiCoins->count);
    Vector2 offset = {26, 0};
    Vector2 textPosition = Vector2Add(uiCoins->transform.position, offset);
    DrawTextureV(uiCoins->icon, uiCoins->transform.position, WHITE);
    DrawText(coinText, textPosition.x, textPosition.y, 20, WHITE);
    free(coinText);
}

PlayerUI* CreatePlayerUI()
{
    PlayerUI* playerUi = (PlayerUI*)malloc(sizeof(PlayerUI));
    playerUi->healthbar = CreateUIHealthbar(10, 10);
    playerUi->coins = CreateUICoins(10, 52);
    return playerUi;
}

void UpdatePlayerUI(PlayerUI* playerUi)
{
    Player* player = GetPlayer();
    if (player != NULL) 
    {
        float fillValue = 0.0f;
        if (player->health > 0) 
        {
            fillValue = player->health / player->maxHealth;
        }

        if (player->coins > 0)
        {
            playerUi->coins->count = player->coins;
        }

        playerUi->healthbar->fillValue = fillValue;
    }
}

void DrawPlayerUI(PlayerUI* playerUi)
{
    DrawUIHealthbar(playerUi->healthbar);
    DrawUICoins(playerUi->coins);
}

void FreePlayerUI(PlayerUI* playerUi)
{
    free(playerUi->healthbar);
    free(playerUi->coins);
    MyFree((void**)(&playerUi));
}