#include "Alien.h"
#include "Assets.h"
#include "Globals.h"
#include "Player.h"

#include <algorithm>

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

Alien::Alien()
{
	Alien::Init();

	Location = {300.0f, -300.0f};

	Health = 5000;
	LowHealthThreshold = 500;
	Explosions = 3;

	HealthBar = {20.0f, GetScreenHeight() - 30.0f, GetScreenWidth() - PANEL_WIDTH - 46.0f, 4.0f, Health, PURPLE};

	bDebug = false;
}

void Alien::Init()
{
	Sprite = GetAsset(Alien);
	Frames = 5;
	FramesSpeed = 10;

	SpawnLocation = {Location.x + 33.0f, Location.y + float(Sprite.height) - 45.0f};
	HitboxOffset = {32.0f, 2.0f};
	Hitbox = {Location.x + HitboxOffset.x, Location.y + HitboxOffset.y, float(Sprite.width)/Frames-60.0f, float(Sprite.height)/5};
	SpriteBox = {Location.x, Location.y, float(Sprite.width)/Frames, float(Sprite.height)};
	MaxVelocity = 2.0f;
	MaxForce = 5.0f;
	Mass = 40.0f; // 40Kg
	TargetRadius = 10.0f;

	ShootRate = 5;
	Speed = 100.0f;
	Damage = GetRandomValue(1, 3);
	bIsBoss = true;
	bActive = true;
	bIsDead = false;

	FrameRec.x = 0.0f;
	FrameRec.y = 0.0f;
	FrameRec.width = float(Sprite.width)/Frames;
	FrameRec.height = float(Sprite.height);

	for (int i = 0; i < 20; i++)
		DeathExplosion[i].Init();

	// WAVE 1
	InitBullet(LinearBullet, BulletPatternGenerator::LINEAR_AIMING);

	InitBullet(LinearMultiBullet[0], BulletPatternGenerator::TWENTY_WAY);
	InitBullet(LinearMultiBullet[1], BulletPatternGenerator::THIRTY_WAY);

	// WAVE 2
	InitBullet(SpiralBullet[0], BulletPatternGenerator::SPIRAL_MULTI_RIGHT);
	InitBullet(SpiralBullet[1], BulletPatternGenerator::SPIRAL_MULTI_LEFT);

	InitBullet(SpreadBullet, BulletPatternGenerator::SPREAD_EIGHT_WAY_LOCK_ON);

	// WAVE 3
	InitBullet(SpiralMultiBullet, BulletPatternGenerator::SPIRAL_MULTI_FIVE_WAY);

	// RAGE
	InitBullet(RageBullet[0], BulletPatternGenerator::RANDOM_SPIRAL_MULTI);
	InitBullet(RageBullet[1], BulletPatternGenerator::RANDOM_AIMING);

	FinalBullets = &RageBullet[1].Bullet;

	BulletWave = FIRST;

	SetDestLocation({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH - Sprite.width/Frames)), float(GetRandomValue(0, 100))});
}

void Alien::Update()
{
	if (IsLowHealth())
		BulletWave = RAGE;
	
	if (bActive && !bIsDead)
	{
		if (!IsLowHealth())
		{
			const Vector2 SeekingForce = Seek(Destination);
			ApplyForce(SeekingForce);
		}
		else
			Seek({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH)), float(GetRandomValue(0, 150))});

		SpriteBox.x = Location.x;
		SpriteBox.y = Location.y;

		Hitbox.x = Location.x + HitboxOffset.x;
		Hitbox.y = Location.y + HitboxOffset.y;

		SpawnLocation = {Location.x + 33.0f, Location.y + float(Sprite.height) - 45.0f};

		UpdateAnimation();
	}

	if(bIsDead)
	{
		Player->BulletLevel = 3;

		// Remove inactive bullets
		const auto Predicate = [](const Bullet &b) { return !b.bActive; };

		for (int i = 0; i < 2; i++)
			RageBullet[i].Bullet.erase(std::remove_if(RageBullet[i].Bullet.begin(), RageBullet[i].Bullet.end(), Predicate), RageBullet[i].Bullet.end());
		
		for (int i = 0; i < 20; i++)
			DeathExplosion[i].Explode({float(GetRandomValue(int(Location.x), int(Location.x) + Sprite.width/Frames)), float(GetRandomValue(int(Location.y), int(Location.y) + Sprite.height))}, Explosions);
	}

	UpdateBullet();

	CheckCollisionWithPlayer();
	CheckCollisionWithPlayerBullets();
	CheckHealth();
}

void Alien::Draw()
{
	if (bDebug)
		DrawRectangleLines(int(SpriteBox.x), int(SpriteBox.y), int(SpriteBox.width), int(SpriteBox.height), WHITE); // Sprite box

	if (!bIsDead && bActive)
	{
		DrawTextureRec(Sprite, FrameRec, Location, WHITE);
		HealthBar.Draw();
	}

	if (bDebug)
	{
		DrawRectangleRec(Hitbox, YELLOW);
		DrawCircle(int(SpawnLocation.x), int(SpawnLocation.y), 3.0f, YELLOW); // Bullet spawn location

		if (bActive)
			RDrawText(FormatText("Health: %02i", Health), 10, 750, 20, WHITE);
	}

	if (bIsDead)
		for (int i = 0; i < 20; i++)
			DeathExplosion[i].Draw();

	DrawBullet();
}

