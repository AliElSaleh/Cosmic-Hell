#pragma once

#include "Player.h"
#include "Star.h"
#include "State.h"
#include "EnemyManager.h"
#include "CirclePattern.h"
#include "Flock.h"
#include "ArchDemon.h"
#include "Explosion.h"
#include "QuadTree.h"
#include "HealthPickup.h"
#include "RaylibLogo.h"
#include "Planet.h"

// Variable Declarations
//-----------------------------------------
// Structs
struct Player Player;
struct Star FarStars[MAX_FAR_STARS];
struct Star SmallStars[MAX_SMALL_STARS];
struct Star CloseStars[MAX_CLOSE_STARS];
struct EnemyManager EnemyManager;
struct Flock Flock;
struct ArchDemon Archon;
struct Explosion BombExplosion;
struct HealthPickup HeartPickup;

Texture2D Background;
Texture2D MenuTitle;
Texture2D Panel;

Section Boundary = Section(350, 400, 350, 400);
QuadTree* QT = new QuadTree(Boundary, 4);

RaylibLogo SplashLogo;

Planet BigPlanet[5];
Planet RingPlanet[2];

// Music
Music MenuMusic;
Music GameMusic;

template<typename PatternType>
PatternType Pattern;

// typedefs
typedef CirclePattern BulletType;

// enums
enum State GameState;

// ints
unsigned short FramesCounter = 0; // To time things
unsigned short MenuFramesCounter = 0;
unsigned short BombSpriteSpacing = 40;
unsigned short HeartSpriteSpacing = 30;

unsigned short ScorePosition = 46;
unsigned short GrazingScorePosition = 90;

unsigned int Highscore = 0;
unsigned int TotalScore = 0;
unsigned int ScoreCounter = 0;

// Menu title animation variables
Vector2 TitleLocation{287.0f, 206.0f};
unsigned short SpriteFramesCounter = 0;
const unsigned short FramesSpeed = 10;
unsigned short CurrentFrame = 0;
const unsigned short Frames = 4;

// Menu Fading animation
float FadeAlpha{0.0f};

// bools
bool bBegan{false};
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
void DrawStatsScreen();
void DrawHighscoreScreen();
void DrawTestScene();
void DrawFlockingScene();

void Unload(); // Unload all assets that have been loaded since InitWindow()
