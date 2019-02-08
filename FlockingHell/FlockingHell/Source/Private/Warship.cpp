#include "Globals.h"
#include "Warship.h"
#include "Player.h"
#include "Assets.h"

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

Warship::Warship()
{
	Location = {400, -400};
	Health = 10000;
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
	bDebug = true;

	// WAVE 1
	for (int i = 0; i < 4; i++)
	{
		LinearBullet[i].SetBulletPattern(BulletPatternGenerator::LINEAR_LOCK_ON);
		LinearBullet[i].SetDelayAmount(i * 0.1f);
		LinearBullet[i].Enemy = this;
		LinearBullet[i].Center = { Location.x + SpawnLocation[i].x, Location.y + SpawnLocation[i].y };
		LinearBullet[i].Init();

		for (unsigned short j = 0; j < LinearBullet[i].Bullet.size(); j++)
		{
			LinearBullet[i].Bullet[j].Player = Player;
			LinearBullet[i].Bullet[j].Location = SpawnLocation[i];
			LinearBullet[i].Bullet[j].Frames = 1;
			LinearBullet[i].Bullet[j].Sprite = GetAsset(RedBullet);

			LinearBullet[i].Bullet[j].InitFrames();
		}
	}

	// WAVE 2
	SpiralMultiWayBullet[0].SetBulletPattern(BulletPatternGenerator::SPIRAL_MULTI_THREE_WAY);
	SpiralMultiWayBullet[1].SetBulletPattern(BulletPatternGenerator::SPIRAL_MULTI_FIVE_WAY);
	SpiralMultiWayBullet[2].SetBulletPattern(BulletPatternGenerator::SPIRAL_MULTI_SEVEN_WAY);

	for (int i = 0; i < 3; i++)
	{
		SpiralMultiWayBullet[i].SetDelayAmount(0.0f);
		SpiralMultiWayBullet[i].Enemy = this;
		SpiralMultiWayBullet[i].Center = {Location.x + CanonSpawnLocation.x, Location.y + CanonSpawnLocation.y};
		SpiralMultiWayBullet[i].Init();

		for (unsigned short j = 0; j < SpiralMultiWayBullet[i].Bullet.size(); j++)
		{
			SpiralMultiWayBullet[i].Bullet[j].Player = Player;
			SpiralMultiWayBullet[i].Bullet[j].Location = CanonSpawnLocation;
			SpiralMultiWayBullet[i].Bullet[j].Frames = 1;
			SpiralMultiWayBullet[i].Bullet[j].Sprite = GetAsset(RedBullet);

			SpiralMultiWayBullet[i].Bullet[j].InitFrames();			
		}
	}

	BulletWave = FIRST;

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
		Player->BossKilled++;
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
					Player->Score += GetRandomValue(50, 80);

					Health -= Player->BulletDamage;
				}
}
