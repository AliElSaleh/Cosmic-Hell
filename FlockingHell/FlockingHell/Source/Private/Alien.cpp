#include "Alien.h"
#include "Assets.h"
#include "Globals.h"

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
	MaxForce = 0.5f;
	Mass = 40.0f; // 40Kg
	TargetRadius = 10.0f;
	
	Health = 3000;
	LowHealthThreshold = 500;
	Explosions = 3;
	Speed = 100.0f;
	Damage = GetRandomValue(1, 3);
	bActive = true;
	bIsDead = false;

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

	SetDestLocation({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH - float(Sprite.width)/Frames)), float(GetRandomValue(0, 100))});
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
			DeathExplosion[i].Explode({float(GetRandomValue(Location.x, Location.x + float(Sprite.width)/Frames)), float(GetRandomValue(Location.y, Location.y + Sprite.height))}, Explosions);


	CheckCollisionWithPlayer();
	CheckCollisionWithPlayerBullets();
	CheckHealth();

	if (IsAtLocation(Destination))
		SetDestLocation({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH - float(Sprite.width)/Frames)), float(GetRandomValue(0, 100))});
}

void Alien::Draw()
{
	if (bDebug)
		DrawRectangle(SpriteBox.x, SpriteBox.y, SpriteBox.width, SpriteBox.height, WHITE); // Sprite box

	if (!bIsDead && bActive)
		DrawTexture(Sprite, int(Location.x), int(Location.y), WHITE);

	if (bDebug)
	{
		DrawRectangle(Hitbox.x, Hitbox.y, Hitbox.width, Hitbox.height, RED); // Hitbox
		DrawCircle(SpawnLocation.x, SpawnLocation.y, 3.0f, YELLOW);
		//DrawText(FormatText("Health: %02i", Health), GetScreenWidth()-PANEL_WIDTH - 150.0f, 10.0f, 20.0f, WHITE);
	}

	if (bIsDead)
		for (int i = 0; i < 20; i++)
			DeathExplosion[i].Draw();
}

