#pragma once

#include "raylib.h"
#include "Player.h"
#include "Bullet.h"
#include "Star.h"
#include "State.h"

#define MAX_SMALL_BULLETS 20
#define MAX_PLAYER_BULLETS 50
#define MAX_SMALL_STARS 30

// Variable Declarations
//-----------------------------------------
// Structs
struct Player Player;
struct Bullet Bullet[MAX_SMALL_BULLETS];
struct Star SmallStars[MAX_SMALL_STARS];
struct Vector2 BulletLocation[MAX_SMALL_BULLETS];
enum State GameState;

// ints
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 800;
int ShootRate = 0;
int Offset = 0;
int FramesCounter = 0;

// bools
bool bShowDebugHit = false;


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
void UpdateSplashScreen();
void DrawSplashScreen();
void UpdateGame();
void DrawGame();
void UpdateMenu();
void DrawMenu();