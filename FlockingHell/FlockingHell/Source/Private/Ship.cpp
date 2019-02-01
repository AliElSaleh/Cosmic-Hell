#include "Globals.h"
#include "Ship.h"
#include "Assets.h"
#include "Player.h"

#include <raymath.h>

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

Ship::Ship()
{
	Location = {float(GetRandomValue(-150, -GetScreenWidth()-PANEL_WIDTH - 150)), float(GetRandomValue(-400, -700))};
	Velocity = {float(GetRandomValue(-2, 2)), float(GetRandomValue(-2, 2))};

	Health = 100;

	Ship::Init();
}

void Ship::Init()
{
	Sprite = GetAsset(Boid);

	SpawnLocation = {Location.x, Location.y};

	MaxVelocity = 1.0f;
	MaxForce = 0.4f;
	Mass = 10.0f; // 10Kg
	TargetRadius = 20.0f;

	if (Sprite.width % 5 == 0)
		Frames = 5;
	else if (Sprite.width % 4 == 0)
		Frames = 4;

	HitboxOffset = {float(Sprite.width)/Frames / 2, float(Sprite.height) / 2};
	Hitbox = {Location.x + HitboxOffset.x, Location.y + HitboxOffset.y, float(Sprite.width)/Frames, float(Sprite.height)/2};
	SpriteBox = {Location.x, Location.y, float(Sprite.width)/Frames, float(Sprite.height)};

	Speed = 100.0f;
	Damage = GetRandomValue(1, 3);
	ShootRate = 5;
	
	Origin.x = float(Sprite.width)/Frames / 2;
	Origin.y = float(Sprite.height) / 2;

	FrameRec.x = 0.0f;
	FrameRec.y = 0.0f;
	FrameRec.width = float(Sprite.width)/Frames;
	FrameRec.height = float(Sprite.height);

	DestFrameRec = {Location.x, Location.y, (float(Sprite.width)/Frames), float(Sprite.height)};

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
		LinearBullet.Bullet[j].Frames = 4;
		LinearBullet.Bullet[j].Sprite = GetAsset(GreenBullet);

		LinearBullet.Bullet[j].InitFrames();
	}

	FinalBullets = &LinearBullet.Bullet;
}

void Ship::Update()
{
	if (bActive && !bIsDead)
	{
		// Rotate towards the direction the ship is moving
		Rotation = atan2(Direction.y, Direction.x)*RAD2DEG;

		DestFrameRec.x = Location.x;
		DestFrameRec.y = Location.y;

		SpriteBox.x = DestFrameRec.x;
		SpriteBox.y = DestFrameRec.y;

		Hitbox = {DestFrameRec.x - HitboxOffset.x, DestFrameRec.y - Origin.y, float(Sprite.width)/Frames, float(Sprite.height)/2};

		SpawnLocation = {DestFrameRec.x, DestFrameRec.y};

		UpdateAnimation();
	}

	if(bIsDead)
	{
		Player->EnemiesKilled++;

		LinearBullet.bRelease = true;
		
		DeathExplosion[0].Explode({float(GetRandomValue(int(Location.x), int(Location.x) + Sprite.width/Frames)), float(GetRandomValue(int(Location.y), int(Location.y) + Sprite.height))}, Explosions);
	}

	UpdateBullet();

	CheckCollisionWithPlayerBullets();
	CheckHealth();
}

void Ship::Draw()
{
	if (bActive && !bIsDead)
		DrawTexturePro(Sprite, FrameRec, DestFrameRec, Origin, Rotation, WHITE);

	if (bDebug)
	{
		DrawCircle(SpawnLocation.x, SpawnLocation.y, 3.0f, BLUE); // Bullet spawn location

		DrawRectangleLines(DestFrameRec.x - Origin.x, DestFrameRec.y - Origin.y, Sprite.width/Frames, Sprite.height, RED); // Sprite box
		DrawRectangle(Hitbox.x, Hitbox.y, Hitbox.width, Hitbox.height, GRAY); // Hitbox
	
		DrawCircle(Destination.x, Destination.y, 5.0f, WHITE);
	}

	if(bIsDead)
		DeathExplosion[0].Draw();

	DrawBullet();
}

void Ship::Flock(std::vector<Enemy*>* Boids)
{
	ApplyBehaviours(Boids);
}

void Ship::ApplyBehaviours(std::vector<Enemy*>* Enemies)
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

void Ship::UpdateAnimation()
{
	SpriteFramesCounter++;

	if (SpriteFramesCounter >= (GetFPS()/FramesSpeed))
	{
		SpriteFramesCounter = 0;
		CurrentFrame++;
	
		if (CurrentFrame > 4)
			CurrentFrame = 0;
	
		FrameRec.x = float(CurrentFrame)*float(Sprite.width)/Frames;
	}
}

void Ship::UpdateBullet()
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

void Ship::DrawBullet()
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

