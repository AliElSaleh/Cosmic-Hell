#include "Main.h"
#include "Assets.h"

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

int main()
{
	Init();
	
	// Main game loop
	while (!WindowShouldClose())
	{
		Update();
		Draw();
	}

	UnloadTextures();

	CloseWindow();

    return 0;
}

// Function Definitions
//-----------------------------------------

void Init()
{
	// Window setup
	const int SCREEN_WIDTH = 700;
	const int SCREEN_HEIGHT = 800;
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cosmic Hell");
	SetTargetFPS(120);

	SetGameState(FLOCKING_TEST);

	ASSETS.LoadSprites();

	// Enemy Manager
	EnemyManager.AddEnemy(new Demon());
	EnemyManager.AddEnemy(new Warship());

	InitGame();
}

void InitGame()
{
	if (GameState == FLOCKING_TEST || GameState == TEST)
	{
		for (int i = 0; i < 50; i++)
		{
			Flock.Ships.emplace_back(new Ship());
			Flock.Ships[i]->bFlock = true;
		}

		//Ship.Init();
	}
	else
	{
		// Initialise Player
		Player.Init();
		Player.GameState = &GameState;

		Pattern<BulletType>.SetBulletPattern(BulletPatternGenerator::CIRCLE);
		Pattern<BulletType>.SetDebug(true);
		Pattern<BulletType>.Init();

		for (int i = 0; i < EnemyManager.Enemies.size(); i++)
		{
			EnemyManager.Enemies[i]->Player = &Player;
			EnemyManager.Enemies[i]->Init();
		}

		// Initialise stars
		for (int i= 0; i < MAX_SMALL_STARS; i++)
		{
			SmallStars[i].Init();

			FastStars[i].Init();
			FastStars[i].Speed = 700.0f;

			CloseStars[i].Init();
			CloseStars[i].Speed = 300.0f;
		}

		bDebug = false;
	}
}

void SetGameState(const State State)
{
	GameState = State;
}

void Update()
{
	switch (GameState)
	{
		case SPLASH:
			UpdateSplashScreen();
		break;

		case MENU:
			UpdateMenuScreen();
		break;

		case GAME:
			UpdateGame();
		break;

		case PAUSE:
			UpdatePauseScreen();
		break;

		case DEATH:
			UpdateDeathScreen();
		break;

		case WIN:
			UpdateWinScreen();
		break;

		case LOSE:
			UpdateLoseScreen();
		break;

		case STATS:
			UpdateStatsScreen();
		break;

		case HIGHSCORE:
			UpdateHighscoreScreen();
		break;

		case TEST:
			UpdateTestScene();
		break;

		case FLOCKING_TEST:
			UpdateFlockingScene();
		break;

		default:
		break;
	}
}

void Draw()
{
	BeginDrawing();

	switch (GameState)
	{
		case SPLASH:
			DrawSplashScreen();
		break;

		case MENU:
			DrawMenuScreen();
		break;

		case GAME:
			DrawGame();
		break;

		case PAUSE:
			DrawPauseScreen();
		break;

		case DEATH:
			DrawDeathScreen();
		break;

		case WIN:
			DrawWinScreen();
		break;

		case LOSE:
			DrawLoseScreen();
		break;

		case STATS:
			DrawStatsScreen();
		break;

		case HIGHSCORE:
			DrawHighscoreScreen();
		break;

		case TEST:
			DrawTestScene();
		break;

		case FLOCKING_TEST:
			DrawFlockingScene();
		break;

		default:
		break;
	}

	EndDrawing();
}

void UpdateSplashScreen()
{
	FramesCounter++;

	if (((FramesCounter/360)%2) == 1)
		SetGameState(MENU);
}

void UpdateMenuScreen()
{	
	if (IsKeyPressed(KEY_ENTER))
	{
		ResetGame();
		SetGameState(GAME);
	}

	SetExitKey(KEY_ESCAPE);
}

void UpdateGame()
{
	FramesCounter++;
	SetExitKey(KEY_ESCAPE);
	HideCursor();

	// Stars
	for (int i = 0; i < MAX_SMALL_STARS; i++)
	{
		SmallStars[i].Update();
		FastStars[i].Update();
		CloseStars[i].Update();
	}

	// Player
	Player.Update();

	// Enemy Manager system
	EnemyManager.Update();

	if (IsKeyPressed(KEY_ESCAPE))
		SetGameState(PAUSE);
}

