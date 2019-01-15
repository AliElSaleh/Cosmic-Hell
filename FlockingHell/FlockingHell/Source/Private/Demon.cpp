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
	Health = 120;
	Speed = 140.0f;
	Damage = GetRandomValue(1, 3);
	ShootRate = 5;
	bActive = true;
	bIsDead = false;
	bDebug = false;

	DemonFrameRec.x = 0.0f;
	DemonFrameRec.y = 0.0f;
	DemonFrameRec.width = float(Sprite.width)/10; // 10 frames
	DemonFrameRec.height = float(Sprite.height);

	// FIRST WAVE
	for (int i = 0; i < 5; i++)
	{
		CircleBullet[i].SetBulletPattern(BulletPatternGenerator::CIRCLE);
		CircleBullet[i].SetDelayAmount(i*0.1f);
		CircleBullet[i].Enemy = this;
		CircleBullet[i].Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
		CircleBullet[i].Init();
	}

	for (int i = 5; i < 10; i++)
	{
		CircleBullet[i].SetBulletPattern(BulletPatternGenerator::CIRCLE_HOLE);
		CircleBullet[i].SetDelayAmount((i-5)*0.1f);
		CircleBullet[i].Enemy = this;
		CircleBullet[i].Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
		CircleBullet[i].Init();
	}

	for (int i = 10; i < 15; i++)
	{
		CircleBullet[i].SetBulletPattern(BulletPatternGenerator::CIRCLE_HOLE_LOCK_ON);
		CircleBullet[i].SetDelayAmount((i-10)*0.1f);
		CircleBullet[i].Enemy = this;
		CircleBullet[i].Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
		CircleBullet[i].Init();
	}

	for (int i = 15; i < 20; i++)
	{
		CircleBullet[i].SetBulletPattern(BulletPatternGenerator::CIRCLE);
		CircleBullet[i].SetDelayAmount((i-15)*0.1f);
		CircleBullet[i].Enemy = this;
		CircleBullet[i].Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
		CircleBullet[i].Init();
	}

	// SECOND WAVE
	for (int i = 0; i < 1; i++)
	{
		LinearMultiBullet[i].SetBulletPattern(BulletPatternGenerator::FIVE_WAY_LINEAR_LOCK_ON);
		LinearMultiBullet[i].SetDelayAmount(i*5.0f);
		LinearMultiBullet[i].Enemy = this;
		LinearMultiBullet[i].Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
		LinearMultiBullet[i].Init();
	}

	for (int i = 1; i < 2; i++)
	{
		LinearMultiBullet[i].SetBulletPattern(BulletPatternGenerator::SIX_WAY_LINEAR_LOCK_ON);
		LinearMultiBullet[i].SetDelayAmount((i-2)*3.0f);
		LinearMultiBullet[i].Enemy = this;
		LinearMultiBullet[i].Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
		LinearMultiBullet[i].Init();
	}

	for (int i = 2; i < 3; i++)
	{
		LinearMultiBullet[i].SetBulletPattern(BulletPatternGenerator::ELEVEN_WAY_AIMING);
		LinearMultiBullet[i].SetDelayAmount((i-4)*5.0f);
		LinearMultiBullet[i].Enemy = this;
		LinearMultiBullet[i].Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
		LinearMultiBullet[i].Init();
	}

	for (int i = 3; i < 4; i++)
	{
		LinearMultiBullet[i].SetBulletPattern(BulletPatternGenerator::TWENTY_WAY);
		LinearMultiBullet[i].SetDelayAmount((i-6)*3.0f);
		LinearMultiBullet[i].Enemy = this;
		LinearMultiBullet[i].Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
		LinearMultiBullet[i].Init();
	}

	SpiralBullet[0].SetBulletPattern(BulletPatternGenerator::SPIRAL_LEFT);
	SpiralBullet[1].SetBulletPattern(BulletPatternGenerator::SPIRAL_RIGHT);
	SpiralBullet[2].SetBulletPattern(BulletPatternGenerator::SPIRAL_DOUBLE);
	SpiralBullet[3].SetBulletPattern(BulletPatternGenerator::SPIRAL_LEFT);

	for (int i = 0; i < 4; i++)
	{
		SpiralBullet[i].SetDelayAmount(0.0f);
		SpiralBullet[i].Enemy = this;
		SpiralBullet[i].Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
		SpiralBullet[i].Init();
	}

	RageBullet.SetBulletPattern(BulletPatternGenerator::SPIRAL_MIX);
	RageBullet.SetDelayAmount(0.0f);
	RageBullet.Enemy = this;
	RageBullet.Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
	RageBullet.Init();

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

	BulletWave = RAGE;

	SetDestLocation({float(GetRandomValue(0 + Sprite.width/10 + 100, GetScreenWidth() - Sprite.width/10 - 100)), float(GetRandomValue(0 + Sprite.height, 100))});
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

		for (int i = 0; i < 20; i++)
		{
			CircleBullet[i].Location = SpawnLocation;
			CircleBullet[i].TargetLocation = Player->Location;
		}

		for (int i = 0; i < 10; i++)
		{
			LinearMultiBullet[i].Location = SpawnLocation;
			LinearMultiBullet[i].TargetLocation = Player->Location;
		}

		for (int i = 0; i < 4; i++)
			SpiralBullet[i].Location = SpawnLocation;

		RageBullet.Location = SpawnLocation;

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
				BulletWave = FIRST_A;
		break;

		case FIRST_A:
			// Release bullets when enemy is at a location
			if (IsAtLocation(Destination))
			{
				StopMoving();
				
				for (int i = 5; i < 10; i++)
					CircleBullet[i].bRelease = true;
			}

			for (int i = 5; i < 10; i++)
				CircleBullet[i].Update();

			// Make enemy move again and switch to the next wave
			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(CircleBullet[9])))
				BulletWave = FIRST_B;
		break;

		case FIRST_B:
			// Release bullets when enemy is at a location
			if (IsAtLocation(Destination))
			{
				StopMoving();
				
				for (int i = 10; i < 15; i++)
					CircleBullet[i].bRelease = true;
			}

			for (int i = 10; i < 15; i++)
				CircleBullet[i].Update();

			// Make enemy move again and switch to the next wave
			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(CircleBullet[14])))
				BulletWave = FIRST_C;
		break;

		case FIRST_C:
			// Release bullets when enemy is at a location
			if (IsAtLocation(Destination))
			{
				StopMoving();
				
				for (int i = 15; i < 20; i++)
					CircleBullet[i].bRelease = true;
			}

			for (int i = 15; i < 20; i++)
				CircleBullet[i].Update();

			// Make enemy move again and switch to the next wave
			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(CircleBullet[19])))
				BulletWave = SECOND;
		break;

		case SECOND:
			// Release bullets when enemy is at a location
			if (IsAtLocation(Destination))
			{
				StopMoving();

				for (int i = 0; i < 1; i++)
					LinearMultiBullet[i].bRelease = true;
			}
			
			for (int i = 0; i < 1; i++)
				LinearMultiBullet[i].Update();

			// Make enemy move again and switch to the next wave
			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(LinearMultiBullet[0])))
				BulletWave = SECOND_A;
		break;

		case SECOND_A:
			// Release bullets when enemy is at a location
			if (IsAtLocation(Destination))
			{
				StopMoving();

				for (int i = 1; i < 2; i++)
					LinearMultiBullet[i].bRelease = true;
			}
			
			for (int i = 1; i < 2; i++)
				LinearMultiBullet[i].Update();

			// Make enemy move again and switch to the next wave
			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(LinearMultiBullet[1])))
				BulletWave = SECOND_B;
		break;

		case SECOND_B:
			// Release bullets when enemy is at a location
			if (IsAtLocation(Destination))
			{
				StopMoving();

				for (int i = 2; i < 3; i++)
					LinearMultiBullet[i].bRelease = true;
			}
			
			for (int i = 2; i < 3; i++)
				LinearMultiBullet[i].Update();

			// Make enemy move again and switch to the next wave
			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(LinearMultiBullet[2])))
				BulletWave = SECOND_C;
		break;

		case SECOND_C:
			// Release bullets when enemy is at a location
			if (IsAtLocation(Destination))
			{
				StopMoving();

				for (int i = 3; i < 4; i++)
					LinearMultiBullet[i].bRelease = true;
			}
			
			for (int i = 3; i < 4; i++)
				LinearMultiBullet[i].Update();

			// Make enemy move again and switch to the next wave
			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(LinearMultiBullet[3])))
				BulletWave = THIRD;
		break;

		case THIRD:
			// Release bullets when enemy is at a location
			if (IsAtLocation(Destination))
			{
				StopMoving();

				SpiralBullet[0].bRelease = true;
			}
			
			SpiralBullet[0].Update();

			// Make enemy move again and switch to the next wave
			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(SpiralBullet[0])))
				BulletWave = THIRD_A;
		break;

		case THIRD_A:
			// Release bullets when enemy is at a location
			if (IsAtLocation(Destination))
			{
				StopMoving();

				SpiralBullet[1].bRelease = true;
			}
			
			SpiralBullet[1].Update();

			// Make enemy move again and switch to the next wave
			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(SpiralBullet[1])))
				BulletWave = THIRD_B;
		break;

		case THIRD_B:
			// Release bullets when enemy is at a location
			if (IsAtLocation(Destination))
			{
				StopMoving();

				SpiralBullet[2].bRelease = true;
			}
			
			SpiralBullet[2].Update();

			// Make enemy move again and switch to the next wave
			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(SpiralBullet[2])))
				BulletWave = THIRD_C;
		break;

		case THIRD_C:
			// Release bullets when enemy is at a location
			if (IsAtLocation(Destination))
			{
				StopMoving();

				SpiralBullet[3].bRelease = true;
			}
			
			SpiralBullet[3].Update();

			// Make enemy move again and switch to the next wave
			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(SpiralBullet[3])))
				BulletWave = RAGE;
		break;

		case RAGE:
			//if (IsAtLocation(Destination))
			//{
				StopMoving();

				RageBullet.bRelease = true;
			//}

			RageBullet.Update();

			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(RageBullet)))
				if (!bIsDead)
					BulletWave = FIRST;
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

		case FIRST_A:
			DrawText("First A WAVE", 10, 50, 20, WHITE);

			for (int i = 5; i < 10; i++)
				CircleBullet[i].Draw();
		break;

		case FIRST_B:
			DrawText("First B WAVE", 10, 50, 20, WHITE);

			for (int i = 10; i < 15; i++)
				CircleBullet[i].Draw();
		break;

		case FIRST_C:
			DrawText("First C WAVE", 10, 50, 20, WHITE);

			for (int i = 15; i < 20; i++)
				CircleBullet[i].Draw();
		break;

		case SECOND:
			DrawText("Second WAVE", 10, 50, 20, WHITE);

			for (int i = 0; i < 1; i++)
				LinearMultiBullet[i].Draw();
		break;

		case SECOND_A:
			for (int i = 1; i < 2; i++)
				LinearMultiBullet[i].Draw();
		break;

		case SECOND_B:
			for (int i = 2; i < 3; i++)
				LinearMultiBullet[i].Draw();
		break;

		case SECOND_C:
			for (int i = 3; i < 4; i++)
				LinearMultiBullet[i].Draw();
		break;

		case THIRD:
			DrawText("Third WAVE", 10, 50, 20, WHITE);

			SpiralBullet[0].Draw();
		break;

		case THIRD_A:
			SpiralBullet[1].Draw();
		break;

		case THIRD_B:
			SpiralBullet[2].Draw();
		break;

		case THIRD_C:
			SpiralBullet[3].Draw();
		break;

		case RAGE:
			RageBullet.Draw();
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
