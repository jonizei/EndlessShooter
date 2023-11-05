#ifndef UTILS_H
#define UTILS_H

#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "game_map.h"
#include "raylib.h"

typedef struct _transform2d {
    Vector2 position;
    Vector2 size;
    float scale;
} Transform2D;

typedef struct _box2d {
    Vector2 min;
    Vector2 max;
    float width;
    float height;
    float angle;
} Box2D;

typedef struct _line2d {
    Vector2 a;
    Vector2 b;
} Line2D;

typedef struct _stats {
    int strength;
    int movementSpeed;
    int attackSpeed;
    int stamina;
} Stats;

typedef enum {
    STAT_STRENGTH,
    STAT_MOVEMENT_SPEED,
    STAT_ATTACK_SPEED,
    STAT_STAMINA
} StatType;

void SetCamera(Camera2D* camera);
Camera2D* GetCamera();
void SetGameMap(GameMap* gameMap);
GameMap* GetGameMap();
void MyFree(void** ptr);
Vector2 GetOrigin(Transform2D transform);
Vector2 GetMouseWorldPosition();
Vector2 GetWorldPosition(Vector2 position);
float GetMouseDirection(Vector2 position);
float GetDirection(Vector2 from, Vector2 to);
Vector2 GetPositionInDistance(Vector2 position, float distance, float direction);
Box2D CreateBox2D(Vector2 position, float width, float height, float direction);
Box2D RotateBox2D(Box2D box, float direction);
Box2D MoveBox2D(Box2D box, Vector2 position);
void DrawBox2D(Box2D box);
Vector2* Box2DToPoints(Box2D box);
bool IsInsideBox2D(Box2D box, Vector2 point);
float degToRad(float degrees);
const char* GetStatTypeText(StatType type);
float GetStatValue(Stats stats, StatType type);
bool IsStatMaxLevel(Stats stats, StatType statType);
void UpdateStatPoint(Stats* stats, StatType statType, int points);
char* IntegerToString(int value);
void DrawTextureBySize(Texture2D, Vector2 position, Vector2 size);

#endif