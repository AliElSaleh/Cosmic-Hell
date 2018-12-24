#include "PulseBullet.h"
#include <raymath.h>

void PulseBullet::Init()
{
	SpawnLocation = {float(GetScreenWidth())/2, float(GetScreenHeight())/2};

	for (int i = 0; i < MAX_PULSE_BULLETS; i++)
	{
		Bullet[i].Init();
		Bullet[i].Location = SpawnLocation;
	}

	AmountToSpawn = 50;
	LoopAmount = 0;
	Speed = 200.0f;
	CircleRadius = 70.0f;
	SpawnOffset = 0;
	Spacing = CircleRadius - AmountToSpawn - PI;

	bKeyPressed = false;
	bDebug = true;

	if (AmountToSpawn > MAX_PULSE_BULLETS)
		AmountToSpawn = MAX_PULSE_BULLETS;
	else if (AmountToSpawn < 4)
		AmountToSpawn = 4;

	for (int i = 0; i < AmountToSpawn; i++)
	{	
		// Initialise spawn points
		SpawnPoint[i].x = SpawnLocation.x + CircleRadius * cosf(SpawnOffset*DEG2RAD);
		SpawnPoint[i].y = SpawnLocation.y + CircleRadius * sinf(SpawnOffset*DEG2RAD);

		SpawnOffset += Spacing;

		// Form a circle of bullets
		Bullet[i].Location = SpawnPoint[i];

		// Calculate their directions
		Direction[i] = Vector2Subtract(SpawnLocation, Bullet[i].Location);
		Direction[i] = Vector2Normalize(Direction[i]);
	}
}

void PulseBullet::Update()
{
	if (IsKeyPressed(KEY_SPACE))
	{
		if (!bKeyPressed)
			bKeyPressed = true;
		else
			bKeyPressed = false;
	}

	if (bKeyPressed)
		for (int i = 0; i < AmountToSpawn; i++)
		{
			if (Bullet[i].bActive)
			{
				Bullet[i].Location.x += Direction[i].x * Speed * GetFrameTime();
				Bullet[i].Location.y += Direction[i].y * Speed * GetFrameTime();		
			}
		}

	if (LoopAmount < 4)
		CheckBulletsOutsideWindow();

	if (Bullet[0].Location.x == SpawnPoint[0].x && Bullet[0].Location.y == SpawnPoint[0].y && bKeyPressed)
	{
		LoopAmount++;
	}
}

void PulseBullet::Draw() const
{
	for (int i = 0; i < AmountToSpawn; i++)
	{
		if (Bullet[i].bActive)
			DrawTexture(Bullet[i].Sprite, Bullet[i].Location.x - Bullet[i].Radius, Bullet[i].Location.y - Bullet[i].Radius, WHITE); // Bullets
	}

	if (bDebug)
	{
		DrawCircleLines(SpawnLocation.x, SpawnLocation.y, CircleRadius, WHITE); // White circle outline

		for (int i = 0; i < AmountToSpawn; i++)
			DrawCircle(SpawnPoint[i].x, SpawnPoint[i].y, 2.0f, BLUE); // SpawnPoints on circle

		DrawText(FormatText("Loop: %i", LoopAmount), 10, 40, 20, WHITE);
	}
}

void PulseBullet::CheckBulletsOutsideWindow()
{
	for (int i = 0; i < AmountToSpawn; i++)
	{
		if (Bullet[i].Location.x - Bullet[i].Radius > GetScreenWidth())
		{
			Bullet[i].Location = SpawnPoint[i];

			//Direction[i].x = -Direction[i].x;
			//Direction[i].y = -Direction[i].y;
		}

		if (Bullet[i].Location.y - Bullet[i].Radius > GetScreenHeight())
		{
			Bullet[i].Location = SpawnPoint[i];
			
			//Direction[i].x = -Direction[i].x;
			//Direction[i].y = -Direction[i].y;
		}

		if (Bullet[i].Location.x + Bullet[i].Radius < 0)
		{
			Bullet[i].Location = SpawnPoint[i];
			
			//Direction[i].x = -Direction[i].x;
			//Direction[i].y = -Direction[i].y;
		}

		if (Bullet[i].Location.y + Bullet[i].Radius < 0)
		{
			Bullet[i].Location = SpawnPoint[i];
			
			//Direction[i].x = -Direction[i].x;
			//Direction[i].y = -Direction[i].y;
		}
	}
}

