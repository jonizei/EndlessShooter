#ifndef UTILS_H
#define UTILS_H

#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "raylib.h"

typedef struct _transform2d {
    Vector2 position;
    Vector2 size;
    float scale;
} Transform2D;

void SetCamera(Camera2D* camera);
Camera2D* GetCamera();
void MyFree(void** ptr);
Vector2 GetLocalMousePosition();
float GetMouseDirection(Vector2 position);
bool ContainsPosition(Vector2 position, int screenWidth, int screenHeight);
bool IsCollision(Transform2D a, Transform2D b);

#endif