#include "Globals.h"
#include "Main.h"
#include "Assets.h"

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)
#define GetFont(Name) ASSETS.GetFont(#Name)

int main()
{
	Init();
	
	// Main game loop
	while (!WindowShouldClose())
	{
		Update();
		Draw();
	}

	Unload();

	CloseWindow();

    return 0;
}

// Function Definitions
//-----------------------------------------

void Init()
{
	// Window setup
	const int SCREEN_WIDTH = 700+PANEL_WIDTH; // From 700 to 1050 on X axis is reserved for player stats
	const int SCREEN_HEIGHT = 800;
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cosmic Hell");
	SetTargetFPS(120);

	SetGameState(GAME);

	ASSETS.LoadSprites();
	ASSETS.LoadFonts();

	// Enemy Manager
	EnemyManager.AddEnemy(new Demon());
	EnemyManager.AddEnemy(new Warship());

	InitGame();
}

void InitGame()
{
	switch (GameState)
	{
		case TEST:
			QT->Init();
		break;

		case FLOCKING_TEST:
			for (int i = 0; i < 50; i++)
				Flock.Boids.emplace_back(new Ship());

			Flock.Init();
		break;

		case GAME:
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

			Panel = GetAsset(Panel);

			for (int i = 0; i < 6; ++i)
				Heart[i] = GetAsset(Heart);
		
			for (int i = 0; i < 4; ++i)
				Bomb[i] = GetAsset(Bomb);

			bDebug = false;
		break;

		default:
		break;
	}
}

void SetGameState(const State State)
{
	GameState = State;
}

void Update()
{
	SetWindowTitle(FormatText("Cosmic Hell | FPS: %02i", GetFPS()));

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
		SetGameState(GAME);
		InitGame();
	}

	SetExitKey(KEY_ESCAPE);
}

void UpdateGame()
{
	FramesCounter++;
	SetExitKey(KEY_ESCAPE);
	HideCursor();

	if (GetMousePosition().x > GetScreenWidth()-PANEL_WIDTH)
		ShowCursor();

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

	// TODO: Reset demon to start location

	if (IsKeyPressed(KEY_ENTER))
	{
		SetGameState(GAME);
		InitGame();
	}

	if (IsKeyPressed(KEY_ESCAPE))
	{
		SetGameState(MENU);
		InitGame();
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

	QT->Update();

	//if (IsKeyPressed(KEY_SPACE))
	//	Pattern<BulletType>.bRelease = true;

	//Pattern<BulletType>.Location = {GetMousePosition().x, GetMousePosition().y};
	//Pattern<BulletType>.Update();
}

void UpdateFlockingScene()
{
	SetExitKey(KEY_ESCAPE);

	Flock.Update();
}

void DrawSplashScreen()
{
	ClearBackground(WHITE);

	DrawText("Splash Screen!!", GetScreenWidth()-PANEL_WIDTH - 300, GetScreenHeight()/2 - 50, 40, RED);
}

void DrawGame()
{
	ClearBackground(LIGHTBLACK);

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

	DrawTexture(Panel, GetScreenWidth()-PANEL_WIDTH, 0, WHITE);
 	DrawTexture(GetAsset(Divider), GetScreenWidth()-PANEL_WIDTH-5, 0, WHITE);
	DrawFPS(GetScreenWidth()-80, GetScreenHeight() - 25);
	DrawGameUI();

	if (bDebug)
		DrawDebugInfo();
}

void DrawGameUI()
{
	unsigned short Spacing  = 0;

	for (int i = 0; i < 6; i++)
	{
		DrawTexture(Heart[i], GetScreenWidth()-PANEL_WIDTH + 140 + Spacing, 436, WHITE);
		Spacing += HeartSpriteSpacing;
	}

	Spacing = 0;
	
	for (int i = 0; i < 4; i++)
	{
		DrawTexture(Bomb[i], GetScreenWidth()-PANEL_WIDTH + 154 + Spacing, 487, WHITE);
		Spacing += 40;
	}

	Spacing = 0;

	unsigned int Highscore = 1758240;

	unsigned int OutputScore = Highscore;

	for (int i = 0; i < 3; i++)
	{
		if (Highscore > 999999)
		{
			Highscore = 999999;
			OutputScore = Highscore;
		}

		DrawTextEx(GetFont(VCR), FormatText("%06i", OutputScore), {float(GetScreenWidth()-PANEL_WIDTH+211), 223.0f + Spacing}, 31.0f, 0.0f, WHITE);

		// Grazing
		if (i > 0)
		{
			OutputScore = Player.GrazingScore;
			Spacing = GrazingScorePosition;
		}
		// Score
		else
		{
			OutputScore = Player.Score;
			Spacing = ScorePosition;
		}
	}
}

void DrawMenuScreen()
{
	ClearBackground(WHITE);

	DrawText("Press [Enter] to play", GetScreenWidth()-PANEL_WIDTH/2 - MeasureText("Press [Enter] to play", 10) - 350, GetScreenHeight()/2 - 30, 20, RED);
	DrawText("Press [ESC] to quit", GetScreenWidth()-PANEL_WIDTH/2 - MeasureText("Press [Enter] to quit", 10) - 350, GetScreenHeight()/2, 20, RED);
}

void DrawPauseScreen()
{
	ClearBackground(WHITE);

	DrawText("Game Paused", GetScreenWidth()-PANEL_WIDTH/2 - MeasureText("Game Paused", 10), GetScreenHeight()/2 - 30, 20, RED);
}

void DrawDeathScreen()
{
	ClearBackground(WHITE);

	DrawText("YOU FAILED", GetScreenWidth()-PANEL_WIDTH/2 - MeasureText("YOU FAILED", 39) - 350, GetScreenHeight()/2 - MeasureText("YOU FAILED", 20), 80, RED);

	DrawText("Press [Enter] to try again", GetScreenWidth()-PANEL_WIDTH/2 - MeasureText("Press [Enter] to try again", 10) - 350, GetScreenHeight()/2 - 30, 20, RED);
	DrawText("Press [ESC] to enter main menu", GetScreenWidth()-PANEL_WIDTH/2 - MeasureText("Press [ESC] to enter main menu", 10) - 350, GetScreenHeight()/2, 20, RED);
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
	DrawText("Testing Playground!", GetScreenWidth()-PANEL_WIDTH + 10, 20, 20, WHITE);

	//DrawLine(700, 0, 700, 800, WHITE);

	QT->Draw();

	//Pattern<BulletType>.Draw();
}

void DrawFlockingScene()
{
	ClearBackground(BLACK);
	DrawFPS(10, 40);
	DrawText("Flocking Simulation!", 10, 10, 20, WHITE);

	Flock.Draw();
}

void DrawDebugInfo()
{
	// DEBUG frames counter
	DrawText(FormatText("%i", FramesCounter), 10, 50, 20, ORANGE);
}

void Unload()
{
	ASSETS.UnloadSprites();
	ASSETS.UnloadFonts();
}
