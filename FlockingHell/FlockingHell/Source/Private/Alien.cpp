#include "Alien.h"
#include "Assets.h"
#include "Globals.h"
#include "Player.h"

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

Alien::Alien()
{
	Alien::Init();

	Location = {300.0f, -300.0f};
	Frames = 1;
	ShootRate = 5;

	bDebug = false;
}

void Alien::Init()
{
	Sprite = GetAsset(Alien);

	SpawnLocation = {Location.x + 27.0f, Location.y + float(Sprite.height) - 45.0f};
	HitboxOffset = {32.0f, 2.0f};
	Hitbox = {Location.x + HitboxOffset.x, Location.y + HitboxOffset.y, float(Sprite.width)/Frames-60.0f, float(Sprite.height)/5};
	SpriteBox = {Location.x, Location.y, float(Sprite.width)/Frames, float(Sprite.height)};
	MaxVelocity = 2.0f;
	MaxForce = 5.0f;
	Mass = 40.0f; // 40Kg
	TargetRadius = 10.0f;
	
	Health = 3000;
	LowHealthThreshold = 500;
	Explosions = 3;
	Speed = 100.0f;
	Damage = GetRandomValue(1, 3);
	bActive = true;
	bIsDead = false;

	// WAVE 1
	LinearBullet.SetBulletPattern(BulletPatternGenerator::LINEAR_AIMING);
	LinearBullet.SetDelayAmount(0.0f);
	LinearBullet.Enemy = this;
	LinearBullet.Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
	LinearBullet.Init();

	for (unsigned short i = 0; i < LinearBullet.Bullet.size(); i++)
	{
		LinearBullet.Bullet[i].Player = Player;
		LinearBullet.Bullet[i].Frames = 4;
		LinearBullet.Bullet[i].Sprite = GetAsset(PurpleBullet);
		
		LinearBullet.Bullet[i].InitFrames();
	}

	// RAGE
	RageBullet.SetBulletPattern(BulletPatternGenerator::RANDOM_SPIRAL_MULTI);
	RageBullet.SetDelayAmount(0.0f);
	RageBullet.Enemy = this;
	RageBullet.Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
	//RageBullet.Init();

	//for (unsigned short i = 0; i < RageBullet.Bullet.size(); i++)
	//{
	//	RageBullet.Bullet[i].Player = Player;
	//	RageBullet.Bullet[i].Frames = 4;
	//	RageBullet.Bullet[i].Sprite = GetAsset(PurpleBullet);
	//
	//	RageBullet.Bullet[i].InitFrames();
	//}

	FinalBullets = &RageBullet.Bullet;

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

		SpawnLocation = {Location.x + 27.0f, Location.y + float(Sprite.height) - 45.0f};
	}

	if(bIsDead)
		for (int i = 0; i < 20; i++)
			DeathExplosion[i].Explode({float(GetRandomValue(int(Location.x), int(Location.x) + Sprite.width/Frames)), float(GetRandomValue(int(Location.y), int(Location.y) + Sprite.height))}, Explosions);

	UpdateBullet();

	CheckCollisionWithPlayer();
	CheckCollisionWithPlayerBullets();
	CheckHealth();
}

void Alien::Draw()
{
	if (bDebug)
		DrawRectangle(int(SpriteBox.x), int(SpriteBox.y), int(SpriteBox.width), int(SpriteBox.height), WHITE); // Sprite box

	if (!bIsDead && bActive)
		DrawTexture(Sprite, int(Location.x), int(Location.y), WHITE);

	if (bDebug)
	{
		DrawRectangle(int(Hitbox.x), int(Hitbox.y), int(Hitbox.width), int(Hitbox.height), RED); // Hitbox
		DrawCircle(int(SpawnLocation.x), int(SpawnLocation.y), 3.0f, YELLOW);
	}

	if (bIsDead)
		for (int i = 0; i < 20; i++)
			DeathExplosion[i].Draw();

	DrawBullet();
}

void Alien::UpdateBullet()
{
	LinearBullet.Location = SpawnLocation;
	LinearBullet.TargetLocation = Player->Center;

	for (unsigned short i = 0; i < LinearBullet.Bullet.size(); i++)
		LinearBullet.Bullet[i].UpdateAnimation();

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
			BulletWave = SECOND;

		break;

	case SECOND:
		if (IsAtLocation(Destination))
		{
			SetDestLocation({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH - Sprite.width/Frames)), float(GetRandomValue(0, 100))});
		}

		break;

	case THIRD:
		break;

	case RAGE:
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

	case SECOND:
		break;

	case THIRD:
		break;

	case RAGE:
		break;

	default:
		break;
	}
}

