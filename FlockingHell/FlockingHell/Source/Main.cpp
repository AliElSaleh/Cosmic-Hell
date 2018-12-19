#include "Main.h"

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
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flocking Hell");
	SetTargetFPS(120);

	SetGameState(GAME);

	InitGame();
}

void InitGame()
{
	// Initialise Player
	Player.Init();
	Player.GameState = &GameState;

	Bullets.reserve(WAVE_ONE_BULLETS + WAVE_TWO_BULLETS);

	// Initialise enemy bullets
	for (int i = 0; i < WAVE_ONE_BULLETS; i++)
	{
		BulletW1[i].Player = &Player;
		BulletW1[i].InitArray(i);
		Bullets.push_back(BulletW1[i]);
	}

	BulletWaves.push_back(Bullets);
	Bullets.erase(Bullets.begin(), Bullets.end());

	for (int i = 0; i < WAVE_TWO_BULLETS; i++)
	{
		BulletW2[i].Player = &Player;
		BulletW2[i].InitArray(i);
		BulletW2[i].InitWave(2);
		Bullets.push_back(BulletW2[i]);
	}

	BulletWaves.push_back(Bullets);

	// Initialise stars
	for (int i= 0; i <MAX_SMALL_STARS; i++)
		SmallStars[i].Init();

	bShowDebugHit = true;
	bDebug = false;
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

void DrawSplashScreen()
{
	ClearBackground(WHITE);

	DrawText("Splash Screen!!", SCREEN_WIDTH/2 - MeasureText("Splash Screen!!", 20), SCREEN_HEIGHT/2 - MeasureText("Splash Screen!!", 5), 40, RED);
}

void UpdateGame()
{
	FramesCounter++;
	SetExitKey(KEY_ESCAPE);
	HideCursor();

	// Player
	Player.Update();

	// Enemy Bullets
	if (!BulletWaves[0][0].IsOutsideWindow())
		for (int i = 0; i < WAVE_ONE_BULLETS; i++)
			BulletWaves[0][i].Update();

	if (BulletWaves[0][0].IsLocationYGreaterThan(500))
		for (int i = 0; i < WAVE_TWO_BULLETS; i++)
			BulletWaves[1][i].Update();

	//else
	//{
	//	if (BulletWaves[0][0].IsOutsideWindow())
	//		for (int i = 0; i < WAVE_ONE_BULLETS; i++)
	//		{
	//			BulletWaves[0][i].Location.y = 0;
	//			BulletWaves[0][i].bActive = true;
	//			BulletWaves[0][i].bIsHit = false;
	//		}
	//
	//	if (BulletWaves[0][0].IsOutsideWindow())
	//		for (int j = 0; j < WAVE_TWO_BULLETS; j++)
	//		{
	//			BulletWaves[1][j].Location.y = 0;
	//			BulletWaves[1][j].bActive = true;
	//			BulletWaves[1][j].bIsHit = false;
	//		}
	//}


	// Stars
	for (int i = 0; i < MAX_SMALL_STARS; i++)
		SmallStars[i].Update();


	if (IsKeyPressed(KEY_ESCAPE))
		SetGameState(PAUSE);
}

void DrawGame()
{
	ClearBackground(LIGHTBLACK);
	
	//------------------------------------------------------------------------------------------------------------------------------------
	DrawGameUI();

	// Stars
	for (int i = 0; i < MAX_SMALL_STARS; i++)
		SmallStars[i].Draw();

	// Player sprite
	Player.Draw();

	// Enemy bullets

	if (!BulletWaves[0][0].IsOutsideWindow())
		for (int i = 0; i < WAVE_ONE_BULLETS; i++)
			BulletWaves[0][i].Draw();
	
	if (BulletWaves[0][0].IsLocationYGreaterThan(500))
		for (int i = 0; i < WAVE_TWO_BULLETS; i++)
			BulletWaves[1][i].Draw();
		

	if (bDebug)
		DrawDebugInfo();
	//------------------------------------------------------------------------------------------------------------------------------------
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

void DrawMenuScreen()
{
	ClearBackground(WHITE);

	DrawText("Press [Enter] to play", SCREEN_WIDTH/2 - MeasureText("Press [Enter] to play", 10), SCREEN_HEIGHT/2 - 30, 20, RED);
	DrawText("Press [ESC] to quit", SCREEN_WIDTH/2 - MeasureText("Press [Enter] to quit", 10), SCREEN_HEIGHT/2, 20, RED);
}

void UpdatePauseScreen()
{
	if (IsKeyPressed(KEY_ESCAPE))
		SetGameState(GAME);
}

void DrawPauseScreen()
{
	ClearBackground(WHITE);

	DrawText("Game Paused", SCREEN_WIDTH/2 - MeasureText("Game Paused", 10), SCREEN_HEIGHT/2 - 30, 20, RED);
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

void DrawDeathScreen()
{
	ClearBackground(WHITE);

	DrawText("YOU FAILED", SCREEN_WIDTH/2 - MeasureText("YOU FAILED", 39), SCREEN_HEIGHT/2 - MeasureText("YOU FAILED", 20), 80, RED);

	DrawText("Press [Enter] to try again", SCREEN_WIDTH/2 - MeasureText("Press [Enter] to try again", 10), SCREEN_HEIGHT/2 - 30, 20, RED);
	DrawText("Press [ESC] to enter main menu", SCREEN_WIDTH/2 - MeasureText("Press [ESC] to enter main menu", 10), SCREEN_HEIGHT/2, 20, RED);
}

void UpdateWinScreen()
{
}

void DrawWinScreen()
{
}

void UpdateLoseScreen()
{
}

void DrawLoseScreen()
{
}

void UpdateStatsScreen()
{
}

void DrawStatsScreen()
{
}

void UpdateHighscoreScreen()
{
}

void DrawHighscoreScreen()
{
}

void ResetGame()
{
	if (!BulletWaves.empty())
		BulletWaves.erase(BulletWaves.begin(), BulletWaves.end());
	
	InitGame();
}

void DrawGameUI()
{
	//DrawFPS(10, 10);
	DrawText(FormatText("Health: %02i", Player.Health), 10, 10, 20, GREEN);
}

void DrawDebugInfo()
{
	// DEBUG enemy bullet hit with player
	for (int i = 0; i < WAVE_ONE_BULLETS; i++)
	{
		if (!BulletW1[i].bActive && BulletW1[i].bIsHit && bShowDebugHit)
		{	
			DrawText("Hit", 10, 80, 20, RED);
			BulletW1[i].bIsHit = false;
		}
	}

	// DEBUG frames counter
	DrawText(FormatText("%i", FramesCounter), 10, 50, 20, ORANGE);
}

void UnloadTextures()
{
	UnloadTexture(Player.PlayerSprite);
	UnloadTexture(Player.BulletSprite);
}

void SetGameState(const State State)
{
	GameState = State;
}