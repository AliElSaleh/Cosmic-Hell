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
	Hitbox = {Location.x + 96.0f, Location.y + 50.0f, 90.0f, 80.0f};

	SpawnLocation = {50.0f, 105.0f};
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

	SetDestLocation({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH)), float(GetRandomValue(0, 150))});
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
			Seek({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH)), float(GetRandomValue(0, 150))});

		// Update sprite box location
		SpriteBox.x = Location.x;
		SpriteBox.y = Location.y;

		// Update hitbox location
		Hitbox.x = Location.x + 96.0f;
		Hitbox.y = Location.y + 50.0f;

		SpawnLocation = {Location.x + 50.0f, Location.y + 105.0f};

		UpdateAnimation();
	}

	if (bIsDead)
	{
		Player->BossKilled++;
	}

	if (IsAtLocation(Destination))
		SetDestLocation({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH)), float(GetRandomValue(0, 150))});

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
	}

	DrawBullet();
}

void Warship::UpdateBullet()
{
}

void Warship::DrawBullet()
{
}
