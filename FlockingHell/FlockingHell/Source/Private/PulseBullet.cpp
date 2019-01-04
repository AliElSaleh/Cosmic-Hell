#include "PulseBullet.h"
#include "Player.h"
#include <raymath.h>

void PulseBullet::Init()
{
	for (int i = 0; i < MAX_PULSE_BULLETS; i++)
		Bullet[i].Init();

	LoopAmount = 0;
	Speed = 300.0f;
	SpawnOffset = 0;
	Spacing = CircleRadius - AmountToSpawn - PI;

	bRelease = false;
	bDebug = true;

	if (AmountToSpawn > MAX_PULSE_BULLETS) // Max
		AmountToSpawn = MAX_PULSE_BULLETS;
	else if (AmountToSpawn < 4) // Min
		AmountToSpawn = 4;
	
	switch (BulletType)
	{
		case PULSE:
			// Clear entries
			for (int i = 0; i < MAX_PULSE_BULLETS; i++)
				Bullet[i].Clear();		

			// Initialise bullets
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
		break;

		case ONELOOP:
		break;

		case MULTILOOP:
			// Clear entries
			for (int i = 0; i < MAX_PULSE_BULLETS; i++)
				Bullet[i].Clear();

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
		break;

		case RAGE:
			// Clear entries
			for (int i = 0; i < MAX_PULSE_BULLETS; i++)
				Bullet[i].Clear();

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
		break;
	}
}

