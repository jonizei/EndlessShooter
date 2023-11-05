#include "include/ui_utils.h"
#include "include/raylib.h"
#include "include/utils.h"
#include "include/player.h"
#include "include/raymath.h"
#include "include/merchant.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

// CONSTANT VALUES
const int MERCHANT_UI_WIDTH = 288;
const int MERCHANT_UI_HEIGHT = 288;
const int MERCHANT_UI_BTN_WIDTH = 96;
const int MERCHANT_UI_BTN_HEIGHT = 64;
const int MERCHANT_UI_EXIT_BTN_WIDTH = 32;
const int MERCHANT_UI_EXIT_BTN_HEIGHT = 32;

// PRIVATE STRUCTS
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

typedef struct _ui_button {
    Transform2D transform;
    Texture2D background;
    Texture2D pressedBackground;
    Rectangle collider;
    bool isPressed;
} UIButton;

typedef struct _ui_merchant_button {
    UIButton* button;
    StatType statType;
    int points;
    int price;
} MerchantUIButton;

typedef struct _ui_merchant {
    Transform2D transform;
    Texture2D background;
    UIButton* exitButton;
    Merchant* activeMerchant;
    MerchantUIButton** buttons;
} MerchantUI;

struct _ui_layer {
    PlayerUI* playerUi;
    MerchantUI* merchantUi;
};

// GLOBAL VARIABLES
UILayer* _mainUiLayer = NULL;
bool _merchantUiOpen = false;

Image healthbarBackground;
Image healthbarFill;
Image healthbarBorder;
Image coinUIImage;
Image merchantBackground;
Image merchantBtnBackground;
Image merchantBtnDownBackground;
Image merchantExitBtnBackground;
Texture2D merchantBtnTexture;
Texture2D merchantBtnDownTexture;
Texture2D merchantExitBtnTexture;

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
MerchantUI* CreateMerchantUI();
void DrawMerchantUI(MerchantUI* merchantUi);
void FreeMerchantUI(MerchantUI* merchantUi);
UIButton* CreateUIButton(float x, float y, float width, float height, Texture2D texture, Texture2D pressedTexture);
void DrawUIButton(UIButton* uiButton);
void FreeUIButton(UIButton* uiButton);
MerchantUIButton* CreateMerchantUIButton(float x, float y, int points, int price);
MerchantUIButton** CreateMerchantUIButtons(float x, float y, float width, float height, float scale);
void DrawMerchantUIButton(MerchantUIButton* merchantButton);
void FreeMerchantUIButton(MerchantUIButton* merchantButton);
void UpdateMerchantUI(MerchantUI* merchantUi);
void ClickMerchantUIButton(MerchantUIButton* merchantButton);
void UpdateMerchantUIButtons(MerchantUI* merchantUi);

// PUBLIC FUNCTIONS
void LoadUIResources()
{
    healthbarBackground = LoadImage("resources/textures/ui/healthbar_background_sprite.png");
    healthbarFill = LoadImage("resources/textures/ui/healthbar_fill_sprite.png");
    healthbarBorder = LoadImage("resources/textures/ui/healthbar_border_sprite.png");
    coinUIImage = LoadImage("resources/textures/ui/coin_sprite.png");
    merchantBackground = LoadImage("resources/textures/ui/merchant_menu_sprite.png");
    merchantBtnBackground = LoadImage("resources/textures/ui/merchant_menu_btn_sprite.png");
    merchantBtnDownBackground = LoadImage("resources/textures/ui/merchant_menu_btn_down_sprite.png");
    merchantExitBtnBackground = LoadImage("resources/textures/ui/merchant_menu_exit_btn_sprite.png");

    merchantBtnTexture = LoadTextureFromImage(merchantBtnBackground);
    merchantBtnDownTexture = LoadTextureFromImage(merchantBtnDownBackground);
    merchantExitBtnTexture = LoadTextureFromImage(merchantExitBtnBackground);
}

void UnloadUIResources()
{
    UnloadImage(healthbarBackground);
    UnloadImage(healthbarFill);
    UnloadImage(healthbarBorder);
    UnloadImage(coinUIImage);
    UnloadImage(merchantBackground);
    UnloadImage(merchantBtnBackground);
    UnloadImage(merchantBtnDownBackground);
    UnloadImage(merchantExitBtnBackground);
}

bool IsUIOpen()
{
    // Put all ui flags here
    // If any of them is true then this returns true
    return _merchantUiOpen;
}

UILayer* CreateUILayer()
{
    UILayer* uiLayer = (UILayer*)malloc(sizeof(UILayer));
    uiLayer->playerUi = CreatePlayerUI();
    uiLayer->merchantUi = CreateMerchantUI();
    return uiLayer;
}

