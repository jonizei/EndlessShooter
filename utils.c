#include "include/utils.h"
#include<math.h>

// PRIVATE FUNCTION DECLARATIONS

bool IsOverlap(int aLeft, int aRight, int bLeft, int bRight);

// PUBLIC FUNCTIONS

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

double GetMouseDirection(Vector2 position)
{
    Vector2 mousePosition = GetMousePosition();
    return atan2(mousePosition.y - position.y, mousePosition.x - position.x);
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