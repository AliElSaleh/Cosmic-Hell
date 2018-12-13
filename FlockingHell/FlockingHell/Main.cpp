#include "raylib.h"
#include "Player.h"
#include "Bullet.h"

#define SMALL_BULLETS 20

// Variable Declarations
//-----------------------------------------
struct Player Player;
//struct Bullet PlayerBullet;
struct Bullet Bullet[SMALL_BULLETS];
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 800;
bool bShowDebugHit = false;
bool bKeyPressed = false;
float ElapsedTime = 0.0f;

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
	SetTargetFPS(200);

	// Player setup
	Player.Location.x = (float)SCREEN_WIDTH / 2;
	Player.Location.y = (float)SCREEN_HEIGHT - 100;
	Player.Size = 20.0f;
	Player.Health = 100;

	for (int i = 0; i < 100; i++)
	{
		Player.Bullet[i].Location = GetMousePosition();
		Player.Bullet[i].Damage = 5;
		Player.Bullet[i].Radius = 3.0f;
		Player.Bullet[i].Speed = 500;
	}


	// Bullet setup
	for (int i = 0; i < SMALL_BULLETS; i++)
	{
		Bullet[i].Location.x = i * Player.Size * 2;
		Bullet[i].Location.y = 50;
		Bullet[i].Radius = 5.0f;
		Bullet[i].Damage = GetRandomValue(5, 10);
	}

	bShowDebugHit = true;
}

/// Update game logic
void Update()
{
	// Player
	MovePlayer();
	UpdatePlayerBullet();
	CheckWindowCollision();
	CheckPlayerHealth();

	// Bullet
	UpdateBullet();
	CheckBulletCollision();
	CheckBulletOutsideWindow();
}

/// Render objects
void Draw()
{
	BeginDrawing();

	ClearBackground(LIGHTBLACK);

	//------------------------------------------------------------------------------------------------------------------------------------
	// Text
	DrawFPS(10, 10);
	DrawText(FormatText("Health: %02i", Player.Health), 10, 30, 20, GREEN); 

	// Player
	for (int i = 0; i < 100; i++)
		DrawCircleGradient((int)Player.Bullet[i].Location.x, (int)Player.Bullet[i].Location.y, Player.Bullet[i].Radius, VIOLET, RED);

	DrawCircleGradient((int)Player.Location.x, (int)Player.Location.y, Player.Size, RED, BLUE);

	// Bullets
	for (int i = 0; i < SMALL_BULLETS; i++)
		DrawCircleGradient((int)Bullet[i].Location.x, (int)Bullet[i].Location.y, Bullet[i].Radius, WHITE, RED);

	// DEBUG
	for (int i = 0; i < SMALL_BULLETS; i++)
	{
		if (CheckCollisionCircles(Player.Location, Player.Size / 2, Bullet[i].Location, Bullet[i].Radius) && !Player.bIsDead && bShowDebugHit)
			DrawText("Hit", 10, 60, 20, RED);
	}

	DrawText(FormatText("Elapsed Time: %02i", GetTime()), 10, 50, 20, BLUE);
	//------------------------------------------------------------------------------------------------------------------------------------
	
	EndDrawing();
}

void MovePlayer()
{
	if (!Player.bIsDead)
	{
		Player.Location = GetMousePosition();
	}
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
	
	if (Player.Location.y - Player.Size < 100)
		Player.Location.y = Player.Size + 100;
}

void UpdateBullet()
{
	for (int i = 0; i < SMALL_BULLETS; i++)
		Bullet[i].Location.y += 1;
}

void CheckBulletCollision()
{
	for (int i = 0; i < SMALL_BULLETS; i++)
	{
		if (CheckCollisionCircles(Player.Location, Player.Size / 2, Bullet[i].Location, Bullet[i].Radius) && !Player.bIsDead)
		{
			if (!Bullet[i].bIsHit)
			{
				// Apply damage to player
				Player.Health -= Bullet[i].Damage;
				Bullet[i].bIsHit = true;
				Player.bIsHit = true;
			}
		}
	}
}

void CheckBulletOutsideWindow()
{
	for (int i = 0; i < 100; i++)
	{
		if (Player.Bullet[i].Location.y - Player.Bullet[i].Radius < 0)
		{
			bKeyPressed = false;
			Player.Bullet[i].Location = Player.Location;
		}
	}
}

void CheckPlayerHealth()
{
	if (Player.Health <= 0)
	{
		// Prevent negative health values
		Player.Health = 0;
		Player.bIsDead = true;
	}
}

void UpdatePlayerBullet()
{
	if (!Player.bIsDead)
	{
		if (IsKeyDown(KEY_SPACE))
		{
			bKeyPressed = true;
		}

		for (int i = 0; i < 100; i++)
		{
			if (bKeyPressed)
				Player.Bullet[i].Location.y -= Player.Bullet[i].Speed * GetFrameTime();
			else
				Player.Bullet[i].Location = Player.Location;	
		}
	}
}
