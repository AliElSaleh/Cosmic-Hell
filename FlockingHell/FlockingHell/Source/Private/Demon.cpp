#include "Demon.h"
#include "Player.h"
#include "Assets.h"

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

Demon::Demon()
{
	Demon::Init();
}

void Demon::Init()
{
	BulletSprite = GetAsset(RedBullet);
	Sprite = GetAsset(Demon);

	Location = {300.0f, 100.0f};
	SpawnLocation = {50.0f, 105.0f};
	HitboxOffset = {50.0f, 105.0f};
	Hitbox = {Location.x + HitboxOffset.x, Location.y + HitboxOffset.y, float(Sprite.width)/10 - 80, float(Sprite.height)/3};
	SpriteBox = {Location.x, Location.y, float(Sprite.width)/10, float(Sprite.height)};
	Health = 600;
	Speed = 120.0f;
	Damage = GetRandomValue(1, 3);
	ShootRate = 5;
	bActive = true;
	bIsDead = false;
	bDebug = false;

	DemonFrameRec.x = 0.0f;
	DemonFrameRec.y = 0.0f;
	DemonFrameRec.width = float(Sprite.width)/10; // 10 frames
	DemonFrameRec.height = float(Sprite.height);


	// Bullet wave setup
	for (int i = 0; i < MAX_PULSE_BULLETS; i++)
	{
		PulseBullet.Bullet[i].Sprite = BulletSprite;
		PulseBullet.Bullet[i].Player = Player;
		PulseBullet2ndWave.Bullet[i].Sprite = BulletSprite; // Probably use a different bullet sprite for each wave
		PulseBullet2ndWave.Bullet[i].Player = Player;
		PulseBullet2ndWave.Bullet[i].Speed = 400.0f; // Probably use a different bullet sprite for each wave
		PulseBullet3rdWave.Bullet[i].Sprite = BulletSprite;
		PulseBullet3rdWave.Bullet[i].Player = Player;
		PulseBullet3rdWave.Bullet[i].Speed = 400.0f;

		for (int j = 0; j < MAX_DEMON_RAGE_BULLETS; j++)
		{
			BulletRage[j].Bullet[i].Sprite = BulletSprite;
			BulletRage[j].Bullet[i].Player = Player;
			BulletRage[j].Bullet[i].Speed = 300.0f;
		}
	}
	
	PulseBullet.SetBulletType(PulseBullet::MULTILOOP);
	PulseBullet.AmountToSpawn = 30;
	PulseBullet.CircleRadius = 20.0f;
	PulseBullet.Init();

	PulseBullet2ndWave.SetBulletType(PulseBullet::PULSE);
	PulseBullet2ndWave.AmountToSpawn = 70;
	PulseBullet2ndWave.CircleRadius = 30.0f;
	PulseBullet2ndWave.Init();

	PulseBullet3rdWave.SetBulletType(PulseBullet::ONELOOP);
	PulseBullet3rdWave.AmountToSpawn = 80;
	PulseBullet3rdWave.CircleRadius = 30.0f;
	PulseBullet3rdWave.Init();


	// Bullet rage setup
	for (int i = 0; i < MAX_DEMON_RAGE_BULLETS; i++)
	{
		BulletRage[i].SetBulletType(PulseBullet::RAGE);
		BulletRage[i].AmountToSpawn = 60;
		BulletRage[i].CircleRadius = 20.0f;
		BulletRage[i].SpawnLocation = {Hitbox.x, Hitbox.y};
		
		for (int j = 0; j < MAX_PULSE_BULLETS; j++)
			BulletRage[i].Bullet[j].Player = Player;
		
		BulletRage[i].Init();
	}

	BulletWave = FIRST;

	SetDestLocation({float(GetRandomValue(0 + Sprite.width/10, GetScreenWidth() - Sprite.width/10)), float(GetRandomValue(0 + Sprite.height, 150))});
}