void PulseBullet::Update()
{
	switch (BulletType)
	{
		case PULSE:
			for (int i = 0; i < AmountToSpawn; i++)
			{
				SpawnPoint[i].x = SpawnLocation.x + CircleRadius * cosf(SpawnOffset*DEG2RAD);
				SpawnPoint[i].y = SpawnLocation.y + CircleRadius * sinf(SpawnOffset*DEG2RAD);
			
				SpawnOffset += Spacing;

				if (!bRelease)
				{
					// Set bullet location to spawn point location
					Bullet[i].Location = SpawnPoint[i];

					// Calculate each bullet's direction
					Direction[i] = Vector2Subtract(SpawnLocation, Bullet[i].Location);
					Direction[i] = Vector2Normalize(Direction[i]);
				}
			}

			if (IsKeyPressed(KEY_ENTER))
			{
				LoopAmount = 0;

				for (int i = 0; i < AmountToSpawn; i++)
					Bullet[i].Location = SpawnPoint[i];
			}

			if (bRelease)
				for (int i = 0; i < ReleaseAmount; i++)
				{
					// Movement
					Bullet[i].Location.x += -Direction[i].x * Bullet[i].Speed * GetFrameTime();
					Bullet[i].Location.y += -Direction[i].y * Bullet[i].Speed * GetFrameTime();

					// Center
					Bullet[i].Center.x = Bullet[i].Location.x + float(Bullet[i].Sprite.width)/8;
					Bullet[i].Center.y = Bullet[i].Location.y + float(Bullet[i].Sprite.height);

					// Collision hit box
					Bullet[i].CollisionOffset.x = Bullet[i].Location.x + Bullet[i].Radius - 1.0f;
					Bullet[i].CollisionOffset.y = Bullet[i].Location.y + Bullet[i].Radius + 0.5f;

					Bullet[i].CheckCollisionWithPlayer();					
				}

			if (LoopAmount < 10)
				CheckBulletWindowCollision();
			
			if (Bullet[0].Location.x == SpawnPoint[0].x && Bullet[0].Location.y == SpawnPoint[0].y && bRelease)
				LoopAmount++;
		break;

		case ONELOOP:
			for (int i = 0; i < AmountToSpawn; i++)
			{
				SpawnPoint[i].x = SpawnLocation.x + CircleRadius * cosf(SpawnOffset*DEG2RAD);
				SpawnPoint[i].y = SpawnLocation.y + CircleRadius * sinf(SpawnOffset*DEG2RAD);
			
				SpawnOffset += Spacing;

				if (!bRelease)
				{
					// Set bullet location to spawn point location
					Bullet[i].Location = SpawnPoint[i];

					// Calculate each bullet's direction
					Direction[i] = Vector2Subtract(SpawnLocation, Bullet[i].Location);
					Direction[i] = Vector2Normalize(Direction[i]);
				}
			}

			if (IsKeyPressed(KEY_ENTER))
			{
				LoopAmount = 0;

				for (int i = 0; i < AmountToSpawn; i++)
					Bullet[i].Location = SpawnPoint[i];
			}

			if (bRelease)
				for (int i = 0; i < ReleaseAmount; i++)
				{
					// Movement
					Bullet[i].Location.x += -Direction[i].x * Bullet[i].Speed * GetFrameTime();
					Bullet[i].Location.y += -Direction[i].y * Bullet[i].Speed * GetFrameTime();

					// Center
					Bullet[i].Center.x = Bullet[i].Location.x + float(Bullet[i].Sprite.width)/8;
					Bullet[i].Center.y = Bullet[i].Location.y + float(Bullet[i].Sprite.height);

					// Collision hit box
					Bullet[i].CollisionOffset.x = Bullet[i].Location.x + Bullet[i].Radius - 1.0f;
					Bullet[i].CollisionOffset.y = Bullet[i].Location.y + Bullet[i].Radius + 0.5f;

					Bullet[i].CheckCollisionWithPlayer();					
				}

			CheckBulletWindowCollision();
		break;

		case MULTILOOP:

			for (int i = 0; i < AmountToSpawn; i++)
			{
				SpawnPoint[i].x = SpawnLocation.x + CircleRadius * cosf(SpawnOffset*DEG2RAD);
				SpawnPoint[i].y = SpawnLocation.y + CircleRadius * sinf(SpawnOffset*DEG2RAD);
				
				SpawnOffset += Spacing;
				
				if (!bRelease)
				{
					Bullet[i].Location = SpawnPoint[i];

					Direction[i] = Vector2Subtract(SpawnLocation, Bullet[i].Location);
					Direction[i] = Vector2Normalize(Direction[i]);
				}
			}
			
			if (IsKeyPressed(KEY_ENTER))
			{
				LoopAmount = 0;

				for (int i = 0; i < AmountToSpawn; i++)
					Bullet[i].Location = SpawnPoint[i];
			}
			
			if (bRelease)
				for (int i = 0; i < AmountToSpawn; i++)
				{
					// Center
					Bullet[i].Center.x = Bullet[i].Location.x + float(Bullet[i].Sprite.width)/8;
					Bullet[i].Center.y = Bullet[i].Location.y + float(Bullet[i].Sprite.height);

					// Collision hit box
					Bullet[i].CollisionOffset.x = Bullet[i].Location.x + Bullet[i].Radius - 1.0f;
					Bullet[i].CollisionOffset.y = Bullet[i].Location.y + Bullet[i].Radius + 0.5f;

					// Movement
					Bullet[i].Location.x += -Direction[i].x * Bullet[i].Speed * GetFrameTime();
					Bullet[i].Location.y += -Direction[i].y * Bullet[i].Speed * GetFrameTime();

					Bullet[i].CheckCollisionWithPlayer();
				}
			
			if (LoopAmount < 4)
				CheckBulletWindowCollision();
			
			if (Bullet[0].Location.x == SpawnPoint[0].x && Bullet[0].Location.y == SpawnPoint[0].y && bRelease)
				LoopAmount++;
		break;

		case RAGE:
			for (int i = 0; i < AmountToSpawn; i++)
			{
				SpawnPoint[i].x = SpawnLocation.x + CircleRadius * cosf(SpawnOffset*DEG2RAD);
				SpawnPoint[i].y = SpawnLocation.y + CircleRadius * sinf(SpawnOffset*DEG2RAD);
			
				SpawnOffset += Spacing;

				if (!bRelease)
				{
					// Set bullet location to spawn point location
					Bullet[i].Location = SpawnPoint[i];

					// Calculate each bullet's direction
					Direction[i] = Vector2Subtract(SpawnLocation, Bullet[i].Location);
					Direction[i] = Vector2Normalize(Direction[i]);
				}
			}
		
			if (bRelease)
				for (int i = 0; i < AmountToSpawn; i++)
				{
					// Movement
					Bullet[i].Location.x += -Direction[i].x * Bullet[i].Speed * GetFrameTime();
					Bullet[i].Location.y += -Direction[i].y * Bullet[i].Speed * GetFrameTime();

					// Center
					Bullet[i].Center.x = Bullet[i].Location.x + float(Bullet[i].Sprite.width)/8;
					Bullet[i].Center.y = Bullet[i].Location.y + float(Bullet[i].Sprite.height);

					// Collision hit box
					Bullet[i].CollisionOffset.x = Bullet[i].Location.x + Bullet[i].Radius - 1.0f;
					Bullet[i].CollisionOffset.y = Bullet[i].Location.y + Bullet[i].Radius + 0.5f;

					Bullet[i].CheckCollisionWithPlayer();					
				}
		break;
		
		default:
		break;
	}
}

