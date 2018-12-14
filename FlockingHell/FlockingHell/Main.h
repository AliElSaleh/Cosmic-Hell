#pragma once

#include "raylib.h"
#include "Player.h"
#include "Bullet.h"
#include "Star.h"

#define MAX_SMALL_BULLETS 20
#define MAX_PLAYER_BULLETS 50
#define MAX_SMALL_STARS 30

// Variable Declarations
//-----------------------------------------
struct Player Player;
struct Bullet Bullet[MAX_SMALL_BULLETS];
struct Star SmallStars[MAX_SMALL_STARS];
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 800;
bool bShowDebugHit = false;
int ShootRate = 0;
int Offset = 0;

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
void UpdateStars();
void CheckStarsOutsideWindow();