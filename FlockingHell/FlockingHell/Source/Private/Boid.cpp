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

	Location = {float(GetRandomValue(0, GetScreenWidth())), float(GetRandomValue(0, GetScreenHeight()))};
	Velocity = {float(GetRandomValue(-3, 3)), float(GetRandomValue(-3, 3))};

	SpriteBox = {Location.x, Location.y, float(Sprite.width)/5, float(Sprite.height)};
	Health = 100;
	Speed = 250.0f;
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

	BoidDestFrameRec = {float(GetScreenWidth())/2, float(GetScreenHeight())/2, (float(Sprite.width)/5), float(Sprite.height)};

	SetDestLocation({float(GetRandomValue(0 + Sprite.width/5, GetScreenWidth() - Sprite.width/5)), float(GetRandomValue(0 + Sprite.height, GetScreenHeight()))});
}

void Boid::Update()
{
	if (bActive && !bIsDead)
	{
		BoidSpriteFramesCounter++;

		const auto Limit = [](Vector2 V, const float Speed)
		{
			if (V.x > Speed)
				V.x = Speed;

			if (V.y > Speed)
				V.y = Speed;

			return V;
		};

		// Move boids
		Velocity = Vector2Add(Velocity, Acceleration);
		Velocity = Limit(Velocity, MaxSpeed);
		Location = Vector2Add(Location, Velocity);

		const auto Lerp = [&](const float From, const float To, const float Time)
		{
			return From + Time * (To - From);
		};

		// Rotate towards the target
		Rotation = Lerp(CurrentRotation, atan2(Direction.y, Direction.x)*RAD2DEG, GetFrameTime()/0.1f);
		CurrentRotation = Rotation;
		
		BoidDestFrameRec.x = Location.x;
		BoidDestFrameRec.y = Location.y;

		UpdateBoidAnimation();
	}

	//IsAtLocation(Destination);
}

void Boid::Draw()
{
	// Draw the demon sprite
	if (bActive && !bIsDead)
		DrawTexturePro(Sprite, BoidFrameRec, BoidDestFrameRec, Origin, Rotation + 180.0f, WHITE);  // Draw part of the demon texture

	DrawCircle(Destination.x, Destination.y, 5.0f, WHITE);
}

void Boid::Flock(std::vector<Boid*> *Boids)
{
	const auto Alignment = Align(Boids);
	Acceleration = Vector2Add(Acceleration, Alignment);
	
	const auto Cohesion = Cohere(Boids);
	Acceleration = Vector2Add(Acceleration, Cohesion);
	
	const auto Separation = Separate(Boids);
	Acceleration = Vector2Add(Acceleration, Separation);
}

bool Boid::IsAtLocation(const Vector2 &GoalLocation)
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

Vector2 Boid::Align(std::vector<Boid*> *Boids)
{
	Vector2 Steering{0.0f, 0.0f};

	const float PerceptionRadius = 40.0f;
	unsigned short Total = 0;

	const auto SetMag = [](Vector2 V, const float Speed)
	{
		V.x = Speed;
		V.y = Speed;
	
		return V;
	};

	const auto Limit = [](Vector2 V, const float Force)
	{
		if (V.x > Force)
			V.x = Force;

		if (V.y > Force)
			V.y = Force;

		return V;
	};

	for (auto Other : *Boids)
	{
		const float Distance = Vector2Distance(this->Location, Other->Location);

		if (Other != this && Distance < PerceptionRadius)
		{
			Steering = Vector2Add(Steering, Other->Velocity);

			// Set this boid's direction to the steering direction of the flock
			Direction = Vector2Subtract(Steering, {0,0}); // I don't know why this works
			Direction = Vector2Normalize(Direction);

			Total++;
		}
	}

	if (Total > 0)
	{
		Steering = Vector2Divide(Steering, Total);
		Steering = SetMag(Steering, MaxSpeed);
		Steering = Vector2Subtract(Steering, Velocity);
		Steering = Limit(Steering, AlignMaxForce);
	}

	return Steering;
}

Vector2 Boid::Cohere(std::vector<Boid*>* Boids)
{
	Vector2 Steering{0.0f, 0.0f};

	const float PerceptionRadius = 20.0f;
	unsigned short Total = 0;

	const auto SetMag = [](Vector2 V, const float Speed)
	{
		V.x = Speed;
		V.y = Speed;
	
		return V;
	};

	const auto Limit = [](Vector2 V, const float Force)
	{
		if (V.x > Force)
			V.x = Force;

		if (V.y > Force)
			V.y = Force;

		return V;
	};

	for (auto Other : *Boids)
	{
		const float Distance = Vector2Distance(this->Location, Other->Location);

		if (Other != this && Distance < PerceptionRadius)
		{
			Steering = Vector2Add(Steering, Other->Location);

			// Set this boid's direction to cohere to nearby flockmates
			Direction = Vector2Subtract(Steering, Other->Location);
			Direction = Vector2Normalize(Direction);
			
			Total++;
		}
	}

	if (Total > 0)
	{
		Steering = Vector2Divide(Steering, Total);
		Steering = Vector2Subtract(Steering, this->Location);
		Steering = SetMag(Steering, MaxSpeed);
		Steering = Vector2Subtract(Steering, this->Velocity);
		Steering = Limit(Steering, CohereMaxForce);
	}

	return Steering;
}

Vector2 Boid::Separate(std::vector<Boid*>* Boids)
{
	Vector2 Steering{0.0f, 0.0f};

	const float PerceptionRadius = 30.0f;
	unsigned short Total = 0;

	const auto Limit = [](Vector2 V, const float Force)
	{
		if (V.x > Force)
			V.x = Force;

		if (V.y > Force)
			V.y = Force;

		return V;
	};

	for (auto Other : *Boids)
	{
		const float Distance = Vector2Distance(this->Location, Other->Location);

		if (Other != this && Distance < PerceptionRadius)
		{
			Vector2 Difference = Vector2Subtract(this->Location, Other->Location);
			Difference = Vector2Divide(Difference, Distance);

			Steering = Vector2Add(Steering, Difference);

			// Set this boid's direction to away from nearby flockmates
			Direction = Vector2Subtract(Steering, Other->Velocity);
			Direction = Vector2Normalize(Direction);

			Total++;
		}
	}

	if (Total > 0)
	{
		Steering = Vector2Divide(Steering, Total);
		Steering = Vector2Subtract(Steering, this->Velocity);
		Steering = Limit(Steering, SeparateMaxForce);
	}

	
	return Steering;
}
