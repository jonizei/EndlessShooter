#include "include/utils.h"
#include "include/raylib.h"
#include "include/raymath.h"
#include<math.h>
#include<string.h>

// CONSTANT VALUES
const int STRENGTH_MAX_LEVEL = 10;
const int MOVEMENT_SPEED_MAX_LEVEL = 10;
const int ATTACK_SPEED_MAX_LEVEL = 10;
const int STAMINA_MAX_LEVEL = 10;

const float STRENGTH_FACTOR = 5.0f;
const float MOVEMENT_SPEED_FACTOR = 0.10f;
const float ATTACK_SPEED_FACTOR = 0.1f;
const float STAMINA_FACTOR = 50;

const char* STRENGTH_TEXT = "STR";
const char* MOVEMENT_SPEED_TEXT = "MSPD";
const char* ATTACK_SPEED_TEXT = "ASPD";
const char* STAMINA_TEXT = "STAM";

// GLOBAL VARIABLES
Camera2D* _CameraRef = NULL;
GameMap * _GameMapRef = NULL;

// PRIVATE FUNCTION DECLARATIONS

// PUBLIC FUNCTIONS

void SetCamera(Camera2D* camera)
{
    _CameraRef = camera;
}

Camera2D* GetCamera()
{
    return _CameraRef;
}

void SetGameMap(GameMap* gameMap)
{
    _GameMapRef = gameMap;
}

GameMap* GetGameMap()
{
    return _GameMapRef;
}

// TODO: Make sure this function works correctly
void MyFree(void** ptr)
{
    free(*ptr);
    *ptr = NULL;
}

Vector2 GetOrigin(Transform2D transform)
{
    return (Vector2){
        transform.position.x + transform.size.x / 2,
        transform.position.y + transform.size.y / 2
    };
}

Vector2 GetMouseWorldPosition()
{
    return GetWorldPosition(GetMousePosition());
}

Vector2 GetWorldPosition(Vector2 position)
{
    return GetScreenToWorld2D(position, *GetCamera());
}

float GetMouseDirection(Vector2 position)
{
    Vector2 mousePosition = GetMouseWorldPosition();
    return GetDirection(position, mousePosition);
}

float GetDirection(Vector2 from, Vector2 to)
{
    return atan2(to.y - from.y, to.x - from.x);
}

Vector2 GetPositionInDistance(Vector2 position, float distance, float direction)
{
    float x = position.x + cos(direction) * distance;
    float y = position.y + sin(direction) * distance;
    return (Vector2){ x, y };
}

Vector2 GetOffsetPositionInDistance(Vector2 position, float offset, float distance, float direction)
{
    float offsetDirection = direction + radToDeg(90);
    Vector2 offsetPosition = GetPositionInDistance(position, offset, offsetDirection);
    return GetPositionInDistance(offsetPosition, distance, direction);
}

Box2D CreateBox2D(Vector2 position, float width, float height, float direction)
{
    Vector2 corner = {
        position.x + width,
        position.y + height
    };

    Box2D box = {
        position,
        corner,
        width,
        height,
        direction
    };

    return RotateBox2D(box, direction);
}

Box2D RotateBox2D(Box2D box, float direction)
{
    Vector2 topLeft = GetPositionInDistance(box.min, box.height, direction);
    Vector2 max = GetPositionInDistance(topLeft, box.width, direction + degToRad(90));

    return (Box2D){
        box.min,
        max,
        box.width,
        box.height,
        box.angle
    };
}

Box2D MoveBox2D(Box2D box, Vector2 position)
{
    return CreateBox2D(position, box.width, box.height, box.angle);
}

void DrawBox2D(Box2D box)
{
    Vector2 topLeft = GetPositionInDistance(box.min, box.height, box.angle);
    Vector2 bottomRight = GetPositionInDistance(box.max, box.height, degToRad(180) + box.angle);
    DrawLineV(box.min, topLeft, RED);
    DrawLineV(topLeft, box.max, RED);
    DrawLineV(box.max, bottomRight, RED);
    DrawLineV(bottomRight, box.min, RED);
}

Vector2* Box2DToPoints(Box2D box)
{
    Vector2* points = malloc(4 * sizeof(Vector2));
    points[0] = box.min;
    points[1] = GetPositionInDistance(box.min, box.height, box.angle);
    points[2] = box.max;
    points[3] = GetPositionInDistance(box.max, box.height, degToRad(180) + box.angle);
    return points;
}

