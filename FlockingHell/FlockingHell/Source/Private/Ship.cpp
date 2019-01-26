#include "Globals.h"
#include "Ship.h"
#include "Assets.h"

#include <raymath.h>

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

Ship::Ship()
{
	Ship::Init();
}

void Ship::Init()
{
	Sprite = GetAsset(Boid);

	Location = {float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH)), float(GetRandomValue(0, GetScreenHeight()))};
	Velocity = {float(GetRandomValue(-2, 2)), float(GetRandomValue(-2, 2))};
	MaxVelocity = 1.0f;
	MaxForce = 0.4f;
	Mass = 10.0f; // 10Kg
	TargetRadius = 20.0f;

	if (Sprite.width % 5 == 0)
		Frames = 5;
	else if (Sprite.width % 4 == 0)
		Frames = 4;

	Health = 50;
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
}

void Ship::Update()
{
	SpriteFramesCounter++;

	// Rotate towards the direction the ship is moving
	Rotation = atan2(Direction.y, Direction.x)*RAD2DEG;

	DestFrameRec.x = Location.x;
	DestFrameRec.y = Location.y;

	UpdateAnimation();
}

void Ship::Draw()
{
	if (bActive && !bIsDead)
		DrawTexturePro(Sprite, FrameRec, DestFrameRec, Origin, Rotation, WHITE);

	if (bDebug)
	{
		DrawCircle(int(Destination.x), int(Destination.y), 3.0f, WHITE); // Destination
		DrawFlockingProperties();
	}
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
	// Ship sprite animation
	if (SpriteFramesCounter >= (GetFPS()/FramesSpeed))
	{
		SpriteFramesCounter = 0;
		CurrentFrame++;
	
		if (CurrentFrame > 4)
			CurrentFrame = 0;
	
		FrameRec.x = float(CurrentFrame)*float(Sprite.width)/Frames;
	}
}

