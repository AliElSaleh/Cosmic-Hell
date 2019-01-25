#include "Globals.h"
#include "ArchDemon.h"
#include "Assets.h"

#include <raymath.h>

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

ArchDemon::ArchDemon()
{
	ArchDemon::Init();
}

void ArchDemon::Init()
{
	Sprite = GetAsset(ArchDemon);

	Location = {float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH)), float(GetRandomValue(0, GetScreenHeight()))};
	Velocity = {float(GetRandomValue(-2, 2)), float(GetRandomValue(-2, 2))};
	MaxVelocity = 1.0f;
	MaxForce = 0.4f;
	Mass = 5.0f; // 5Kg
	TargetRadius = 20.0f;
	Frames = 10;

	Health = 100;
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
}

void ArchDemon::Update()
{
	UpdateAnimation();
}

void ArchDemon::Draw()
{
	if (bActive && !bIsDead)
		DrawTextureRec(Sprite, FrameRec, Location, WHITE);

	if (bDebug)
		DrawCircle(int(Destination.x), int(Destination.y), 3.0f, WHITE);
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
		
			FrameRec.x = float(CurrentFrame)*float(Sprite.width)/10;
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
		
			FrameRec.x = float(CurrentFrame)*float(Sprite.width)/10;
		}
	}
}
