#include "Globals.h"
#include "BulletPatternGenerator.h"
#include "Demon.h"
#include "Player.h"
#include "Assets.h"

#include <algorithm>

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

Demon::Demon()
{
	Location = {300.0f, -300.0f};
	Health = 2000;
	LowHealthThreshold = 400;
	Explosions = 3;

	HealthBar = {20.0f, GetScreenHeight() - 30.0f, GetScreenWidth() - PANEL_WIDTH - 46.0f, 4.0f, Health, ORANGE};
}

void Demon::Init()
{
	Sprite = GetAsset(Demon);

	SpawnLocation = {50.0f, 105.0f};
	HitboxOffset = {50.0f, 105.0f};
	Hitbox = {Location.x + HitboxOffset.x, Location.y + HitboxOffset.y, float(Sprite.width)/10 - 80, float(Sprite.height)/3};
	SpriteBox = {Location.x, Location.y, float(Sprite.width)/10, float(Sprite.height)};
	MaxVelocity = 2.0f;
	MaxForce = 0.5f;
	Mass = 30.0f; // 30Kg
	TargetRadius = 10.0f;

	Speed = 140.0f;
	Damage = GetRandomValue(1, 3);
	Frames = 10;
	ShootRate = 5;

	bIsBoss = true;
	bActive = true;
	bIsDead = false;
	bDebug = false;

	FrameRec.x = 0.0f;
	FrameRec.y = 0.0f;
	FrameRec.width = float(Sprite.width)/Frames;
	FrameRec.height = float(Sprite.height);

	for (int i = 0; i < 20; i++)
		DeathExplosion[i].Init();

	// FIRST WAVE
	for (int i = 0; i < 5; i++)
		InitBullet(CircleBullet[i], i*0.1f, BulletPatternGenerator::CIRCLE);

	for (int i = 5; i < 10; i++)
		InitBullet(CircleBullet[i], (i-5)*0.1f, BulletPatternGenerator::CIRCLE_HOLE);

	for (int i = 10; i < 15; i++)
		InitBullet(CircleBullet[i], (i-10)*0.1f, BulletPatternGenerator::CIRCLE_HOLE_LOCK_ON);

	for (int i = 15; i < 20; i++)
		InitBullet(CircleBullet[i], (i-15)*0.1f, BulletPatternGenerator::CIRCLE);

	// SECOND WAVE
	InitBullet(LinearMultiBullet[0], 0.0f, BulletPatternGenerator::FIVE_WAY_LINEAR_LOCK_ON);
	InitBullet(LinearMultiBullet[1], 0.0f, BulletPatternGenerator::SIX_WAY_LINEAR_LOCK_ON);
	InitBullet(LinearMultiBullet[2], 0.0f, BulletPatternGenerator::ELEVEN_WAY_AIMING);
	InitBullet(LinearMultiBullet[3], 0.0f, BulletPatternGenerator::TWENTY_WAY);

	// THIRD WAVE
	InitBullet(SpiralBullet[0], 0.0f, BulletPatternGenerator::SPIRAL_LEFT);
	InitBullet(SpiralBullet[1], 0.0f, BulletPatternGenerator::SPIRAL_RIGHT);
	InitBullet(SpiralBullet[2], 0.0f, BulletPatternGenerator::SPIRAL_DOUBLE);
	InitBullet(SpiralBullet[3], 0.0f, BulletPatternGenerator::SPIRAL_LEFT);

	// RAGE
	InitBullet(RageBullet, 0.0f, BulletPatternGenerator::SPIRAL_MIX);
	
	FinalBullets = &RageBullet.Bullet;

	BulletWave = FIRST;

	SetDestLocation({float(GetRandomValue(0 + Sprite.width/10 + 100, GetScreenWidth()-PANEL_WIDTH - Sprite.width/10 - 100)), float(GetRandomValue(0 - 10, 100))});
}

void Demon::Update()
{
	if (IsLowHealth())
		BulletWave = RAGE;

	// Update enemy sprite position and its components
	if (bActive && !bIsDead)
	{
		SpriteFramesCounter++;

		if (!IsLowHealth())
		{
			const Vector2 SeekingForce = Seek(Destination);
			ApplyForce(SeekingForce);
		}
		else
			Seek({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH)), float(GetRandomValue(0, 150))});

		SpriteBox.x = Location.x;
		SpriteBox.y = Location.y;

		Hitbox.y = Location.y + HitboxOffset.y;

		SpawnLocation = {Location.x + float(Sprite.width) /20, Location.y + float(Sprite.height) / 2 - 30};

		UpdateAnimation();
	}
	
	if (bIsDead)
	{
		Player->BulletLevel = 2;

		const auto Predicate = [](const Bullet &b) { return !b.bActive; };
		RageBullet.Bullet.erase(std::remove_if(RageBullet.Bullet.begin(), RageBullet.Bullet.end(), Predicate), RageBullet.Bullet.end());
		
		for (int i = 0; i < 20; i++)
			DeathExplosion[i].Explode({float(GetRandomValue(int(Location.x), int(Location.x) + Sprite.width/Frames)), float(GetRandomValue(int(Location.y), int(Location.y) + Sprite.height))}, Explosions);
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
		DrawRectangleLines(int(SpriteBox.x), int(SpriteBox.y), int(SpriteBox.width), int(SpriteBox.height), WHITE); // A rectangle that its width/height is the same as the sprite's width/height
		DrawRectangle(int(Hitbox.x), int(Hitbox.y), int(Hitbox.width), int(Hitbox.height), GRAY); // Hitbox
		DrawCircle(int(SpawnLocation.x), int(SpawnLocation.y), 5.0f, YELLOW);
	}

	// Draw the demon sprite
	if (bActive && !bIsDead)
	{
		DrawTextureRec(Sprite, FrameRec, Location, WHITE);  // Draw part of the demon texture
		HealthBar.Draw();
	}
	
	if (bIsDead)
		for (int i = 0; i < 20; i++)
			DeathExplosion[i].Draw();

	DrawBullet();
}

