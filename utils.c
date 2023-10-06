#include "include/utils.h"
#include "include/raylib.h"
#include "include/raymath.h"
#include<math.h>

// GLOBAL VARIABLES
Camera2D* _CameraRef = NULL;
float _MapWidth = 0.0f;
float _MapHeight = 0.0f;

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

void SetMapWidth(float width)
{
    _MapWidth = width;
}

void SetMapHeight(float height)
{
    _MapHeight = height;
}

float GetMapWidth()
{
    return _MapWidth;
}

float GetMapHeight()
{
    return _MapHeight;
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

// PRIVATE FUNCTIONS
