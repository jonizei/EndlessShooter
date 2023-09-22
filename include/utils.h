#ifndef UTILS_H
#define UTILS_H

#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include "raylib.h"

typedef struct _transform2d {
    Vector2 position;
    Vector2 size;
} Transform2D;

void MyFree(void** ptr);
double GetMouseDirection(Vector2 position);
bool ContainsPosition(Vector2 position, int screenWidth, int screenHeight);

#endif