void UpdatePauseScreen()
{
	if (IsKeyPressed(KEY_ESCAPE))
		SetGameState(GAME);
}

void UpdateDeathScreen()
{
	SetExitKey(0);

	if (IsKeyPressed(KEY_ENTER))
	{
		ResetGame();
		SetGameState(GAME);
	}

	if (IsKeyPressed(KEY_ESCAPE))
	{
		ResetGame();
		SetGameState(MENU);
	}
}

void UpdateWinScreen()
{
}

void UpdateLoseScreen()
{
}

void UpdateStatsScreen()
{
}

void UpdateHighscoreScreen()
{
}

void UpdateTestScene()
{
	FramesCounter++;
	SetExitKey(KEY_ESCAPE);


	if (IsKeyPressed(KEY_SPACE))
		Pattern<BulletType>.bRelease = true;

	Pattern<BulletType>.Location = {GetMousePosition().x, GetMousePosition().y};
	Pattern<BulletType>.Update();


	//Warship.Update();
}

void UpdateFlockingScene()
{
	SetExitKey(KEY_ESCAPE);

	Flock.Update();

	//Ship.Update();
}

void DrawSplashScreen()
{
	ClearBackground(WHITE);

	DrawText("Splash Screen!!", GetScreenWidth()/2 - MeasureText("Splash Screen!!", 20), GetScreenHeight()/2 - MeasureText("Splash Screen!!", 5), 40, RED);
}

void DrawGame()
{
	ClearBackground(LIGHTBLACK);
	
	DrawGameUI();

	DrawFPS(10, 100);

	// Stars
	for (int i = 0; i < MAX_SMALL_STARS; i++)
	{
		SmallStars[i].Draw();
		CloseStars[i].Draw();
		FastStars[i].Draw();
	}

	// Player sprite
	Player.Draw();

	// Enemy
	EnemyManager.Draw();

	Player.DrawBullets();

	if (bDebug)
		DrawDebugInfo();
}

void DrawGameUI()
{
	DrawText(FormatText("Health: %02i", Player.Health), 10, 10, 20, GREEN);
	DrawText(FormatText("Enemies Killed: %02i", Player.EnemiesKilled), 10, 30, 20, RED);
}

void DrawMenuScreen()
{
	ClearBackground(WHITE);

	DrawText("Press [Enter] to play", GetScreenWidth()/2 - MeasureText("Press [Enter] to play", 10), GetScreenHeight()/2 - 30, 20, RED);
	DrawText("Press [ESC] to quit", GetScreenWidth()/2 - MeasureText("Press [Enter] to quit", 10), GetScreenHeight()/2, 20, RED);
}

void DrawPauseScreen()
{
	ClearBackground(WHITE);

	DrawText("Game Paused", GetScreenWidth()/2 - MeasureText("Game Paused", 10), GetScreenHeight()/2 - 30, 20, RED);
}

void DrawDeathScreen()
{
	ClearBackground(WHITE);

	DrawText("YOU FAILED", GetScreenWidth()/2 - MeasureText("YOU FAILED", 39), GetScreenHeight()/2 - MeasureText("YOU FAILED", 20), 80, RED);

	DrawText("Press [Enter] to try again", GetScreenWidth()/2 - MeasureText("Press [Enter] to try again", 10), GetScreenHeight()/2 - 30, 20, RED);
	DrawText("Press [ESC] to enter main menu", GetScreenWidth()/2 - MeasureText("Press [ESC] to enter main menu", 10), GetScreenHeight()/2, 20, RED);
}

void DrawWinScreen()
{
}

void DrawLoseScreen()
{
}

void DrawStatsScreen()
{
}

void DrawHighscoreScreen()
{
}

void DrawTestScene()
{
	ClearBackground(BLACK);
	DrawFPS(10, 170);
	DrawText("Testing Playground!", 10, 10, 20, WHITE);
	
	//Warship.Draw();

	Pattern<BulletType>.Draw();
}

void DrawFlockingScene()
{
	ClearBackground(BLACK);
	DrawFPS(10, 40);
	DrawText("Flocking Simulation!", 10, 10, 20, WHITE);

	Flock.Draw();

	//Ship.Draw();
}

void DrawDebugInfo()
{
	// DEBUG frames counter
	DrawText(FormatText("%i", FramesCounter), 10, 50, 20, ORANGE);
}

void ResetGame()
{	
	InitGame();
}

void UnloadTextures()
{
	ASSETS.UnloadSprites();
}
