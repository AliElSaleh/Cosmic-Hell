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

	Health = 7000;
	LowHealthThreshold = 500;
	Explosions = 3;

	bDebug = false;
}

void Alien::Init()
{
	Sprite = GetAsset(Alien);
	Frames = 5;
	FramesSpeed = 10;

	SpawnLocation = {Location.x + 27.0f, Location.y + float(Sprite.height) - 45.0f};
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
	bActive = true;
	bIsDead = false;

	FrameRec.x = 0.0f;
	FrameRec.y = 0.0f;
	FrameRec.width = float(Sprite.width)/Frames;
	FrameRec.height = float(Sprite.height);

	for (int i = 0; i < 20; i++)
		DeathExplosion[i].Init();

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

	// WAVE 2
	SpiralBullet[0].SetBulletPattern(BulletPatternGenerator::SPIRAL_MULTI_RIGHT);
	SpiralBullet[1].SetBulletPattern(BulletPatternGenerator::SPIRAL_MULTI_LEFT);

	for (int i = 0; i < 2; i++)
	{
		SpiralBullet[i].SetDelayAmount(0.0f);
		SpiralBullet[i].Enemy = this;
		SpiralBullet[i].Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
		SpiralBullet[i].Init();

		for (unsigned short j = 0; j < SpiralBullet[i].Bullet.size(); j++)
		{
			SpiralBullet[i].Bullet[j].Player = Player;
			SpiralBullet[i].Bullet[j].Frames = 4;
			SpiralBullet[i].Bullet[j].Sprite = GetAsset(PurpleBullet);

			SpiralBullet[i].Bullet[j].InitFrames();
		}
	}

	// WAVE 3
	SpiralMultiBullet.SetBulletPattern(BulletPatternGenerator::SPIRAL_MULTI_FIVE_WAY);
	SpiralMultiBullet.SetDelayAmount(0.0f);
	SpiralMultiBullet.Enemy = this;
	SpiralMultiBullet.Init();

	for (unsigned short j = 0; j < SpiralMultiBullet.Bullet.size(); j++)
	{
		SpiralMultiBullet.Bullet[j].Player = Player;
		SpiralMultiBullet.Bullet[j].Frames = 4;
		SpiralMultiBullet.Bullet[j].Sprite = GetAsset(PurpleBullet);

		SpiralMultiBullet.Bullet[j].InitFrames();
	}

	// RAGE
	RageBullet[0].SetBulletPattern(BulletPatternGenerator::RANDOM_SPIRAL_MULTI);
	RageBullet[1].SetBulletPattern(BulletPatternGenerator::RANDOM_AIMING);

	for (int i = 0; i < 2; i++)
	{
		RageBullet[i].SetDelayAmount(0.0f);
		RageBullet[i].Enemy = this;
		RageBullet[i].Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
		RageBullet[i].Init();

		for (unsigned short j = 0; j < RageBullet[i].Bullet.size(); j++)
		{
			RageBullet[i].Bullet[j].Player = Player;
			RageBullet[i].Bullet[j].Frames = 4;
			RageBullet[i].Bullet[j].Sprite = GetAsset(PurpleBullet);
					  
			RageBullet[i].Bullet[j].InitFrames();	
		}
	}

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

		SpawnLocation = {Location.x + 27.0f, Location.y + float(Sprite.height) - 45.0f};

		UpdateAnimation();
	}

	if(bIsDead)
	{
		Player->BossKilled++;
		Player->BulletLevel = 3;
		Player->bChangeMusic = true;
		
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
		DrawTextureRec(Sprite, FrameRec, Location, WHITE);

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

void Alien::UpdateBullet()
{
	// WAVE 1
	LinearBullet.Location = SpawnLocation;
	LinearBullet.TargetLocation = Player->Center;
	LinearBullet.UpdateAnimation();

	// WAVE 2
	for (int i = 0; i < 2; i++)
	{
		SpiralBullet[i].Location = SpawnLocation;
		SpiralBullet[i].TargetLocation = Player->Center;
		SpiralBullet[i].UpdateAnimation();
	}

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
						RageBullet[i].Bullet[i].Frames = 4;
						RageBullet[i].Bullet[i].Sprite = GetAsset(PurpleBullet);
						
						RageBullet[i].Bullet[i].InitFrames();
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

	case SECOND:
		for (int i = 0; i < 2; i++)
			SpiralBullet[i].Draw();
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

