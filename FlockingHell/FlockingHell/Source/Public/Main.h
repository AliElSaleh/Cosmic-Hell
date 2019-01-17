#pragma once

#include "Player.h"
#include "Star.h"
#include "State.h"
#include "Demon.h"
#include "Warship.h"
#include "EnemyManager.h"
#include "SpiralMultiPattern.h"
#include "SpiralOneWayPattern.h"
#include "SpiralMultiWayPattern.h"
#include "CirclePattern.h"
#include "RandomPattern.h"
#include "LinearPattern.h"
#include "LinearMultiPattern.h"
#include "Boid.h"
#include "Flock.h"

// Variable Declarations
//-----------------------------------------
// Structs
struct Player Player;
struct Star SmallStars[MAX_SMALL_STARS];
struct Star FastStars[MAX_SMALL_STARS];
struct Star CloseStars[MAX_SMALL_STARS];
struct EnemyManager EnemyManager;
struct Flock Flock;
struct Boid Ship;

template<typename PatternType>
PatternType Pattern;

// typedefs
typedef CirclePattern BulletType;

// enums
enum State GameState;

// ints
unsigned short FramesCounter = 0; // To time the splash screen

// bools
bool bDebug;


// Function Declarations
//-----------------------------------------
void Init(); // Initialise application
void InitGame(); // Create instances of objects and initialise their member variables
void SetGameState(State state); // Sets the next frame's current game state to the specified state

void Update(); // Update logic depending on the current game state
void Draw(); // Render graphics depending on the current game state

void UpdateSplashScreen();
void UpdateMenuScreen();
void UpdateGame(); // Game Update loop
void UpdatePauseScreen();
void UpdateDeathScreen();
void UpdateWinScreen();
void UpdateLoseScreen();
void UpdateStatsScreen();
void UpdateHighscoreScreen();
void UpdateTestScene();
void UpdateFlockingScene();

void DrawSplashScreen();
void DrawGame(); // Game Render loop
void DrawGameUI();
void DrawMenuScreen();
void DrawPauseScreen();
void DrawDeathScreen();
void DrawWinScreen();
void DrawLoseScreen();
void DrawStatsScreen();
void DrawHighscoreScreen();
void DrawTestScene();
void DrawFlockingScene();
void DrawDebugInfo();

void ResetGame();
void UnloadTextures(); // Unload all textures that have been loaded since InitWindow()
