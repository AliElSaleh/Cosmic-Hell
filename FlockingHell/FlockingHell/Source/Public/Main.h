#pragma once

#include "Player.h"
#include "Star.h"
#include "State.h"
#include "EnemyManager.h"
#include "CirclePattern.h"
#include "RaylibLogo.h"
#include "Planet.h"
#include "Credits.h"

#include <WS2tcpip.h> // Sending and recieving data from server

// Variable Declarations
//-----------------------------------------
// Structs
struct Player Player;
struct Star FarStars[MAX_FAR_STARS];
struct Star SmallStars[MAX_SMALL_STARS];
struct Star CloseStars[MAX_CLOSE_STARS];
struct EnemyManager EnemyManager;
struct Credits Credits;

// UI
Texture2D Background;
Texture2D MenuTitle;
Texture2D Panel;

// Splash screen
RaylibLogo SplashLogo;

// Planets
Planet BigPlanet[5];
Planet RingPlanet[2];

// Music
Music MenuMusic;
Music GameMusic;
Music WinMusic;

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

// Music fading in game
float MusicFade{0.0f};
bool bMusicFaded{false};

// Server
WSADATA Data;
WORD Version;
SOCKET Socket{};
sockaddr_in Hint{};
std::string DataToSend;
char Buffer[4096];
unsigned short ReconnectDelay{7200}; // 1 minute (in frames)

// bools
bool bConnected{false};
bool bBegan{false};
bool bDebug;


// Function Declarations
//-----------------------------------------
void Init(); // Initialise application
void InitGame(); // Create instances of objects and initialise their member variables
void SetGameState(State state); // Sets the next frame's current game state to the specified state
bool ConnectToServer();
void SendData();

void Update(); // Update logic depending on the current game state
void Draw(); // Render graphics depending on the current game state

void UpdateSplashScreen();
void UpdateMenuScreen();
void UpdateGame(); // Game Update loop
void UpdatePauseScreen();
void UpdateDeathScreen();
void UpdateWinScreen();
void UpdateLeaderboardScreen();
void UpdateStatsScreen();
void UpdateCreditsScreen();
void UpdateTestScene();
void UpdateFlockingScene();

void DrawSplashScreen();
void DrawGame(); // Game Render loop
void DrawGameUI();
void DrawMenuScreen();
void DrawPauseScreen();
void DrawDeathScreen();
void DrawWinScreen();
void DrawLeaderboardScreen();
void DrawStatsScreen();
void DrawCreditsScreen();
void DrawTestScene();
void DrawFlockingScene();

// Load/Save system
void LoadHighscore();
void SaveHighscore();

void Unload(); // Unload all assets that have been loaded since InitWindow()
