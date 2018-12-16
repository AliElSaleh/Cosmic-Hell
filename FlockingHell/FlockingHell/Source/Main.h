#pragma once

#include "raylib.h"
#include "Player.h"
#include "Bullet.h"
#include "Star.h"
#include "State.h"

#define MAX_SMALL_BULLETS 20
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
int FramesCounter = 0;

// bools
bool bShowDebugHit = false;
bool bDebug = false;


// Function Declarations
//-----------------------------------------
void Init();
void InitGame();
void Update();
void Draw();
void UpdateBullet();
void CheckBulletCollision();
void CheckBulletOutsideWindow();
void CheckPlayerHealth();
void UpdateStars();
void CheckStarsOutsideWindow();
void UpdateSplashScreen();
void DrawSplashScreen();
void UpdateGame();
void DrawGame();
void UpdateMenuScreen();
void DrawMenuScreen();
void UpdatePauseScreen();
void UpdateDeathScreen();
void UpdateWinScreen();
void UpdateLoseScreen();
void UpdateStatsScreen();
void UpdateHighscoreScreen();
void DrawPauseScreen();
void DrawDeathScreen();
void DrawWinScreen();
void DrawLoseScreen();
void DrawStatsScreen();
void DrawHighscoreScreen();
void ResetGame();
void DrawGameUI();
void DrawStars();
void DrawPlayer();
void DrawEnemyBullets();
void DrawDebugInfo();
void UnloadTextures();