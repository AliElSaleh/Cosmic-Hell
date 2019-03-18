#include "Globals.h"
#include "Warship.h"
#include "Player.h"
#include "Assets.h"

#include <algorithm>

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

Warship::Warship()
{
	Location = {400, -400};
	Health = 30000;
	LowHealthThreshold = 1000;
	Frames = 6;
	Explosions = 6;

	HealthBar = {20.0f, GetScreenHeight() - 30.0f, GetScreenWidth() - PANEL_WIDTH - 40.0f, 5.0f, Health, RED};
}

void Warship::Init()
{
	Sprite = GetAsset(Warship);

	SpriteBox = {Location.x, Location.y, float(Sprite.width)/Frames, float(Sprite.height)};

	Hitbox[0] = {Location.x + 96.0f, Location.y + 50.0f, 90.0f, 80.0f};
	Hitbox[1] = {Location.x + 260.0f, Location.y + 50.0f, 90.0f, 80.0f};

	CanonSpawnLocation = { Location.x + float(Sprite.width) / Frames / 2 + 30.0f, Location.y + float(Sprite.height) - 10.0f };

	SpawnLocation[0] = { Location.x + 115.0f, Location.y + float(Sprite.height) };
	SpawnLocation[1] = { Location.x + 145.0f, Location.y + float(Sprite.height) };
	SpawnLocation[2] = { Location.x + 290.0f, Location.y + float(Sprite.height) };
	SpawnLocation[3] = { Location.x + 320.0f, Location.y + float(Sprite.height) };

	HitboxOffset = {50.0f, 105.0f};

	MaxVelocity = 1.5f;
	MaxForce = 0.5f;
	Mass = 100.0f; // 100Kg
	TargetRadius = 20.0f;

	Speed = 100.0f;
	Damage = GetRandomValue(20, 30);
	ShootRate = 5;

	for (int i = 0; i < 20; i++)
		DeathExplosion[i].Init();

	Frames = 6;
	FrameRec.x = 0.0f;
	FrameRec.y = 0.0f;
	FrameRec.width = float(Sprite.width)/Frames;
	FrameRec.height = float(Sprite.height);

	bIsBoss = true;
	bActive = true;
	bIsDead = false;
	bDebug = false;

	// WAVE 1
	for (int i = 0; i < 4; i++)
		InitBullet(LinearBullet[i], SpawnLocation[i], i * 0.1f, BulletPatternGenerator::LINEAR_LOCK_ON);

	InitBullet(SpiralMultiWayBullet[0], CanonSpawnLocation, 0.0f, BulletPatternGenerator::SPIRAL_MULTI_THREE_WAY);
	InitBullet(SpiralMultiWayBullet[1], CanonSpawnLocation, 0.0f, BulletPatternGenerator::SPIRAL_MULTI_FIVE_WAY);
	InitBullet(SpiralMultiWayBullet[2], CanonSpawnLocation, 0.0f, BulletPatternGenerator::SPIRAL_MULTI_SEVEN_WAY);

	// WAVE 2
	for (int i = 0; i < 4; i++)
		InitBullet(LinearAimingBullet[i], SpawnLocation[i], i * 0.1f, BulletPatternGenerator::LINEAR_LOCK_ON);
	InitBullet(LinearAimingBullet[4], CanonSpawnLocation, 0.0f, BulletPatternGenerator::LINEAR_AIMING);

	// 2A
	for (int i = 0; i < 5; i++)
		InitBullet(CircleBullet[i], CanonSpawnLocation, i * 0.1f, BulletPatternGenerator::CIRCLE);

	// 2B
	InitBullet(SpiralOneWayBullet[0], CanonSpawnLocation, 0.0f, BulletPatternGenerator::SPIRAL_FOUR_WAY);

	// 2C
	InitBullet(SpiralOneWayBullet[1], CanonSpawnLocation, 0.0f, BulletPatternGenerator::SPIRAL_EIGHT_WAY);

	// WAVE 3
	InitBullet(SpiralMultiBullet[0], SpawnLocation[0], 0.0f, BulletPatternGenerator::SPIRAL_MULTI_RIGHT);
	InitBullet(SpiralMultiBullet[1], SpawnLocation[1], 0.0f, BulletPatternGenerator::SPIRAL_MULTI_LEFT);
	InitBullet(SpiralMultiBullet[2], SpawnLocation[2], 0.0f, BulletPatternGenerator::SPIRAL_MULTI_RIGHT);
	InitBullet(SpiralMultiBullet[3], SpawnLocation[3], 0.0f, BulletPatternGenerator::SPIRAL_MULTI_LEFT);

	// 3A
	for (int i = 0; i < 4; i++)
		InitBullet(LinearMultiBullet[i], SpawnLocation[i], 0.0f, BulletPatternGenerator::SIX_WAY_LINEAR);

	// 3B
	for (int i = 5; i < 9; i++)
		InitBullet(LinearAimingBullet[i], SpawnLocation[i-5], 0.0f, BulletPatternGenerator::LINEAR_AIMING);

	// RAGE
	for (int i = 0; i < 4; i++)
		InitBullet(RageBullet[i], SpawnLocation[i], 0.0f, BulletPatternGenerator::RANDOM_SPIRAL_MULTI);

	BulletWave = FIRST;

	FinalBullets = &RageBullet[3].Bullet;

	SetDestLocation({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH - Sprite.width/Frames)), float(GetRandomValue(0, 100))});
}

