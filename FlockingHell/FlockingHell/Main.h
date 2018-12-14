#pragma once

#include "raylib.h"
#include "Player.h"
#include "Bullet.h"

#define MAX_SMALL_BULLETS 20
#define MAX_PLAYER_BULLETS 50

// Variable Declarations
//-----------------------------------------
struct Player Player;
struct Bullet Bullet[MAX_SMALL_BULLETS];
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 800;
bool bShowDebugHit = false;
bool bKeyPressed = false;
int ShootRate = 0;

// Function Declarations
//-----------------------------------------
void Init();
void Update();
void Draw();
void MovePlayer();
void CheckWindowCollision();
void UpdateBullet();
void CheckBulletCollision();
void CheckBulletOutsideWindow();
void CheckPlayerHealth();
void UpdatePlayerBullet();