#include "BulletPatternGenerator.h"
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


	for (int i = 0; i < 5; i++)
	{
		CircleBullet[i].SetBulletPattern(BulletPatternGenerator::CIRCLE);
		CircleBullet[i].SetDelayAmount(i*0.2f);
		CircleBullet[i].Enemy = this;
		CircleBullet[i].Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
		CircleBullet[i].Init();
	}

	LinearMultiBullet.SetBulletPattern(BulletPatternGenerator::ELEVEN_WAY_AIMING);
	LinearMultiBullet.Enemy = this;
	LinearMultiBullet.Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
	LinearMultiBullet.Init();

	// Bullet wave setup
	//PulseBullet3rdWave.SetBulletType(PulseBullet::ONELOOP);
	//PulseBullet3rdWave.AmountToSpawn = 80;
	//PulseBullet3rdWave.CircleRadius = 30.0f;
	//PulseBullet3rdWave.Init();


	//// Bullet rage setup
	//for (int i = 0; i < MAX_DEMON_RAGE_BULLETS; i++)
	//{
	//	BulletRage[i].SetBulletType(PulseBullet::RAGE);
	//	BulletRage[i].AmountToSpawn = 60;
	//	BulletRage[i].CircleRadius = 20.0f;
	//	BulletRage[i].SpawnLocation = {Hitbox.x, Hitbox.y};
	//	
	//	for (int j = 0; j < MAX_PULSE_BULLETS; j++)
	//		BulletRage[i].Bullet[j].Player = Player;
	//	
	//	BulletRage[i].Init();
	//}

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

		SpawnLocation = {Location.x + float(Sprite.width) /20, Location.y + float(Sprite.height) / 2 - 30};

		for (int i = 0; i < 5; i++)
			CircleBullet[i].Location = SpawnLocation;

		LinearMultiBullet.Location = SpawnLocation;
		LinearMultiBullet.TargetLocation = Player->Location;

		UpdateDemonAnimation();
	}

	UpdateBullet();

	CheckCollisionWithPlayer();
	CheckCollisionWithPlayerBullets();
	CheckHealth();
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
		DrawCircle(SpawnLocation.x, SpawnLocation.y, 5.0f, WHITE);
	}

	// Draw the demon sprite
	if (bActive && !bIsDead)
		DrawTextureRec(Sprite, DemonFrameRec, Location, WHITE);  // Draw part of the demon texture
	
	DrawBullet();

}

bool Demon::IsBulletSequenceComplete(const BulletPatternGenerator &BulletPattern)
{
	if (BulletPattern.Bullet.empty())
	{
		SetDestLocation({float(GetRandomValue(0 + Sprite.width/10, GetScreenWidth() - Sprite.width/10)), float(GetRandomValue(0 - Sprite.height/4, GetScreenHeight() - 650))});
		StartMoving();
		bIsDestinationSet = true;
	}
	else
		bIsDestinationSet = false;

	return bIsDestinationSet;
}

float Demon::Round(const float Number)
{
	float Value;
	if (Number < 0)
		Value = (int)(Number - 0.5);
	else
		Value = (int)(Number + 0.5);

    return Value; 
}

void Demon::UpdateBullet()
{
	switch (BulletWave)
	{
		case FIRST:
			// Release bullets when enemy is at a location
			if (IsAtLocation(Destination))
			{
				StopMoving();
				
				for (int i = 0; i < 5; i++)
					CircleBullet[i].bRelease = true;
			}

			for (int i = 0; i < 5; i++)
				CircleBullet[i].Update();

			// Make enemy move again and switch to the next wave
			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(CircleBullet[4])))
				BulletWave = SECOND;
		break;

		case SECOND:
			// Release bullets when enemy is at a location
			if (IsAtLocation(Destination))
			{
				StopMoving();

				LinearMultiBullet.bRelease = true;
			}
			
			LinearMultiBullet.Update();

			// Make enemy move again and switch to the next wave
			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(LinearMultiBullet)))
				BulletWave = THIRD;
		break;

		case THIRD:
			FramesCounter++;
			
			//PulseBullet3rdWave.SpawnLocation = {Hitbox.x, Hitbox.y};
			//
			//if (((FramesCounter/15)%2) == 1) // 0.125 second
			//{
			//	PulseBullet3rdWave.ReleaseAmount += ShootRate;
			//	PulseBullet3rdWave.bRelease = true;
			//	FramesCounter = 0;
			//	StopMoving();
			//}
			//
			//// To prevent game from crashing/accessing null memory
			//if (PulseBullet3rdWave.ReleaseAmount > PulseBullet3rdWave.AmountToSpawn)
			//{
			//	PulseBullet3rdWave.ReleaseAmount = PulseBullet3rdWave.AmountToSpawn;
			//	StartMoving();			
			//}
			//
			//PulseBullet3rdWave.Update();
		break;

		case RAGE:
			FramesCounter++;
			
			//for (int i = 0; i < MAX_DEMON_RAGE_BULLETS; i++)
			//{
			//	BulletRage[i].SpawnLocation = {Hitbox.x, Hitbox.y};
			//
			//	if (((FramesCounter/40)%2) == 1 && !BulletRage[i].bRelease) // 0.3 second
			//	{
			//		BulletRage[i].bRelease = true;
			//		FramesCounter = 0;
			//	}
			//
			//	BulletRage[i].Update();
			//
			//	if (!bIsDead)
			//		BulletRage[i].CheckBulletWindowCollision();
			//}
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

void Demon::DrawBullet()
{
	switch (BulletWave)
	{
		case FIRST:
			DrawText("First WAVE", 10, 50, 20, WHITE);

			for (int i = 0; i < 5; i++)
				CircleBullet[i].Draw();
		break;

		case SECOND:
			DrawText("Second WAVE", 10, 50, 20, WHITE);

			LinearMultiBullet.Draw();
		break;

		case THIRD:
			DrawText("Third WAVE", 10, 50, 20, WHITE);

			//PulseBullet3rdWave.Draw();
		break;

		case RAGE:
			//for (int i = 0; i < MAX_DEMON_RAGE_BULLETS; i++)
			//	BulletRage[i].Draw();
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

bool Demon::IsAtLocation(const Vector2 &GoalLocation)
{
	if (Round(Location.x) == GoalLocation.x && Round(Location.y) == GoalLocation.y) // Is at the goal location?
		bIsAtLocation = true;
	else
		bIsAtLocation = false;

	return bIsAtLocation;
}

bool Demon::IsLowHealth() const
{
	bool bLowHealth;

	Health <= 150 ? bLowHealth = true : bLowHealth = false;

	return bLowHealth;
}
