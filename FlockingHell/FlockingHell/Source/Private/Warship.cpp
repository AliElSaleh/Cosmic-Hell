#include "Globals.h"
#include "Warship.h"
#include "Player.h"
#include "Assets.h"
#include <raymath.h>
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
	Hitbox = {Location.x + 96.0f, Location.y + 50.0f, 90.0f, 80.0f};

	CanonSpawnLocation = { Location.x + float(Sprite.width) / Frames / 2, Location.y + float(Sprite.height) - 20.0f };

	SpawnLocation[0] = { Location.x + 115.0f, Location.y + float(Sprite.height) };
	SpawnLocation[1] = { Location.x + 145.0f, Location.y + float(Sprite.height) };
	SpawnLocation[2] = { Location.x + 290.0f, Location.y + float(Sprite.height) };
	SpawnLocation[3] = { Location.x + 320.0f, Location.y + float(Sprite.height) };

	HitboxOffset = {50.0f, 105.0f};

	MaxVelocity = 1.5f;
	MaxForce = 0.5f;
	Mass = 100.0f; // 30Kg
	TargetRadius = 10.0f;

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

	BulletWave = FIRST;

	SetDestLocation({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH - Sprite.width/Frames)), float(GetRandomValue(0, 150))});
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
			Seek({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH - Sprite.width / Frames)), float(GetRandomValue(0, 150))});

		// Update sprite box location
		SpriteBox.x = Location.x;
		SpriteBox.y = Location.y;

		// Update hitbox location
		Hitbox.x = Location.x + 96.0f;
		Hitbox.y = Location.y + 50.0f;

		// Middle
		CanonSpawnLocation = { Location.x + float(Sprite.width) / Frames / 2, Location.y + float(Sprite.height) - 20.0f };

		// Left to right
		SpawnLocation[0] = { Location.x + 115.0f, Location.y + float(Sprite.height) };
		SpawnLocation[1] = { Location.x + 145.0f, Location.y + float(Sprite.height) };
		SpawnLocation[2] = { Location.x + 290.0f, Location.y + float(Sprite.height) };
		SpawnLocation[3] = { Location.x + 320.0f, Location.y + float(Sprite.height) };

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
		DrawRectangle(int(Hitbox.x), int(Hitbox.y), int(Hitbox.width), int(Hitbox.height), GRAY); // Hitbox

		for (int i = 0; i < 4; i++)
			DrawCircle(int(SpawnLocation[i].x), int(SpawnLocation[i].y), 3.0f, YELLOW);
	}

	DrawBullet();
}

void Warship::UpdateBullet()
{
	// Linear Bullet
	for (unsigned short i = 0; i < 4; i++)
	{
		LinearBullet[i].Location = SpawnLocation[i];
		LinearBullet[i].TargetLocation = Player->Center;
		LinearBullet[i].UpdateAnimation();
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
			SetDestLocation({ float(GetRandomValue(0, GetScreenWidth() - PANEL_WIDTH - Sprite.width / Frames)), float(GetRandomValue(0, 150)) });
		}
		break;

	case FIRST_B:
		break;

	case FIRST_C:
		break;

	case SECOND:
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
		break;

	case FIRST_B:
		break;

	case FIRST_C:
		break;

	case SECOND:
		break;

	case RAGE:
		break;

	default:
		break;
	}
}
