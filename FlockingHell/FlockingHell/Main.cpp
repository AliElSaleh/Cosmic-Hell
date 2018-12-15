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

	CloseWindow();

    return 0;
}

// Function Definitions
//-----------------------------------------
/// Initialise application
void Init()
{
	// Window setup
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flocking Hell");
	SetTargetFPS(120);

	GameState = SPLASH;

	// Player setup
	Player.Location.x = (float)SCREEN_WIDTH / 2;
	Player.Location.y = (float)SCREEN_HEIGHT - 100;
	Player.bIsDead = false;
	Player.Size = 20.0f;
	Player.Health = 100;

	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		Player.Bullet[i].Location = Player.Location;
		Player.Bullet[i].Radius = 3.0f;
		Player.Bullet[i].Speed = 500.0f;
		Player.Bullet[i].Damage = GetRandomValue(20, 40);
		Player.Bullet[i].bActive = false;
	}

	// Bullet setup
	for (int i = 0; i < MAX_SMALL_BULLETS; i++)
	{
		Bullet[i].Spacing = 35;
		Bullet[i].Location.x = i * Bullet[i].Spacing + Player.Size;
		Bullet[i].Location.y = 0;
		Bullet[i].Speed = 200.0f;
		Bullet[i].Radius = 5.0f;
		Bullet[i].Damage = GetRandomValue(10, 15);
		Bullet[i].bActive = true;
		Bullet[i].bIsHit = false;
		BulletLocation[i] = Bullet[i].Location;
	}

	// Star setup
	for (int i= 0; i <MAX_SMALL_STARS; i++)
	{
		SmallStars[i].Location.x = (float)GetRandomValue(0, SCREEN_WIDTH);
		SmallStars[i].Location.y = (float)GetRandomValue(0, SCREEN_HEIGHT);
		SmallStars[i].Radius = 1.0f;
		SmallStars[i].Speed = 100.0f;
	}

	bShowDebugHit = true;
	bDebug = false;
}

/// Update game logic
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

/// Render objects
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

void MovePlayer()
{
	if (!Player.bIsDead)
		Player.Location = GetMousePosition();
}

void CheckWindowCollision()
{
	// Player Collision check
	if (Player.Location.x + Player.Size > GetScreenWidth())
		Player.Location.x = GetScreenWidth() - Player.Size;
	
	if (Player.Location.y + Player.Size > GetScreenHeight())
		Player.Location.y = GetScreenHeight() - Player.Size;

	if (Player.Location.x - Player.Size < 0)
		Player.Location.x = Player.Size;
	
	if (Player.Location.y - Player.Size < 0)
		Player.Location.y = Player.Size;
}

void UpdateBullet()
{
	for (int i = 0; i < MAX_SMALL_BULLETS; i++)
		Bullet[i].Location.y += Bullet[i].Speed * GetFrameTime();
}

void CheckBulletCollision()
{
	// Player bullet collision with enemy bullets
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		for (int j = 0; j < MAX_SMALL_BULLETS; j++)
		{
			if (CheckCollisionCircles(Player.Bullet[i].Location, Player.Bullet[i].Radius, Bullet[j].Location, Bullet[j].Radius))
				if (!Bullet[j].bIsHit)
					Bullet[j].bActive = false;
		}
	}

	// Enemy bullet collision with player
	for (int i = 0; i < MAX_SMALL_BULLETS; i++)
	{
		if (CheckCollisionCircles(Player.Location, Player.Size / 2, Bullet[i].Location, Bullet[i].Radius) && !Player.bIsDead)
		{
			if (!Bullet[i].bIsHit)
			{
				// Apply damage to player
				if (Bullet[i].bActive)
				{
					Player.Health -= Bullet[i].Damage;
					Bullet[i].bIsHit = true;
					Bullet[i].bActive = false;
					Player.bIsHit = true;
				}
			}
		}
	}
}

void CheckBulletOutsideWindow()
{
	// Player Bullets
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if (Player.Bullet[i].Location.y - Player.Bullet[i].Radius < 0)
		{
			Player.Bullet[i].bActive = false;
			Player.Bullet[i].Location = Player.Location;
			ShootRate = 0;
		}
	}

	// Enemy bullets
	if (((FramesCounter/60)%2) == 1)
    {   
		for (int i = 0; i < MAX_SMALL_BULLETS; i++)
		{
			if (Bullet[i].Location.y - Bullet[i].Radius > SCREEN_HEIGHT)
			{
				Bullet[i].Location = BulletLocation[i];
				Bullet[i].bActive = true;
			}
		}

		FramesCounter = 0;
    }
}

void CheckPlayerHealth()
{
	if (Player.Health <= 0)
	{
		// Prevent negative health values
		Player.Health = 0;
		Player.bIsDead = true;
		GameState = DEATH;
	}
}

void UpdatePlayerBullet()
{
	// Initialise bullets
	if (IsKeyDown(KEY_SPACE))
	{
		if (!Player.bIsDead)
		{
			ShootRate += 2;

	        for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
	        {
	            if (!Player.Bullet[i].bActive && ShootRate % 20 == 0)
	            {
	                Player.Bullet[i].Location = Player.Location;
					Player.Bullet[i].Damage = GetRandomValue(20, 40);
	                Player.Bullet[i].bActive = true;
	                break;
	            }
			}
		}
	}

	// Apply movement to bullets when they become active
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
    {
        if (Player.Bullet[i].bActive)
        {
            // Bullet movement
            Player.Bullet[i].Location.y -= Player.Bullet[i].Speed * GetFrameTime();
		}
	}
}