void Warship::Update()
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
			Seek({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH - Sprite.width / Frames)), float(GetRandomValue(0, 100))});

		// Update sprite box location
		SpriteBox.x = Location.x;
		SpriteBox.y = Location.y;

		// Update hitbox location
		Hitbox[0] = {Location.x + 96.0f, Location.y + 50.0f, 90.0f, 80.0f};
		Hitbox[1] = {Location.x + 260.0f, Location.y + 50.0f, 90.0f, 80.0f};

		// Middle
		CanonSpawnLocation = {Location.x + float(Sprite.width) / Frames / 2 + 25.0f, Location.y + float(Sprite.height) - 10.0f};

		// Left to right
		SpawnLocation[0] = {Location.x + 115.0f, Location.y + float(Sprite.height)};
		SpawnLocation[1] = {Location.x + 145.0f, Location.y + float(Sprite.height)};
		SpawnLocation[2] = {Location.x + 290.0f, Location.y + float(Sprite.height)};
		SpawnLocation[3] = {Location.x + 320.0f, Location.y + float(Sprite.height)};

		UpdateAnimation();
	}

	if (bIsDead)
	{
		const auto Predicate = [](const Bullet &b) { return !b.bActive; };

		for (int i = 0; i < 4; i++)
			RageBullet[i].Bullet.erase(std::remove_if(RageBullet[i].Bullet.begin(), RageBullet[i].Bullet.end(), Predicate), RageBullet[i].Bullet.end());
		
		for (int i = 0; i < 20; i++)
			DeathExplosion[i].Explode({float(GetRandomValue(int(Location.x), int(Location.x) + Sprite.width/Frames)), float(GetRandomValue(int(Location.y), int(Location.y) + Sprite.height))}, Explosions);
	}

	UpdateBullet();

	CheckCollisionWithPlayer();
	CheckCollisionWithPlayerBullets();
	CheckHealth();
}

void Warship::Draw()
{
	// Draw the warship sprite
	if (bActive && !bIsDead)
	{
		DrawTextureRec(Sprite, FrameRec, Location, WHITE);
		HealthBar.Draw();
	}

	if (bDebug && bActive && !bIsDead)
	{
		DrawRectangleLines(int(SpriteBox.x), int(SpriteBox.y), int(SpriteBox.width), int(SpriteBox.height), WHITE); // A rectangle that its width/height is the same as the sprite's width/height

		for (int i = 0; i < 2; i++)
			DrawRectangle(int(Hitbox[i].x), int(Hitbox[i].y), int(Hitbox[i].width), int(Hitbox[i].height), GRAY); // Hitbox

		DrawCircle(int(CanonSpawnLocation.x), int(CanonSpawnLocation.y), 5.0f, RED); // The big canon in the middle of the warship

		for (int i = 0; i < 4; i++)
			DrawCircle(int(SpawnLocation[i].x), int(SpawnLocation[i].y), 3.0f, YELLOW); // The 4 small canons on warship
		
		RDrawText(FormatText("Health: %i", Health), 10, 750, 20, WHITE);
	}
	
	if (bIsDead)
		for (int i = 0; i < 20; i++)
			DeathExplosion[i].Draw();

	DrawBullet();
}

void Warship::InitBullet(BulletPatternGenerator& BulletPattern, const Vector2& SpawnLocation, const float Delay, const BulletPatternGenerator::Pattern Pattern)
{
	BulletPattern.SetBulletPattern(Pattern);
	BulletPattern.SetDelayAmount(Delay);
	BulletPattern.Enemy = this;
	BulletPattern.Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
	BulletPattern.Init();

	for (unsigned short j = 0; j < BulletPattern.Bullet.size(); j++)
	{
		BulletPattern.Bullet[j].Player = Player;
		BulletPattern.Bullet[j].Location = SpawnLocation;
		BulletPattern.Bullet[j].Frames = 1;
		BulletPattern.Bullet[j].Sprite = GetAsset(RedBullet);
	
		BulletPattern.Bullet[j].InitFrames();
	}
}