void SetMainUILayer(UILayer* uiLayer)
{
    _mainUiLayer = uiLayer;
}

UILayer* GetMainUILayer()
{
    return _mainUiLayer;
}

void UpdateUILayer(UILayer* uiLayer)
{
    UpdatePlayerUI(uiLayer->playerUi);
    if (_merchantUiOpen)
    {
        UpdateMerchantUI(uiLayer->merchantUi);
    }
}

void DrawUILayer(UILayer* uiLayer)
{
    DrawPlayerUI(uiLayer->playerUi);
    if (_merchantUiOpen)
    {
        DrawMerchantUI(uiLayer->merchantUi);
    }
}

void FreeUILayer(UILayer* uiLayer)
{
    FreePlayerUI(uiLayer->playerUi);
    FreeMerchantUI(uiLayer->merchantUi);
    MyFree((void**)(&uiLayer));
}

void OpenMerchantUI(UILayer* uiLayer, Merchant* merchant)
{
    _merchantUiOpen = true;
    uiLayer->merchantUi->activeMerchant = merchant;
    UpdateMerchantUIButtons(uiLayer->merchantUi);
}

void CloseMerchantUI(UILayer* uiLayer)
{
    _merchantUiOpen = false;
    uiLayer->merchantUi->activeMerchant = NULL;
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
   DrawTextureBySize(healthbar->fill, healthbar->transform.position, healthbar->transform.size);
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

        if (player->coins >= 0)
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

MerchantUI* CreateMerchantUI()
{
    MerchantUI* merchantUi = malloc(sizeof(MerchantUI));
    merchantUi->transform.position.x = GetScreenWidth() / 2 - MERCHANT_UI_WIDTH / 2;
    merchantUi->transform.position.y = GetScreenHeight() / 2 - MERCHANT_UI_HEIGHT / 2;
    merchantUi->transform.size.x = MERCHANT_UI_WIDTH;
    merchantUi->transform.size.y = MERCHANT_UI_HEIGHT;
    merchantUi->background = LoadTextureFromImage(merchantBackground);

    merchantUi->activeMerchant = NULL;

    merchantUi->exitButton = CreateUIButton(
        merchantUi->transform.position.x + MERCHANT_UI_WIDTH - MERCHANT_UI_EXIT_BTN_WIDTH * 2
        , merchantUi->transform.position.y + MERCHANT_UI_EXIT_BTN_HEIGHT
        , MERCHANT_UI_EXIT_BTN_WIDTH
        , MERCHANT_UI_EXIT_BTN_HEIGHT
        , merchantExitBtnTexture
        , merchantExitBtnTexture
    );

    merchantUi->buttons = CreateMerchantUIButtons(
        merchantUi->transform.position.x
        , merchantUi->transform.position.y
        , merchantUi->transform.size.x
        , merchantUi->transform.size.y
        , merchantUi->transform.scale
    );

    return merchantUi;
}

void UpdateMerchantUI(MerchantUI* merchantUi)
{
    if (merchantUi == NULL)
    {
        return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
    {
        bool exitButtonClicked = CheckCollisionPointRec(GetMousePosition(), merchantUi->exitButton->collider);
        if (exitButtonClicked)
        {
            CloseMerchantUI(GetMainUILayer());
            return;
        }

        for (int i = 0; i < 4; i++) 
        {
            bool buttonClicked = CheckCollisionPointRec(GetMousePosition(), merchantUi->buttons[i]->button->collider);
            if (buttonClicked)
            {
                ClickMerchantUIButton(merchantUi->buttons[i]);
            }
        }
    }
    else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        merchantUi->exitButton->isPressed = false;
        
        for (int i = 0; i < 4; i++)
        {
            merchantUi->buttons[i]->button->isPressed = false;
        }
    }
}

MerchantUIButton* CreateMerchantUIButton(float x, float y, int points, int price)
{
    MerchantUIButton* merchantButton = malloc(sizeof(MerchantUIButton));
    merchantButton->button = CreateUIButton(x, y, MERCHANT_UI_BTN_WIDTH, MERCHANT_UI_BTN_HEIGHT, merchantBtnTexture, merchantBtnDownTexture);
    merchantButton->points = points;
    merchantButton->price = price;
    return merchantButton;
}

void DrawMerchantUIButton(MerchantUIButton* merchantButton)
{
    float x = merchantButton->button->transform.position.x;
    float y = merchantButton->button->transform.position.y;
    float width = merchantButton->button->transform.size.x;
    float height = merchantButton->button->transform.size.y;

    Vector2 pointTextPosition = {
        x + width / 8,
        y + height / 5
    };

    Vector2 priceTextPosition = {
        x + width / 8,
        y + (height / 5 * 3)
    };

    const char* statText = GetStatTypeText(merchantButton->statType);
    char* pointText = IntegerToString(merchantButton->points);
    char* priceText = IntegerToString(merchantButton->price);
    DrawUIButton(merchantButton->button);
    DrawText(pointText, pointTextPosition.x, pointTextPosition.y, 17, BLACK);
    DrawText(statText, pointTextPosition.x + 16, pointTextPosition.y, 17, BLACK);

    DrawText(priceText, priceTextPosition.x, priceTextPosition.y, 17, BLACK);
    DrawText("COINS", priceTextPosition.x + 16, priceTextPosition.y, 17, BLACK);

    free(pointText);
    free(priceText);
}

void UpdateMerchantUIButtons(MerchantUI* merchantUi)
{
    if (merchantUi == NULL) 
    {
        return;
    }

    if (merchantUi->activeMerchant != NULL) 
    {
        Merchant* active = merchantUi->activeMerchant;
        for (int i = 0; i < 4; i++)
        {
            MerchantUIButton* button = merchantUi->buttons[i];
            if (button != NULL && active->items[i] != NULL)
            {
                button->statType = active->items[i]->statType;
                button->points = active->items[i]->points;
                button->price = active->items[i]->price;
            }
        }
    }
}

void ClickMerchantUIButton(MerchantUIButton* merchantButton)
{
    merchantButton->button->isPressed = true;
    Player* player = GetPlayer();

    if (PlayerHasEnoughCoins(player, merchantButton->price) && IsStatMaxLevel(player->stats, merchantButton->statType))
    {
        PlayerSpentCoins(player, merchantButton->price);
        UpdateStatPoint(&(player->stats), merchantButton->statType, merchantButton->points);
        UpdatePlayerStats(player);
    }
}

void FreeMerchantUIButton(MerchantUIButton* merchantButton)
{
    FreeUIButton(merchantButton->button);
    MyFree((void**)&merchantButton);
}

MerchantUIButton** CreateMerchantUIButtons(float x, float y, float width, float height, float scale)
{
    float colGap = 16;
    float rowGap = 16;

    x = x + width / 2 - (MERCHANT_UI_BTN_WIDTH + colGap / 2);
    y = y + height / 2 - (MERCHANT_UI_BTN_HEIGHT + rowGap / 2);

    float secondColumn = x + MERCHANT_UI_BTN_WIDTH + colGap;
    float secondRow = y + MERCHANT_UI_BTN_HEIGHT + rowGap;

    MerchantUIButton** buttons = malloc(4 * sizeof(UIButton*));
    buttons[0] = CreateMerchantUIButton(x, y, 0, 0);
    buttons[1] = CreateMerchantUIButton(secondColumn, y, 0, 0);
    buttons[2] = CreateMerchantUIButton(x, secondRow, 0, 0);
    buttons[3] = CreateMerchantUIButton(secondColumn, secondRow, 0, 0);

    return buttons;
}

void DrawMerchantUI(MerchantUI* merchantUi)
{
    if (merchantUi == NULL)
    {
        return;
    }

    DrawTextureBySize(merchantUi->background, merchantUi->transform.position, merchantUi->transform.size);
    DrawUIButton(merchantUi->exitButton);

    for (int i = 0; i < 4; i++)
    {
        DrawMerchantUIButton(merchantUi->buttons[i]);
    }

}

void FreeMerchantUI(MerchantUI* merchantUi)
{
    for (int i = 0; i < 4; i++)
    {
        FreeMerchantUIButton(merchantUi->buttons[i]);
    }
    FreeUIButton(merchantUi->exitButton);
    MyFree((void**)&(merchantUi->buttons));
    MyFree((void**)&merchantUi);
}

UIButton* CreateUIButton(float x, float y, float width, float height, Texture2D texture, Texture2D pressedTexture)
{
    UIButton* button = malloc(sizeof(UIButton));
    button->transform.position.x = x;
    button->transform.position.y = y;
    button->transform.size.x = width;
    button->transform.size.y = height;
    button->background = texture;
    button->pressedBackground = pressedTexture;
    button->isPressed = false;
    button->collider = (Rectangle){
        x,
        y,
        width,
        height
    };
    return button;
}

void DrawUIButton(UIButton* uiButton)
{
    if (uiButton->isPressed) 
    {
        DrawTextureBySize(uiButton->pressedBackground, uiButton->transform.position, uiButton->transform.size);
    } 
    else 
    {
        DrawTextureBySize(uiButton->background, uiButton->transform.position, uiButton->transform.size);
    }
}

void FreeUIButton(UIButton* uiButton)
{
    MyFree((void**)&uiButton);
}