void UpdateStars()
{
	for (int i = 0; i < MAX_SMALL_STARS; i++)
		SmallStars[i].Location.y += SmallStars[i].Speed * GetFrameTime();
}

void CheckStarsOutsideWindow()
{
	for (int i = 0; i < MAX_SMALL_STARS; i++)
	{
		if (SmallStars[i].Location.y - SmallStars[i].Radius > GetScreenHeight())
			SmallStars[i].Location.y = SmallStars[i].Radius;
	}
}

void UpdateSplashScreen()
{
	FramesCounter++;

	if (((FramesCounter/360)%2) == 1)
		GameState = MENU;
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

	// Player
	MovePlayer();
	UpdatePlayerBullet();
	CheckWindowCollision();
	CheckPlayerHealth();


	// Bullet
	UpdateBullet();
	CheckBulletCollision();
	CheckBulletOutsideWindow();


	// Stars
	UpdateStars();
	CheckStarsOutsideWindow();


	if (IsKeyPressed(KEY_ESCAPE))
		GameState = PAUSE;
}

void DrawGame()
{
	ClearBackground(LIGHTBLACK);
	
	//------------------------------------------------------------------------------------------------------------------------------------
	DrawGameUI();

	DrawStars();

	DrawPlayer();

	DrawEnemyBullets();

	if (bDebug)
		DrawDebugInfo();
	//------------------------------------------------------------------------------------------------------------------------------------
}

void UpdateMenuScreen()
{	
	if (IsKeyPressed(KEY_ENTER))
	{
		ResetGame();
		GameState = GAME;
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
		GameState = GAME;
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
		GameState = GAME;
	}

	if (IsKeyPressed(KEY_ESCAPE))
	{
		ResetGame();
		GameState = MENU;
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
	// Player setup
	Player.Location.x = (float)SCREEN_WIDTH / 2;
	Player.Location.y = (float)SCREEN_HEIGHT - 100;
	Player.bIsDead = false;
	Player.Health = 100;

	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		Player.Bullet[i].Location = Player.Location;
		Player.Bullet[i].Radius = 3.0f;
		Player.Bullet[i].Speed = 500.0f;
		Player.Bullet[i].Damage = GetRandomValue(20, 40);
		Player.Bullet[i].bActive = false;
	}

	// Bullet setup
	for (int i = 0; i < MAX_SMALL_BULLETS; i++)
	{
		Bullet[i].Spacing = 35;
		Bullet[i].Location.x = i * Bullet[i].Spacing + Player.Size;
		Bullet[i].Location.y = 0;
		Bullet[i].Speed = 200.0f;
		Bullet[i].Radius = 5.0f;
		Bullet[i].Damage = GetRandomValue(10, 15);
		Bullet[i].bActive = true;
		Bullet[i].bIsHit = false;
		BulletLocation[i] = Bullet[i].Location;
	}


	// Star setup
	for (int i= 0; i <MAX_SMALL_STARS; i++)
	{
		SmallStars[i].Location.x = (float)GetRandomValue(0, SCREEN_WIDTH);
		SmallStars[i].Location.y = (float)GetRandomValue(0, SCREEN_HEIGHT);
		SmallStars[i].Radius = 1.0f;
		SmallStars[i].Speed = 100.0f;
	}
}

void DrawGameUI()
{
	//DrawFPS(10, 10);
	DrawText(FormatText("Health: %02i", Player.Health), 10, 10, 20, GREEN);
}

void DrawStars()
{
	// Small Stars
	for (int i = 0; i < MAX_SMALL_STARS; i++)
		DrawCircle((int)SmallStars[i].Location.x, (int)SmallStars[i].Location.y, SmallStars[i].Radius, WHITE);
}

void DrawPlayer()
{
	// Player bullets
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
		if (Player.Bullet[i].bActive)
			DrawCircleGradient((int)Player.Bullet[i].Location.x, (int)Player.Bullet[i].Location.y, Player.Bullet[i].Radius, VIOLET, RED);

	// Player
	DrawCircleGradient((int)Player.Location.x, (int)Player.Location.y, Player.Size, RED, BLUE);
}

void DrawEnemyBullets()
{
	// Small bullets
	for (int i = 0; i < MAX_SMALL_BULLETS; i++)
		if (Bullet[i].bActive)
			DrawCircleGradient((int)Bullet[i].Location.x, (int)Bullet[i].Location.y, Bullet[i].Radius, WHITE, RED);
}

void DrawDebugInfo()
{
	// DEBUG enemy bullet hit with player
	for (int i = 0; i < MAX_SMALL_BULLETS; i++)
	{
		if (!Bullet[i].bActive && Bullet[i].bIsHit && bShowDebugHit)
		{	
			DrawText("Hit", 10, 80, 20, RED);
			Bullet[i].bIsHit = false;
		}
	}

	// DEBUG frames counter
	DrawText(FormatText("%i", FramesCounter), 10, 50, 20, ORANGE);
}