void Warship::UpdateBullet()
{
	// WAVE 1
	for (unsigned short i = 0; i < 4; i++)
		UpdateBulletComponents(LinearBullet[i], SpawnLocation[i], Player->Center);

	for (unsigned short i = 0; i < 3; i++)
		UpdateBulletComponents(SpiralMultiWayBullet[i], CanonSpawnLocation, Player->Center);

	// WAVE 2
	for (unsigned short i = 0; i < 4; i++)
		UpdateBulletComponents(LinearAimingBullet[i], SpawnLocation[i], Player->Center);

	UpdateBulletComponents(LinearAimingBullet[4], CanonSpawnLocation, Player->Center);

	for (unsigned short i = 0; i < 5; i++)
		UpdateBulletComponents(CircleBullet[i], CanonSpawnLocation, Player->Center);

	for (unsigned short i = 0; i < 2; i++)
		UpdateBulletComponents(SpiralOneWayBullet[i], CanonSpawnLocation, Player->Center);

	// WAVE 3
	for (unsigned short i = 0; i < 4; i++)
		UpdateBulletComponents(SpiralMultiBullet[i], SpawnLocation[i], Player->Center);

	for (unsigned short i = 0; i < 4; i++)
		UpdateBulletComponents(LinearMultiBullet[i], SpawnLocation[i], Player->Center);

	for (unsigned short i = 5; i < 9; i++)
		UpdateBulletComponents(LinearAimingBullet[i], SpawnLocation[i-5], Player->Center);

	// RAGE
	for (unsigned short i = 0; i < 4; i++)
		UpdateBulletComponents(RageBullet[i], SpawnLocation[i], Player->Center);

	switch (BulletWave)
	{
	case FIRST:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			for (int i = 0; i < 4; i++)
				LinearBullet[i].bRelease = true;
		}

		for (int i = 0; i < 4; i++)
			LinearBullet[i].Update();

		if (IsBulletSequenceComplete(dynamic_cast<const BulletPatternGenerator&>(LinearBullet[3])))
			BulletWave = FIRST_A;

		break;

	case FIRST_A:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			SpiralMultiWayBullet[0].bRelease = true;
		}

		SpiralMultiWayBullet[0].Update();

		if (IsBulletSequenceComplete(dynamic_cast<const BulletPatternGenerator&>(SpiralMultiWayBullet[0])))
			BulletWave = FIRST_B;
		break;

	case FIRST_B:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			SpiralMultiWayBullet[1].bRelease = true;
		}

		SpiralMultiWayBullet[1].Update();

		if (IsBulletSequenceComplete(dynamic_cast<const BulletPatternGenerator&>(SpiralMultiWayBullet[1])))
			BulletWave = FIRST_C;
		break;

	case FIRST_C:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			SpiralMultiWayBullet[2].bRelease = true;
		}

		SpiralMultiWayBullet[2].Update();

		if (IsBulletSequenceComplete(dynamic_cast<const BulletPatternGenerator&>(SpiralMultiWayBullet[2])))
			BulletWave = SECOND;
		break;

	case SECOND:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			for (int i = 0; i < 5; i++)
				LinearAimingBullet[i].bRelease = true;
		}

		for (int i = 0; i < 5; i++)
			LinearAimingBullet[i].Update();

		if (IsBulletSequenceComplete(dynamic_cast<const BulletPatternGenerator&>(LinearAimingBullet[4])))
			BulletWave = SECOND_A;

		break;

	case SECOND_A:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			for (int i = 0; i < 5; i++)
				CircleBullet[i].bRelease = true;
		}

		for (int i = 0; i < 5; i++)
			CircleBullet[i].Update();

		if (IsBulletSequenceComplete(dynamic_cast<const BulletPatternGenerator&>(CircleBullet[4])))
			BulletWave = SECOND_B;

		break;

	case SECOND_B:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			SpiralOneWayBullet[0].bRelease = true;
		}

		SpiralOneWayBullet[0].Update();

		if (IsBulletSequenceComplete(dynamic_cast<const BulletPatternGenerator&>(SpiralOneWayBullet[0])))
			BulletWave = SECOND_C;
		break;

	case SECOND_C:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			SpiralOneWayBullet[1].bRelease = true;
		}

		SpiralOneWayBullet[1].Update();

		if (IsBulletSequenceComplete(dynamic_cast<const BulletPatternGenerator&>(SpiralOneWayBullet[1])))
			BulletWave = THIRD;
		break;

	case THIRD:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			for (unsigned short i = 0; i < 4; i++)
				SpiralMultiBullet[i].bRelease = true;
		}

		for (unsigned short i = 0; i < 4; i++)
			SpiralMultiBullet[i].Update();

		if (IsBulletSequenceComplete(dynamic_cast<const BulletPatternGenerator&>(SpiralMultiBullet[3])))
			BulletWave = THIRD_A;
		break;

	case THIRD_A:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			for (unsigned short i = 0; i < 4; i++)
				LinearMultiBullet[i].bRelease = true;
		}

		for (unsigned short i = 0; i < 4; i++)
			LinearMultiBullet[i].Update();

		if (IsBulletSequenceComplete(dynamic_cast<const BulletPatternGenerator&>(LinearMultiBullet[3])))
			BulletWave = THIRD_B;

		break;

	case THIRD_B:
		if (IsAtLocation(Destination))
		{
			StopMoving();

			for (unsigned short i = 5; i < 9; i++)
				LinearAimingBullet[i].bRelease = true;
		}

		for (unsigned short i = 5; i < 9; i++)
			LinearAimingBullet[i].Update();

		if (IsBulletSequenceComplete(dynamic_cast<const BulletPatternGenerator&>(LinearAimingBullet[8])))
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

		for (int i = 0; i < 4; i++)
		{
			RageBullet[i].Center = SpawnLocation[i];
			RageBullet[i].bRelease = true;
			RageBullet[i].Update();
		}
		
		if (IsBulletSequenceComplete(dynamic_cast<BulletPatternGenerator&>(RageBullet[3])))
			if (!bIsDead)
			{
				BulletWave = RAGE;

				for (int i = 0; i < 4; i++)
				{
					RageBullet[i].Init();

					for (unsigned short j = 0; j < RageBullet[i].Bullet.size(); j++)
					{
						RageBullet[i].Bullet[j].Player = Player;
						RageBullet[i].Bullet[j].Frames = 1;
						RageBullet[i].Bullet[j].Sprite = GetAsset(RedBullet);
						
						RageBullet[i].Bullet[j].InitFrames();
					}
				}
			}

		break;

	default:
		break;
	}
}