void Demon::InitBullet(BulletPatternGenerator& BulletPattern, const float Delay, const BulletPatternGenerator::Pattern Pattern)
{
	BulletPattern.SetBulletPattern(Pattern);
	BulletPattern.SetDelayAmount(Delay);
	BulletPattern.Enemy = this;
	BulletPattern.Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
	BulletPattern.Init();

	for (unsigned short i = 0; i < BulletPattern.Bullet.size(); i++)
	{
		BulletPattern.Bullet[i].Player = Player;
		BulletPattern.Bullet[i].Frames = 6;
		BulletPattern.Bullet[i].Sprite = GetAsset(FireBullet);
		
		BulletPattern.Bullet[i].InitFrames();
	}
}

void Demon::UpdateBullet()
{
	for (int i = 0; i < 20; i++)
	{
		CircleBullet[i].Location = SpawnLocation;
		CircleBullet[i].TargetLocation = Player->Center;
		CircleBullet[i].UpdateAnimation();
	}

	for (int i = 0; i < 10; i++)
	{
		LinearMultiBullet[i].Location = SpawnLocation;
		LinearMultiBullet[i].TargetLocation = Player->Location;
		LinearMultiBullet[i].UpdateAnimation();
	}

	for (int i = 0; i < 4; i++)
	{
		SpiralBullet[i].Location = SpawnLocation;
		SpiralBullet[i].UpdateAnimation();
	}

	RageBullet.Location = SpawnLocation;
	RageBullet.UpdateAnimation();

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
			{
				if (!IsLowHealth())
				{
					Init();
					BulletWave = FIRST;
				}
				else
					BulletWave = RAGE;
			}
		break;

		case RAGE:
			StopMoving();

			RageBullet.Center = SpawnLocation;
			RageBullet.bRelease = true;
			
			RageBullet.Update();

			if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(RageBullet)))
				if (!bIsDead)
				{
					BulletWave = RAGE;

					RageBullet.Init();

					for (unsigned short i = 0; i < RageBullet.Bullet.size(); i++)
					{
						RageBullet.Bullet[i].Player = Player;
						RageBullet.Bullet[i].Frames = 6;
						RageBullet.Bullet[i].Sprite = GetAsset(FireBullet);
						
						RageBullet.Bullet[i].InitFrames();
					}
				}
		break;

		default:
		break;
	}
}

void Demon::UpdateAnimation()
{
	if (Direction.x > 0)
	{
		Hitbox.x = Location.x + HitboxOffset.x;

		// Demon sprite animation
		if (SpriteFramesCounter >= (GetFPS()/FramesSpeed))
		{
			SpriteFramesCounter = 0;
			CurrentFrame++;
		
			if (CurrentFrame > 4)
				CurrentFrame = 0;
		
			FrameRec.x = float(CurrentFrame)*float(Sprite.width)/Frames;
		}
	}
	else if (Direction.x < 0)
	{
		Hitbox.x = Location.x + 35.0f;

		// Demon sprite animation
		if (SpriteFramesCounter >= (GetFPS()/FramesSpeed))
		{
			SpriteFramesCounter = 0;
			CurrentFrame++;
		
			if (CurrentFrame > 9)
				CurrentFrame = 5;
		
			FrameRec.x = float(CurrentFrame)*float(Sprite.width)/Frames;
		}
	}
}

void Demon::DrawBullet()
{
	switch (BulletWave)
	{
		case FIRST:
			if (bDebug)
				RDrawText("First WAVE", 10, 50, 20, WHITE);

			for (int i = 0; i < 5; i++)
				CircleBullet[i].Draw();
		break;

		case FIRST_A:
			if (bDebug)
				RDrawText("First A WAVE", 10, 50, 20, WHITE);

			for (int i = 5; i < 10; i++)
				CircleBullet[i].Draw();
		break;

		case FIRST_B:
			if (bDebug)
				RDrawText("First B WAVE", 10, 50, 20, WHITE);

			for (int i = 10; i < 15; i++)
				CircleBullet[i].Draw();
		break;

		case FIRST_C:
			if (bDebug)
				RDrawText("First C WAVE", 10, 50, 20, WHITE);

			for (int i = 15; i < 20; i++)
				CircleBullet[i].Draw();
		break;

		case SECOND:
			if (bDebug)
				RDrawText("Second WAVE", 10, 50, 20, WHITE);

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
			if (bDebug)
				RDrawText("Third WAVE", 10, 50, 20, WHITE);

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

void Demon::IncreasePlayerScore()
{
	Player->Score += GetRandomValue(30, 40);
}