void Demon::Update()
{
	if (IsLowHealth())
		BulletWave = RAGE;

	// Update enemy sprite position and its components
	if (bActive && !bIsDead)
	{
		DemonSpriteFramesCounter++;

		if (!IsLowHealth())
			MoveToLocation(Destination);
		else
			MoveToLocation({float(GetRandomValue(0, GetScreenWidth())), float(GetRandomValue(0, 150))});

		SpriteBox.x = Location.x;
		SpriteBox.y = Location.y;

		Hitbox.x = Location.x + HitboxOffset.x;
		Hitbox.y = Location.y + HitboxOffset.y;

		UpdateDemonAnimation();
	}

	UpdateBullet();

	CheckCollisionWithPlayer();
	CheckCollisionWithPlayerBullets();
	CheckHealth();

	IsAtLocation(Destination);
}

void Demon::Draw()
{
	// Draw debug information
	if (bDebug && bActive && !bIsDead)
	{
		DrawRectangle(int(SpriteBox.x), int(SpriteBox.y), int(SpriteBox.width), int(SpriteBox.height), WHITE); // A rectangle that its width/height is the same as the sprite's width/height
		DrawRectangle(int(Hitbox.x), int(Hitbox.y), int(Hitbox.width), int(Hitbox.height), GRAY); // Hitbox
		DrawText(FormatText("Demon Health: %02i", Health), 10, 60, 20, RED); // Demon health
		DrawText(FormatText("X: %01i", Direction.x), 10, 100, 18, WHITE);
	}

	// Draw the demon sprite
	if (bActive && !bIsDead)
		DrawTextureRec(Sprite, DemonFrameRec, Location, WHITE);  // Draw part of the demon texture
	
	DrawBullet();

}

void Demon::UpdateBullet()
{
	switch (BulletWave)
	{
		case FIRST:
			if (IsAtLocation(Destination)) // If enemy has set a new location to go to
				PulseBullet.bRelease = true;

			PulseBullet.SpawnLocation = {Hitbox.x, Hitbox.y};
			PulseBullet.Update();

			if (PulseBullet.IsOutsideWindow())
				BulletWave = SECOND;
		break;

		case SECOND:
			FramesCounter++;
			
			PulseBullet2ndWave.SpawnLocation = {Hitbox.x, Hitbox.y};

			if (((FramesCounter/12)%2) == 1) // 0.125 second
			{
				PulseBullet2ndWave.ReleaseAmount += ShootRate;
				PulseBullet2ndWave.bRelease = true;
				FramesCounter = 0;
				StopMoving();
			}

			// To prevent game from crashing/accessing null memory
			if (PulseBullet2ndWave.ReleaseAmount > PulseBullet2ndWave.AmountToSpawn)
			{
				PulseBullet2ndWave.ReleaseAmount = PulseBullet2ndWave.AmountToSpawn;
				StartMoving();
			}

			PulseBullet2ndWave.Update();

			if (PulseBullet2ndWave.IsOutsideWindow())
			{
				BulletWave = THIRD;
				FramesCounter = 0;
			}
		break;

		case THIRD:
			FramesCounter++;
			
			PulseBullet3rdWave.SpawnLocation = {Hitbox.x, Hitbox.y};

			if (((FramesCounter/15)%2) == 1) // 0.125 second
			{
				PulseBullet3rdWave.ReleaseAmount += ShootRate;
				PulseBullet3rdWave.bRelease = true;
				FramesCounter = 0;
				StopMoving();
			}

			// To prevent game from crashing/accessing null memory
			if (PulseBullet3rdWave.ReleaseAmount > PulseBullet3rdWave.AmountToSpawn)
			{
				PulseBullet3rdWave.ReleaseAmount = PulseBullet3rdWave.AmountToSpawn;
				StartMoving();			
			}

			PulseBullet3rdWave.Update();
		break;

		case RAGE:
			FramesCounter++;
			
			for (int i = 0; i < MAX_DEMON_RAGE_BULLETS; i++)
			{
				BulletRage[i].SpawnLocation = {Hitbox.x, Hitbox.y};

				if (((FramesCounter/40)%2) == 1 && !BulletRage[i].bRelease) // 0.3 second
				{
					BulletRage[i].bRelease = true;
					FramesCounter = 0;
				}
			
				BulletRage[i].Update();

				if (!bIsDead)
					BulletRage[i].CheckBulletWindowCollision();
			}
		break;

		default:
		break;
	}
}