void Alien::InitBullet(BulletPatternGenerator &BulletPattern, const BulletPatternGenerator::Pattern Pattern)
{
	BulletPattern.SetBulletPattern(Pattern);
	BulletPattern.SetDelayAmount(0.0f);
	BulletPattern.Enemy = this;
	BulletPattern.Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
	BulletPattern.Init();

	for (unsigned short i = 0; i < BulletPattern.Bullet.size(); i++)
	{
		BulletPattern.Bullet[i].Player = Player;
		BulletPattern.Bullet[i].Frames = 4;
		BulletPattern.Bullet[i].Sprite = GetAsset(PurpleBullet);
		
		BulletPattern.Bullet[i].InitFrames();
	}
}

void Alien::UpdateBullet()
{
	// WAVE 1
	LinearBullet.Location = SpawnLocation;
	LinearBullet.TargetLocation = Player->Center;
	LinearBullet.UpdateAnimation();

	for (int i = 0; i < 2; i++)
	{
		LinearMultiBullet[i].Location = SpawnLocation;
		LinearMultiBullet[i].TargetLocation = Player->Center;
		LinearMultiBullet[i].UpdateAnimation();
	}

	// WAVE 2
	for (int i = 0; i < 2; i++)
	{
		SpiralBullet[i].Location = SpawnLocation;
		SpiralBullet[i].TargetLocation = Player->Center;
		SpiralBullet[i].UpdateAnimation();
	}

	SpreadBullet.Location = SpawnLocation;
	SpreadBullet.TargetLocation = Player->Center;
	SpreadBullet.UpdateAnimation();

	// WAVE 3
	SpiralMultiBullet.Location = SpawnLocation;
	SpiralMultiBullet.TargetLocation = Player->Center;
	SpiralMultiBullet.UpdateAnimation();

	// RAGE
	for (int i = 0; i < 2; i++)
	{
		RageBullet[i].Location = SpawnLocation;
		RageBullet[i].TargetLocation = Player->Center;
		RageBullet[i].UpdateAnimation();
	}

	switch (BulletWave)
	{
	case FIRST:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			LinearBullet.bRelease = true;
		}

		LinearBullet.Update();

		if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(LinearBullet)))
			BulletWave = FIRST_A;

		break;

	case FIRST_A:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			LinearMultiBullet[0].bRelease = true;
		}

		LinearMultiBullet[0].Update();

		if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(LinearMultiBullet[0])))
			BulletWave = FIRST_B;

		break;

		case FIRST_B:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			LinearMultiBullet[1].bRelease = true;
		}

		LinearMultiBullet[1].Update();

		if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(LinearMultiBullet[1])))
			BulletWave = SECOND;

		break;

	case SECOND:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			for (int i = 0; i < 2; i++)
				SpiralBullet[i].bRelease = true;
		}

		for (int i = 0; i < 2; i++)
			SpiralBullet[i].Update();

		if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(SpiralBullet[1])))
			BulletWave = SECOND_A;

		break;

	case SECOND_A:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			SpreadBullet.bRelease = true;
		}

		SpreadBullet.Update();

		if (IsBulletSequenceComplete(dynamic_cast<const BulletPatternGenerator&>(SpreadBullet)))
			BulletWave = THIRD;

		break;

	case THIRD:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			SpiralMultiBullet.bRelease = true;
		}

		SpiralMultiBullet.Update();

		if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(SpiralMultiBullet)))
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

		for (int i = 0; i < 2; i++)
		{
			RageBullet[i].Center = SpawnLocation;
			RageBullet[i].bRelease = true;
		
			RageBullet[i].Update();
		}

		if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(RageBullet[1])))
			if (!bIsDead)
			{
				BulletWave = RAGE;

				for (int i = 0; i < 2; i++)
					RageBullet[i].Init();

				FinalBullets = &RageBullet[1].Bullet;

				for (int i = 0; i < 2; i++)
					for (unsigned short j = 0; j < RageBullet[i].Bullet.size(); j++)
					{
						RageBullet[i].Bullet[j].Player = Player;
						RageBullet[i].Bullet[j].Frames = 4;
						RageBullet[i].Bullet[j].Sprite = GetAsset(PurpleBullet);
						
						RageBullet[i].Bullet[j].InitFrames();
					}
			}
		break;

	default:
		break;
	}
}

void Alien::DrawBullet()
{
	switch (BulletWave)
	{
	case FIRST:
		LinearBullet.Draw();
		break;

	case FIRST_A:
		LinearMultiBullet[0].Draw();
		break;

	case FIRST_B:
		LinearMultiBullet[1].Draw();
		break;

	case SECOND:
		for (int i = 0; i < 2; i++)
			SpiralBullet[i].Draw();
		break;

	case SECOND_A:
		SpreadBullet.Draw();
		break;

	case THIRD:
		SpiralMultiBullet.Draw();
		break;

	case RAGE:
		for (int i = 0; i < 2; i++)
			RageBullet[i].Draw();
		break;

	default:
		break;
	}
}

void Alien::IncreasePlayerScore()
{
	Player->Score += GetRandomValue(40, 50);
}