bool IsInsideBox2D(Box2D box, Vector2 point)
{
    Vector2* points = Box2DToPoints(box);
    bool isInsideA = CheckCollisionPointTriangle(point, points[0], points[1], points[2]);
    bool isInsideB = CheckCollisionPointTriangle(point, points[0], points[3], points[2]);
    free(points);
    return isInsideA || isInsideB;
}

float degToRad(float degrees)
{
    return degrees * DEG2RAD;
}

float radToDeg(float radians)
{
    return radians / DEG2RAD;
}

const char* GetStatTypeText(StatType type)
{
    const char* text = "";
    switch(type)
    {
        case STAT_STRENGTH:
            text = STRENGTH_TEXT;
        break;

        case STAT_MOVEMENT_SPEED:
            text = MOVEMENT_SPEED_TEXT;
        break;

        case STAT_ATTACK_SPEED:
            text = ATTACK_SPEED_TEXT;
        break;

        case STAT_STAMINA:
            text = STAMINA_TEXT;
        break;
    }

    return text;
}

float GetStatValue(Stats stats, StatType type)
{
    float statValue = 0;
    switch(type) 
    {
        case STAT_STRENGTH:
            if (stats.strength <= STRENGTH_MAX_LEVEL) 
            {
                statValue = stats.strength * STRENGTH_FACTOR;
            }
        break;

        case STAT_MOVEMENT_SPEED:
            if (stats.movementSpeed <= MOVEMENT_SPEED_MAX_LEVEL) 
            {
                statValue = stats.movementSpeed * MOVEMENT_SPEED_FACTOR;
            }
        break;

        case STAT_ATTACK_SPEED:
            if (stats.attackSpeed <= ATTACK_SPEED_MAX_LEVEL) 
            {
                statValue = (ATTACK_SPEED_MAX_LEVEL - stats.attackSpeed) * ATTACK_SPEED_FACTOR;
            }
        break;

        case STAT_STAMINA:
            if (stats.stamina <= STAMINA_MAX_LEVEL)
            {
                statValue = stats.stamina * STAMINA_FACTOR;
            }
        break;
    }

    return statValue;
}

bool IsStatMaxLevel(Stats stats, StatType statType)
{
    bool isMaxLevel = false;
    switch(statType)
    {
        case STAT_STRENGTH:
            isMaxLevel = stats.strength == STRENGTH_MAX_LEVEL;
        break;

        case STAT_MOVEMENT_SPEED:
            isMaxLevel = stats.movementSpeed == MOVEMENT_SPEED_MAX_LEVEL;
        break;

        case STAT_ATTACK_SPEED:
            isMaxLevel = stats.attackSpeed == ATTACK_SPEED_MAX_LEVEL;
        break;

        case STAT_STAMINA:
            isMaxLevel = stats.stamina == STAMINA_MAX_LEVEL;
        break;
    }

    return isMaxLevel;
}

void UpdateStatPoint(Stats* stats, StatType statType, int points)
{
    switch(statType) 
    {
        case STAT_STRENGTH:
            if (stats->strength + points <= STRENGTH_MAX_LEVEL) 
            {
                stats->strength += points;
            }
        break;

        case STAT_MOVEMENT_SPEED:
            if (stats->movementSpeed + points <= MOVEMENT_SPEED_MAX_LEVEL) 
            {
                stats->movementSpeed += points;
            }
        break;

        case STAT_ATTACK_SPEED:
            if (stats->attackSpeed + points <= ATTACK_SPEED_MAX_LEVEL) 
            {
                stats->attackSpeed += points;
            }
        break;

        case STAT_STAMINA:
            if (stats->stamina + points <= STAMINA_MAX_LEVEL)
            {
                stats->stamina += points;
            }
        break;
    }
}

char* IntegerToString(int value)
{
    int length = snprintf(NULL, 0, "%d", value);
    char* str = calloc(length + 1, sizeof(char));
    sprintf(str, "%d", value);
    return str;
}

void DrawTextureBySize(Texture2D texture, Vector2 position, Vector2 size, float rotation, bool flipX, bool flipY)
{
    Rectangle source = {
        0,
        0,
        flipX == true ? -texture.width : texture.width,
        flipY == true ? -texture.height : texture.height
    };

    Rectangle destination = {
        position.x,
        position.y,
        size.x,
        size.y
    };

    DrawTexturePro(texture, source, destination, (Vector2){0, 0}, rotation, WHITE);
}

// PRIVATE FUNCTIONS
