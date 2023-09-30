#include "include/utils.h"
#include<math.h>

// GLOBAL VARIABLES
Camera2D* cameraRef = NULL;

// PRIVATE FUNCTION DECLARATIONS

bool IsOverlap(int aLeft, int aRight, int bLeft, int bRight);

// PUBLIC FUNCTIONS

void SetCamera(Camera2D* camera)
{
    cameraRef = camera;
}

Camera2D* GetCamera()
{
    return cameraRef;
}

bool IsCollision(Transform2D a, Transform2D b)
{
    bool xOverlaps = IsOverlap(a.position.x, a.position.x + a.size.x, b.position.x, b.position.x + b.size.x);
    bool yOverlaps = IsOverlap(a.position.y, a.position.y + a.size.y, b.position.y, b.position.y + b.position.y);

    return xOverlaps && yOverlaps;
}

// TODO: Make sure this function works correctly
void MyFree(void** ptr)
{
    free(*ptr);
    *ptr = NULL;
}

Vector2 GetLocalMousePosition()
{
    return GetScreenToWorld2D(GetMousePosition(), *GetCamera());
}

float GetMouseDirection(Vector2 position)
{
    Vector2 mousePosition = GetLocalMousePosition();
    return atan2(mousePosition.y - position.y, mousePosition.x - position.x);
}

Vector2 GetPositionInDistance(Vector2 position, float distance, float direction)
{
    float x = position.x + cos(direction) * distance;
    float y = position.y + sin(direction) * distance;
    return (Vector2){ x, y };
}

bool ContainsPosition(Vector2 position, int screenWidth, int screenHeight)
{
    bool isXOut = position.x < 0 || position.x > screenWidth;
    bool isYOut = position.y < 0 || position.y > screenHeight;

    return !isXOut && !isYOut;
}

// PRIVATE FUNCTIONS

bool IsOverlap(int aLeft, int aRight, int bLeft, int bRight)
{
    bool leftOverlaps = aLeft > bLeft && aLeft < bRight;
    bool rightOverlaps = aRight > bLeft && aRight < bRight;
    return leftOverlaps || rightOverlaps;
}