void Demon::UpdateDemonAnimation()
{
	if (Direction.x > 0)
	{
		// Demon sprite animation
		if (DemonSpriteFramesCounter >= (GetFPS()/FramesSpeed))
		{
			DemonSpriteFramesCounter = 0;
			DemonCurrentFrame++;
		
			if (DemonCurrentFrame > 4)
				DemonCurrentFrame = 0;
		
			DemonFrameRec.x = float(DemonCurrentFrame)*float(Sprite.width)/10;
		}
	}
	else if (Direction.x < 0)
	{
		// Demon sprite animation
		if (DemonSpriteFramesCounter >= (GetFPS()/FramesSpeed))
		{
			DemonSpriteFramesCounter = 0;
			DemonCurrentFrame++;
		
			if (DemonCurrentFrame > 9)
				DemonCurrentFrame = 5;
		
			DemonFrameRec.x = float(DemonCurrentFrame)*float(Sprite.width)/10;
		}
	}
}

void Demon::DrawBullet() const
{
	switch (BulletWave)
	{
		case FIRST:
			DrawText("First WAVE", 10, 50, 20, WHITE);

			if (PulseBullet.bRelease) // If enemy has a new location to go to
				PulseBullet.Draw();
		break;

		case SECOND:
			DrawText("Second WAVE", 10, 50, 20, WHITE);

			PulseBullet2ndWave.Draw();
		break;

		case THIRD:
			DrawText("Third WAVE", 10, 50, 20, WHITE);

			PulseBullet3rdWave.Draw();
		break;

		case RAGE:
			for (int i = 0; i < MAX_DEMON_RAGE_BULLETS; i++)
				BulletRage[i].Draw();
		break;

		default:
		break;
	}
}

void Demon::CheckCollisionWithPlayer()
{
	if (bActive && !bIsDead)
		if (CheckCollisionRecs(Player->Hitbox, SpriteBox))
			Player->Health -= Damage;
}

void Demon::CheckCollisionWithPlayerBullets()
{
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
		if (CheckCollisionCircleRec(Player->Bullet[i].Center, Player->Bullet[i].Radius, Hitbox))
			if (bActive && !bIsDead)
			{
				Player->ResetBullet(i);

				Health -= Player->BulletDamage;
			}
}

void Demon::CheckHealth()
{
	if (Health <= 0 && bActive && !bIsDead)
	{
		Health = 0;
		Player->EnemiesKilled++;
		bActive = false;
		bIsDead = true;
	}
}

bool Demon::IsAtLocation(const Vector2 & GoalLocation)
{
	if (Location.x > GoalLocation.x - TOLERANCE && Location.y > GoalLocation.y - TOLERANCE) // Is at the goal location?
	{
		bIsDestinationSet = false;

		if (!bIsDestinationSet)
		{
			SetDestLocation({float(GetRandomValue(0 + Sprite.width/10, GetScreenWidth() - Sprite.width/10)), float(GetRandomValue(0 - Sprite.height/4, GetScreenHeight() - 650))});
			bIsDestinationSet = true;
		}
	}
	else
		bIsDestinationSet = false;

	return bIsDestinationSet;
}

bool Demon::IsLowHealth() const
{
	bool bLowHealth;

	Health <= 150 ? bLowHealth = true : bLowHealth = false;

	return bLowHealth;
}
