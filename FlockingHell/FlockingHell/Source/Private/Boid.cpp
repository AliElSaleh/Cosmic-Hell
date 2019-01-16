#include "Boid.h"
#include "Assets.h"
#include <raymath.h>

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

Boid::Boid()
{
	Boid::Init();
}

void Boid::Init()
{
	Sprite = GetAsset(Boid);

	Location = {0.0f, 0.0f};
	SpriteBox = {Location.x, Location.y, float(Sprite.width)/5, float(Sprite.height)};
	Health = 100;
	Speed = 250.0f;
	Damage = GetRandomValue(1, 3);
	ShootRate = 5;
	bActive = true;
	bIsDead = false;
	bDebug = false;

	Origin.x = float(Sprite.width)/5 /2;
	Origin.y = float(Sprite.height) / 2;

	BoidFrameRec.x = 0.0f;
	BoidFrameRec.y = 0.0f;
	BoidFrameRec.width = float(Sprite.width)/5;
	BoidFrameRec.height = Sprite.height;

	BoidDestFrameRec = {float(GetScreenWidth())/2, float(GetScreenHeight())/2, (float(Sprite.width)/5), float(Sprite.height)};

	SetDestLocation({float(GetRandomValue(0 + Sprite.width/5, GetScreenWidth() - Sprite.width/5)), float(GetRandomValue(0 + Sprite.height, GetScreenHeight()))});
}

void Boid::Update()
{
	if (bActive && !bIsDead)
	{
		BoidSpriteFramesCounter++;

		BoidDestFrameRec.x = Location.x;
		BoidDestFrameRec.y = Location.y;

		MoveToLocation(Destination);

		const auto Lerp = [&](const float From, const float To, const float Time)
		{
			return From + Time * (To - From);
		};

		Rotation = Lerp(CurrentRotation, atan2(Direction.y, Direction.x)*RAD2DEG, GetFrameTime()/0.1f);
		CurrentRotation = Rotation;
		

		UpdateBoidAnimation();
	}

	IsAtLocation(Destination);
}

void Boid::Draw()
{
	// Draw the demon sprite
	if (bActive && !bIsDead)
		DrawTexturePro(Sprite, BoidFrameRec, BoidDestFrameRec, Origin, Rotation + 180.0f, WHITE);  // Draw part of the demon texture


	DrawCircle(Destination.x, Destination.y, 5.0f, WHITE);
}

float Boid::Round(const float Number)
{
	float Value;
	if (Number < 0)
		Value = (int)(Number - 0.5);
	else
		Value = (int)(Number + 0.5);

    return Value; 
}

bool Boid::IsAtLocation(const Vector2 & GoalLocation)
{
	const auto Compare = [&](const float A, const float B, const float Epsilon = 1.5f)
	{
	    return (fabs(A - B) < Epsilon);
	};

	if (Compare(Location.x, GoalLocation.x) && Compare(Location.y, GoalLocation.y)) // Is at the goal location?
	{
		bIsAtLocation = true;
		SetDestLocation({float(GetRandomValue(0 + Sprite.width/5, GetScreenWidth() - Sprite.width/5)), float(GetRandomValue(0 + Sprite.height, GetScreenHeight()))});
	}
	else
		bIsAtLocation = false;

	return bIsAtLocation;
}

void Boid::UpdateBoidAnimation()
{
	// Demon sprite animation
	if (BoidSpriteFramesCounter >= (GetFPS()/FramesSpeed))
	{
		BoidSpriteFramesCounter = 0;
		BoidCurrentFrame++;
	
		if (BoidCurrentFrame > 4)
			BoidCurrentFrame = 0;
	
		BoidFrameRec.x = float(BoidCurrentFrame)*float(Sprite.width)/5; // 5 Frames
	}
}