void Warship::DrawBullet()
{
	switch (BulletWave)
	{
	case FIRST:
		for (int i = 0; i < 4; i++)
			LinearBullet[i].Draw();
		break;

	case FIRST_A:
		SpiralMultiWayBullet[0].Draw();
		break;

	case FIRST_B:
		SpiralMultiWayBullet[1].Draw();
		break;

	case FIRST_C:
		SpiralMultiWayBullet[2].Draw();
		break;

	case SECOND:
		for (int i = 0; i < 5; i++)
			LinearAimingBullet[i].Draw();
		break;

	case SECOND_A:
		for (int i = 0; i < 5; i++)
			CircleBullet[i].Draw();
		break;

	case SECOND_B:
		SpiralOneWayBullet[0].Draw();
		break;

	case SECOND_C:
		SpiralOneWayBullet[1].Draw();
		break;

	case THIRD:
		for (unsigned short i = 0; i < 4; i++)
			SpiralMultiBullet[i].Draw();
		break;

	case THIRD_A:
		for (unsigned short i = 0; i < 4; i++)
			LinearMultiBullet[i].Draw();
		break;

	case THIRD_B:
		for (unsigned short i = 5; i < 9; i++)
			LinearAimingBullet[i].Draw();
		break;

	case RAGE:
		for (int i = 0; i < 4; i++)
			RageBullet[i].Draw();
		break;

	default:
		break;
	}
}

void Warship::UpdateBulletComponents(BulletPatternGenerator& BulletPattern, const Vector2 & Location, const Vector2 & TargetLocation) const
{
	BulletPattern.Location = Location;
	BulletPattern.TargetLocation = TargetLocation;
	BulletPattern.UpdateAnimation();
}

void Warship::CheckCollisionWithPlayerBullets()
{
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++)
		for (int j = 0; j < 2; j++)
			if (CheckCollisionCircleRec(Player->Bullet[i].Center, Player->Bullet[i].Radius, Hitbox[j]))
				if (bActive && !bIsDead)
				{
					Player->ResetBullet(i);
					IncreasePlayerScore();

					HealthBar.Update(Health);
						
					Health -= Player->Bullet[0].Damage;
				}
}

void Warship::IncreasePlayerScore()
{
	Player->Score += GetRandomValue(50, 60);
}
