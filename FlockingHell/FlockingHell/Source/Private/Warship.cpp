#include "Globals.h"
#include "Warship.h"
#include "Player.h"
#include "Assets.h"

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

Warship::Warship()
{
	Location = {400, -400};
	Health = 30000;
	LowHealthThreshold = 1000;
	Frames = 6;
	Explosions = 6;

	Warship::Init();
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
	Mass = 100.0f; // 30Kg
	TargetRadius = 20.0f;

	Speed = 100.0f;
	Damage = GetRandomValue(20, 30);
	ShootRate = 5;

	Frames = 6;
	FrameRec.x = 0.0f;
	FrameRec.y = 0.0f;
	FrameRec.width = float(Sprite.width)/Frames;
	FrameRec.height = float(Sprite.height);

	bActive = true;
	bIsDead = false;
	bDebug = false;

	// WAVE 1
	for (int i = 0; i < 4; i++)
		InitBullet(LinearBullet[i], SpawnLocation[i], i * 0.1f, BulletPatternGenerator::LINEAR_LOCK_ON);

	// WAVE 2
	InitBullet(SpiralMultiWayBullet[0], CanonSpawnLocation, 0.0f, BulletPatternGenerator::SPIRAL_MULTI_THREE_WAY);
	InitBullet(SpiralMultiWayBullet[1], CanonSpawnLocation, 0.0f, BulletPatternGenerator::SPIRAL_MULTI_FIVE_WAY);
	InitBullet(SpiralMultiWayBullet[2], CanonSpawnLocation, 0.0f, BulletPatternGenerator::SPIRAL_MULTI_SEVEN_WAY);

	BulletWave = FIRST;

	FinalBullets = {};

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
		DrawTextureRec(Sprite, FrameRec, Location, WHITE);

	if (bDebug && bActive && !bIsDead)
	{
		DrawRectangleLines(int(SpriteBox.x), int(SpriteBox.y), int(SpriteBox.width), int(SpriteBox.height), WHITE); // A rectangle that its width/height is the same as the sprite's width/height

		for (int i = 0; i < 2; i++)
			DrawRectangle(int(Hitbox[i].x), int(Hitbox[i].y), int(Hitbox[i].width), int(Hitbox[i].height), GRAY); // Hitbox

		DrawCircle(int(CanonSpawnLocation.x), int(CanonSpawnLocation.y), 5.0f, RED); // The big canon in the middle of the warship

		for (int i = 0; i < 4; i++)
			DrawCircle(int(SpawnLocation[i].x), int(SpawnLocation[i].y), 3.0f, YELLOW); // The 4 small canons on warship
	}

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
	{
		LinearBullet[i].Location = SpawnLocation[i];
		LinearBullet[i].TargetLocation = Player->Center;
		LinearBullet[i].UpdateAnimation();
	}

	// WAVE 2
	for (unsigned short i = 0; i < 3; i++)
	{
		SpiralMultiWayBullet[i].Location = CanonSpawnLocation;
		SpiralMultiWayBullet[i].TargetLocation = Player->Center;
		SpiralMultiWayBullet[i].UpdateAnimation();
	}

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
			SetDestLocation({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH - Sprite.width/Frames)), float(GetRandomValue(0, 100))});
		}

		break;

	case RAGE:
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
		break;

	case RAGE:
		break;

	default:
		break;
	}
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
					
					Health -= Player->Bullet[0].Damage;
				}
}

void Warship::IncreasePlayerScore()
{
	Player->Score += GetRandomValue(50, 60);
}
