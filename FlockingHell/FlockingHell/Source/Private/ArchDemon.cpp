#include "Globals.h"
#include "ArchDemon.h"
#include "Assets.h"
#include "Player.h"

#include <raymath.h>

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

ArchDemon::ArchDemon()
{
	Location = {float(GetRandomValue(-150, -GetScreenWidth()-PANEL_WIDTH-150)), float(GetRandomValue(-400, -700))};
	Velocity = {float(GetRandomValue(-2, 2)), float(GetRandomValue(-2, 2))};

	Health = 100;

	ArchDemon::Init();
}

void ArchDemon::Init()
{
	Sprite = GetAsset(ArchDemon);

	SpawnLocation = {Location.x, Location.y};

	MaxVelocity = 1.0f;
	MaxForce = 0.4f;
	Mass = 5.0f; // 5Kg
	TargetRadius = 20.0f;
	Frames = 10;

	HitboxOffset = {float(Sprite.width)/Frames / 2  - 15, float(Sprite.height) / 2 - 15};
	Hitbox = {Location.x, Location.y, float(Sprite.width)/Frames/2, float(Sprite.height)/2};
	SpriteBox = {Location.x, Location.y, float(Sprite.width)/Frames, float(Sprite.height)};

	Speed = 140.0f;
	Damage = GetRandomValue(1, 3);
	ShootRate = 5;

	FrameRec.x = 0.0f;
	FrameRec.y = 0.0f;
	FrameRec.width = float(Sprite.width)/Frames;
	FrameRec.height = float(Sprite.height);

	bActive = true;
	bIsDead = false;
	bDebug = false;

	DeathExplosion[0].Init();

	Explosions = 2;

	// Initialise bullets
	LinearBullet.SetBulletPattern(BulletPatternGenerator::LINEAR_LOCK_ON);
	LinearBullet.SetDelayAmount(0.0f);
	LinearBullet.Enemy = this;
	LinearBullet.Center = {Location.x + SpawnLocation.x, Location.y + SpawnLocation.y};
	LinearBullet.Init();

	for (unsigned short j = 0; j < LinearBullet.Bullet.size(); j++)
	{
		LinearBullet.Bullet[j].Player = Player;
		LinearBullet.Bullet[j].Frames = 6;
		LinearBullet.Bullet[j].Sprite = GetAsset(FireBullet);

		LinearBullet.Bullet[j].InitFrames();
	}

	FinalBullets = &LinearBullet.Bullet;

	BulletWave = FIRST;
}

void ArchDemon::Update()
{
	if (bActive && !bIsDead)
	{
		SpriteBox.x = Location.x;
		SpriteBox.y = Location.y;

		Hitbox = {Location.x + HitboxOffset.x, Location.y + HitboxOffset.y, float(Sprite.width)/Frames/2, float(Sprite.height)/2};

		SpawnLocation = {Location.x + HitboxOffset.x + 15, Location.y + HitboxOffset.y + 15};

		UpdateAnimation();
	}

	if (bIsDead)
	{
		Player->EnemiesKilled++;

		LinearBullet.bRelease = true;
		
		DeathExplosion[0].Explode({float(GetRandomValue(int(Location.x), int(Location.x) + Sprite.width/Frames)), float(GetRandomValue(int(Location.y), int(Location.y) + Sprite.height))}, Explosions);
	}

	UpdateBullet();

	CheckCollisionWithPlayerBullets();
	CheckHealth();
}

void ArchDemon::Draw()
{
	if (bActive && !bIsDead)
		DrawTextureRec(Sprite, FrameRec, Location, WHITE);

	if (bDebug)
	{
		DrawRectangleLines(int(Location.x), int(Location.y), Sprite.width/Frames, Sprite.height, RED); // Spritebox
		DrawRectangleRec(Hitbox, YELLOW);

		DrawCircle(int(SpawnLocation.x), int(SpawnLocation.y), 3.0f, BLUE); // Bullet spawn location

		DrawCircle(int(Destination.x), int(Destination.y), 5.0f, WHITE);
	}

	if(bIsDead)
		DeathExplosion[0].Draw();

	DrawBullet();
}

void ArchDemon::Flock(std::vector<Enemy*>* Boids)
{
	ApplyBehaviours(Boids);
}

void ArchDemon::ApplyBehaviours(std::vector<Enemy*>* Enemies)
{
	Vector2 Alignment = Align(Enemies);
	Vector2 Cohesion = Cohere(Enemies);
	Vector2 Separation = Separate(Enemies);
	Vector2 GoalSeeking = Seek(Destination);
	
	Alignment = Vector2Scale(Alignment, AlignmentForce);
	Cohesion = Vector2Scale(Cohesion, CohesionForce);
	Separation = Vector2Scale(Separation, SeparationForce);		// Arbitrary weight values
	GoalSeeking = Vector2Scale(GoalSeeking, GoalSeekForce);
	
	ApplyForce(Alignment);
	ApplyForce(Cohesion);
	ApplyForce(Separation);
	ApplyForce(GoalSeeking);
}

void ArchDemon::UpdateAnimation()
{
	SpriteFramesCounter++;

	if (Direction.x > 0)
	{
		// Arch Demon sprite animation
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
		// Arch Demon sprite animation
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

void ArchDemon::IncreasePlayerScore()
{
	Player->Score += GetRandomValue(10, 20);
}

void ArchDemon::UpdateBullet()
{
	LinearBullet.Location = SpawnLocation;
	LinearBullet.TargetLocation = Player->Center;
	LinearBullet.UpdateAnimation();

	switch (BulletWave)
	{
	case FIRST:
		FramesCounter++;

		if (FramesCounter/GetRandomValue(240, 360)%2)
			LinearBullet.bRelease = true;
			
		LinearBullet.Update();

		if (LinearBullet.Bullet.empty())
			if (!bIsDead)
			{
				Init();
				FramesCounter = 0;
				BulletWave = FIRST;
			}

		break;

	default:
		break;
	}
}

void ArchDemon::DrawBullet()
{
	switch (BulletWave)
	{
	case FIRST:
		LinearBullet.Draw();
		break;

	default:
		break;
	}
}