void PulseBullet::Draw() const
{
	for (int i = 0; i < AmountToSpawn; i++)
		DrawTexture(Bullet[i].Sprite, Bullet[i].Location.x - Bullet[i].Radius, Bullet[i].Location.y - Bullet[i].Radius, WHITE); // Bullets

	if (bDebug)
	{
		DrawCircleLines(SpawnLocation.x, SpawnLocation.y, CircleRadius, WHITE); // White circle outline

		for (int i = 0; i < AmountToSpawn; i++)
		{
			DrawCircle(SpawnPoint[i].x, SpawnPoint[i].y, 2.0f, BLUE); // SpawnPoints on circle
			DrawCircle(Bullet[i].CollisionOffset.x, Bullet[i].CollisionOffset.y, Bullet[i].Radius, WHITE);
		}

		switch (BulletType)
		{
			case PULSE:
				DrawText("Pulse MODE", 10, 70, 20, WHITE);
			break;

			case ONELOOP:
				DrawText(FormatText("Loop %i of %i", LoopAmount, 1), 10, 70, 20, WHITE);
			break;

			case MULTILOOP:
				DrawText(FormatText("Loop %i of %i", LoopAmount, 4), 10, 70, 20, WHITE);
			break;

			case RAGE:
				DrawText("RAGE MODE", 10, 70, 20, RED);
			break;
		}
	}
}

void PulseBullet::SetBulletType(TYPE BulletType)
{
	this->BulletType = BulletType;
}

bool PulseBullet::IsOutsideWindow() const
{
	for (const int i = 0; i < AmountToSpawn;)
	{
		return Bullet[i].Location.x - Bullet[i].Radius > GetScreenWidth() || 
			   Bullet[i].Location.x + Bullet[i].Radius < 0 || 
			   Bullet[i].Location.y - Bullet[i].Radius > GetScreenHeight() || 
			   Bullet[i].Location.y + Bullet[i].Radius < 0;
	}

	return false;
}

void PulseBullet::CheckBulletWindowCollision()
{
	switch (BulletType)
	{
		case PULSE:
			for (int i = 0; i < AmountToSpawn; i++)
			{
				if (Bullet[i].Location.x - Bullet[i].Radius > GetScreenWidth() || 
					Bullet[i].Location.x + Bullet[i].Radius < 0 ||
					Bullet[i].Location.y - Bullet[i].Radius > GetScreenHeight() ||
					Bullet[i].Location.y + Bullet[i].Radius < 0)
				{
					Bullet[i].Speed = 400.0f;
					Bullet[i].Location = SpawnPoint[i];

					Direction[i] = Vector2Negate(Direction[i]);
				}
			}
		break;

		case ONELOOP:
			for (int i = 0; i < AmountToSpawn; i++)
			{
				if (Bullet[i].Location.x - Bullet[i].Radius > GetScreenWidth() || 
					Bullet[i].Location.x + Bullet[i].Radius < 0 ||
					Bullet[i].Location.y - Bullet[i].Radius > GetScreenHeight() ||
					Bullet[i].Location.y + Bullet[i].Radius < 0)
				{
					Bullet[i].Speed = 400.0f;
					Bullet[i].Location = SpawnPoint[i];

					Direction[i] = Vector2Negate(Direction[i]);
				}
			}
		break;

		case MULTILOOP:
			for (int i = 0; i < AmountToSpawn; i++)
			{
				if (Bullet[i].Location.x - Bullet[i].Radius > GetScreenWidth() || 
					Bullet[i].Location.x + Bullet[i].Radius < 0 ||
					Bullet[i].Location.y - Bullet[i].Radius > GetScreenHeight() ||
					Bullet[i].Location.y + Bullet[i].Radius < 0)
				{
					Bullet[i].Speed = 500.0f;
					Bullet[i].Location = SpawnPoint[i];

					// Calculate direction to player
					Direction[i] = Vector2Subtract(Bullet[i].Player->Center, Bullet[i].Location);
					Direction[i] = Vector2Normalize(Direction[i]);

					Direction[i] = Vector2Negate(Direction[i]);
				}
			}
		break;

		case RAGE:
			for (int i = 0; i < AmountToSpawn; i++)
			{
				if (Bullet[i].Location.x - Bullet[i].Radius > GetScreenWidth() || 
					Bullet[i].Location.x + Bullet[i].Radius < 0 ||
					Bullet[i].Location.y - Bullet[i].Radius > GetScreenHeight() ||
					Bullet[i].Location.y + Bullet[i].Radius < 0)
				{
					Bullet[i].Speed = 300.0f;
					Bullet[i].Location = SpawnPoint[i];
					
					Direction[i] = Vector2Normalize(Direction[i]);
					Direction[i] = Vector2Negate(Direction[i]);
				}
			}
		break;

		default:
		break;
	}
}

