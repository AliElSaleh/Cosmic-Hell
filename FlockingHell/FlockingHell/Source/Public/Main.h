#pragma once

#include "Player.h"
#include "Star.h"
#include "State.h"
#include "EnemyManager.h"
#include "CirclePattern.h"
#include "RaylibLogo.h"
#include "Planet.h"
#include "Credits.h"
#include "Leaderboard.h"

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
struct Stats LifetimeStats;
struct Stats DummyStats[10];
//struct Leaderboard Leaderboard;

// UI
Texture2D Background;
Texture2D MenuTitle;
Texture2D Panel;
Texture2D Selector;

// Splash screen
RaylibLogo SplashLogo;

// Fonts
Font PixelFont;

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
unsigned int HighGrazingScore = 0;
unsigned int TotalScore = 0;
unsigned int ScoreCounter = 0;
unsigned short GamesPlayed = 0;
unsigned short FlawlessRuns = 0;

// Menu title animation variables
Vector2 TitleLocation{287.0f, 206.0f};
unsigned short SpriteFramesCounter = 0;
const unsigned short FramesSpeed = 10;
unsigned short CurrentFrame = 0;
const unsigned short Frames = 4;

// Menu Fading animation
float FadeAlpha{0.0f};

// Options menu variables
Vector2 StartLocation{359.0f, 388.0f};
Vector2 EndLocation{359.0f, 628.0f};
Vector2 SelectorLocation{};
signed short Index{0};
unsigned short JumpAmount{60};

// Music fading in game
float MusicFade{0.0f};
bool bMusicFaded{false};

// Server
WSADATA Data;
WORD Version;
SOCKET Socket{};
sockaddr_in Hint{};
std::string HighscoreString;
std::string GrazingScoreString;
std::string DeathsString;
std::string BombsUsedString;
std::string FlawlessRunsString;
const char* Buffer[1000000];
char ReceiveBuffer[1000000];
std::string Stats[6];
unsigned short ServerSendDelay{50};
unsigned short StatsIndex{0};
unsigned short ReconnectDelay{7200}; // 1 minute (in frames)
unsigned short ServerFramesCounter{};

// bools
bool bDataSent{false};
bool bDataReceived{false};
bool bSoundPlayed[5]{false};
bool bEntered{false}; // To connect immediately upon entering the leaderboard game state
bool bConnected{false};
bool bBegan{false};
bool bDebug;


// Function Declarations
//-----------------------------------------
void Init(); // Initialise application
void InitGame(); // Create instances of objects and initialise their member variables
void SetGameState(State State); // Sets the next frame's current game state to the specified state
bool ConnectToServer();

void Update(); // Update logic depending on the current game state
void Draw(); // Render graphics depending on the current game state

void UpdateSplashScreen();
void UpdateMenuScreen();
void UpdateOptionsScreen();
void UpdateGame(); // Game Update loop
void UpdatePauseScreen();
void UpdateStats();
void UpdateDeathScreen();
void UpdateWinScreen();
void UpdateLeaderboardScreen();
void UpdateStatsScreen();
void UpdateCreditsScreen();

void DrawSplashScreen();
void DrawGame(); // Game Render loop
void DrawGameUI();
void DrawMenuScreen();
void DrawOptionsScreen();
void DrawPauseScreen();
void DrawDeathScreen();
void DrawWinScreen();
void DrawLeaderboardScreen();
void DrawStatsScreen();
void DrawCreditsScreen();

// Load/Save system
void LoadHighscore();
void SaveHighscore();

void Unload(); // Unload all assets that have been loaded since InitWindow()
