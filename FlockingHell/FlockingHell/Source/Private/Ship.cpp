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

	Health = 50;
	Speed = 100.0f;
	Damage = GetRandomValue(1, 3);
	ShootRate = 5;
	bActive = true;
	bIsDead = false;
	bDebug = false;
	
	Origin.x = float(Sprite.width)/5 / 2;
	Origin.y = float(Sprite.height) / 2;

	BoidFrameRec.x = 0.0f;
	BoidFrameRec.y = 0.0f;
	BoidFrameRec.width = float(Sprite.width)/5;
	BoidFrameRec.height = Sprite.height;

	BoidDestFrameRec = {Location.x, Location.y, (float(Sprite.width)/5), float(Sprite.height)};
}

void Ship::Update()
{
	BoidSpriteFramesCounter++;

	// Rotate towards the direction the ship is moving
	Rotation = atan2(Direction.y, Direction.x)*RAD2DEG;
	CurrentRotation = Rotation;

	BoidDestFrameRec.x = Location.x;
	BoidDestFrameRec.y = Location.y;

	UpdateShipAnimation();

	if (bDebug)
	{
		if (IsMouseButtonPressed(0))
			AlignmentForce += 0.1f;
		else if(IsMouseButtonPressed(1))
			AlignmentForce -= 0.1f;

		if (IsKeyPressed(KEY_LEFT))
			CohesionForce += 0.1f;
		else if (IsKeyPressed(KEY_RIGHT))
			CohesionForce -= 0.1f;

		if (IsKeyPressed(KEY_UP))
			SeparationForce += 0.1f;
		else if (IsKeyPressed(KEY_DOWN))
			SeparationForce -= 0.1f;
	}
}

void Ship::Draw()
{
	DrawCircle(Destination.x, Destination.y, 3.0f, WHITE); // Destination

	DrawTexturePro(Sprite, BoidFrameRec, BoidDestFrameRec, Origin, Rotation + 180.0f, WHITE);
	//DrawCircle(Location.x, Location.y, 5.0f, RED); // Ships

	if (bDebug)
	{
		DrawText(FormatText("Alignment Force: %f", AlignmentForce), 10, 70, 18, WHITE);
		DrawText(FormatText("Cohesion Force: %f", CohesionForce), 10, 90, 18, WHITE);
		DrawText(FormatText("Separation Force: %f", SeparationForce), 10, 110, 18, WHITE);
		DrawText(FormatText("Goal seeking Force: %f", GoalSeekForce), 10, 130, 18, WHITE);
	}
}

void Ship::Flock(std::vector<Enemy*>* Boids)
{
	ApplyBehaviours(Boids);
}

Vector2 Ship::Seek(const Vector2& DestLocation)
{
	// Arrival logic
	DesiredVelocity = Vector2Subtract(DestLocation, Location);
	const float Distance = Vector2Length(DesiredVelocity);

	if (Distance < TargetRadius)
	{
		// Inside the slowing area
		DesiredVelocity = Vector2Scale(Vector2Normalize(Vector2Subtract(DestLocation, Location)), MaxVelocity * (Distance / TargetRadius));
	}
	else
	{
		// Outside the slowing area
		DesiredVelocity = Vector2Scale(Vector2Normalize(Vector2Subtract(DestLocation, Location)), MaxVelocity);
	}

	Steering = Vector2Subtract(DesiredVelocity, Velocity);

	Direction = Velocity;

	// Movement of boid
	Steering = Limit(Steering, MaxForce);
	Steering = Vector2Divide(Steering, Mass);

	return Steering;
}

Vector2 Ship::Align(std::vector<Enemy*>* Boids) const
{
	Vector2 SumOfVectors = {0.0f, 0.0f};
	const float PerceptionRadius = 50.0f;
	unsigned short Neighbors = 0;

	for (auto Other : *Boids)
	{
		const float Distance = Vector2Distance(Location, Other->Location);

		if (Distance > 0 && Distance < PerceptionRadius)
		{
			SumOfVectors = Vector2Add(SumOfVectors, Other->Velocity);
			Neighbors++;
		}
	}

	if (Neighbors > 0)
	{
		SumOfVectors = Vector2Divide(SumOfVectors, Neighbors); // Get the average velocity vectors

		Vector2 Steer = Vector2Subtract(SumOfVectors, Velocity); // Reynold's steering formula
		Steer = Limit(Steer, MaxForce);

		return Steer;
	}

	return Vector2Zero();
}

Vector2 Ship::Cohere(std::vector<Enemy*>* Boids)
{
	Vector2 SumOfVectors = {0.0f, 0.0f};
	const float PerceptionRadius = 60.0f;
	unsigned short Neighbors = 0;

	for (auto Other : *Boids)
	{
		const float Distance = Vector2Distance(Location, Other->Location);

		if (Distance > 0 && Distance < PerceptionRadius)
		{
			SumOfVectors = Vector2Add(SumOfVectors, Other->Location);
			Neighbors++;
		}
	}

	if (Neighbors > 0)
	{
		SumOfVectors = Vector2Divide(SumOfVectors, Neighbors); // Get the average velocity vectors

		return Seek(SumOfVectors);
	}

	return Vector2Zero();
}

Vector2 Ship::Separate(std::vector<Enemy*>* Boids) const
{
	Vector2 SumOfVectors = {0.0f, 0.0f};
	const float DesiredSeparation = float(Sprite.width)/5 + 10.0f; // 10 is an Arbitrary value
	unsigned short Neighbors = 0;

	for (auto Other : *Boids)
	{
		const float Distance = Vector2Distance(Location, Other->Location);

		if (Distance > 0 && Distance < DesiredSeparation)
		{
			Vector2 Difference = Vector2Subtract(Location, Other->Location);
			Difference = Vector2Normalize(Difference);

			SumOfVectors = Vector2Add(SumOfVectors, Difference);
			Neighbors++;
		}
	}

	if (Neighbors > 0)
	{
		SumOfVectors = Vector2Divide(SumOfVectors, Neighbors);

		Vector2 Steer = Vector2Subtract(SumOfVectors, Velocity);
		Steer = Limit(Steer, MaxForce);
		
		return Steer;
	}

	return Vector2Zero();
}

Vector2 Ship::Limit(Vector2 V, const float Amount) const
{
	if (V.x > Amount)
		V.x = Amount;

	if (V.y > Amount)
		V.y = Amount;

	return V;
}

void Ship::ApplyForce(const Vector2 Force)
{
	Velocity = Vector2Add(Velocity, Force);
	Location = Vector2Add(Location, Vector2Scale(Velocity, Speed * GetFrameTime()));
}

void Ship::ApplyBehaviours(std::vector<Enemy*>* Boids)
{
	Vector2 Alignment = Align(Boids);
	Vector2 Cohesion = Cohere(Boids);
	Vector2 Separation = Separate(Boids);
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

void Ship::UpdateShipAnimation()
{
	// Ship sprite animation
	if (BoidSpriteFramesCounter >= (GetFPS()/FramesSpeed))
	{
		BoidSpriteFramesCounter = 0;
		BoidCurrentFrame++;
	
		if (BoidCurrentFrame > 4)
			BoidCurrentFrame = 0;
	
		BoidFrameRec.x = float(BoidCurrentFrame)*float(Sprite.width)/5; // 5 Frames
	}
}

