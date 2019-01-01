#include "Demon.h"
#include "Player.h"

void Demon::Init()
{
	Location = {300.0f, 100.0f};
	SpawnLocation = {50.0f, 105.0f};
	HitboxOffset = {50.0f, 105.0f};
	Hitbox = {Location.x + HitboxOffset.x, Location.y + HitboxOffset.y, float(Sprite.width)/11, float(Sprite.height)/4};
	SpriteBox = {Location.x, Location.y, float(Sprite.width), float(Sprite.height)};
	Health = 500;
	Speed = 120.0f;
	Damage = GetRandomValue(1, 3);
	ShootRate = 5;
	bActive = true;
	bIsDead = false;
	bFirstLaunch = true;
	bDebug = false;

	if (bFirstLaunch)
	{
		BulletSprite = LoadTexture("Sprites/RedBullet.png");
		bFirstLaunch = false;
	}

	for (int i = 0; i < MAX_PULSE_BULLETS; i++)
	{
		PulseBullet.Bullet[i].Sprite = BulletSprite;
		PulseBullet2ndWave.Bullet[i].Sprite = BulletSprite; // Probably use a different bullet sprite for each wave
		PulseBullet2ndWave.Bullet[i].Speed = 400.0f; // Probably use a different bullet sprite for each wave

		for (int j = 0; j < MAX_DEMON_RAGE_BULLETS; j++)
		{
			BulletRage[j].Bullet[i].Sprite = BulletSprite;
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

	for (int i = 0; i < MAX_DEMON_RAGE_BULLETS; i++)
	{
		BulletRage[i].SetBulletType(PulseBullet::RAGE);
		BulletRage[i].AmountToSpawn = 60.0f;
		BulletRage[i].CircleRadius = 20.0f;
		BulletRage[i].SpawnLocation = {Hitbox.x, Hitbox.y};
		
		for (int j = 0; j < MAX_PULSE_BULLETS; j++)
			BulletRage[i].Bullet[j].Player = Player;
		
		BulletRage[i].Init();
	}

	BulletWave = FIRST;

	SetDestLocation({float(GetRandomValue(0 + Sprite.width, GetScreenWidth() - Sprite.width)), float(GetRandomValue(0 + Sprite.height/2, 150))});
}

void Demon::Update()
{
	if (IsLowHealth())
		BulletWave = RAGE;

	// Update enemy sprite position and its components
	if (bActive && !bIsDead)
	{
		if (!IsLowHealth())
			MoveToLocation(Destination);
		else
			MoveToLocation({float(GetRandomValue(0, GetScreenWidth())), float(GetRandomValue(0, 150))});

		SpriteBox.x = Location.x;
		SpriteBox.y = Location.y;

		Hitbox.x = Location.x + HitboxOffset.x;
		Hitbox.y = Location.y + HitboxOffset.y;
	}

	UpdateBullet();

	CheckCollisionWithPlayer();
	CheckCollisionWithPlayerBullets();
	CheckHealth();

	IsAtLocation(Destination);
}

void Demon::Draw()
{
	// Draw the demon sprite
	if (bActive && !bIsDead)
		DrawTexture(Sprite, int(Location.x), int(Location.y), WHITE); // Demon sprite
	
	DrawBullet();

	// Draw debug information
	if (bDebug && bActive && !bIsDead)
	{
		DrawRectangle(int(SpriteBox.x), int(SpriteBox.y), int(SpriteBox.width), int(SpriteBox.height), WHITE); // A rectangle that its width/height is the same as the sprite's width/height
		DrawRectangle(int(Hitbox.x), int(Hitbox.y), int(Hitbox.width), int(Hitbox.height), WHITE); // Hitbox
		DrawText(FormatText("Demon Health: %02i", Health), 10, 60, 20, RED); // Demon health
	}	
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

			//if (PulseBullet2ndWave.IsOutsideWindow())
			//	BulletWave = THIRD;
		break;

		case THIRD:
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

void Demon::DrawBullet() const
{
	switch (BulletWave)
	{
		case FIRST:
			if (PulseBullet.bRelease) // If enemy has a new location to go to
				PulseBullet.Draw();
		break;

		case SECOND:
			PulseBullet2ndWave.Draw();
		break;

		case THIRD:
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

bool Demon::IsLowHealth() const
{
	bool bLowHealth;

	Health <= 150 ? bLowHealth = true : bLowHealth = false;

	return bLowHealth;
}
