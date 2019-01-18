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

	Location = {float(GetRandomValue(0, GetScreenWidth())), float(GetRandomValue(0, GetScreenHeight()))};
	Velocity = {float(GetRandomValue(-3, 3)), float(GetRandomValue(-3, 3))};
	MaxVelocity = 2.0f;
	MaxForce = 0.2f;
	Mass = 10.0f; // 10Kg
	TargetRadius = 10.0f;

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

	BoidDestFrameRec = {Location.x, Location.y, (float(Sprite.width)/5), float(Sprite.height)};
}

void Ship::Update()
{
	BoidSpriteFramesCounter++;
	
	Destination = GetMousePosition();

	const auto Lerp = [&](const float From, const float To, const float Time)
	{
		return From + Time * (To - From);
	};

	// Rotate towards the direction the ship is moving
	Rotation = Lerp(CurrentRotation, atan2(Direction.y, Direction.x)*RAD2DEG, GetFrameTime()/0.1f);
	CurrentRotation = Rotation;

	BoidDestFrameRec.x = Location.x;
	BoidDestFrameRec.y = Location.y;

	UpdateShipAnimation();

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

void Ship::Draw()
{
	DrawCircle(GetMousePosition().x, GetMousePosition().y, 3.0f, WHITE); // Destination

	DrawTexturePro(Sprite, BoidFrameRec, BoidDestFrameRec, Origin, Rotation + 180.0f, WHITE);
	//DrawCircle(Location.x, Location.y, 5.0f, RED); // Ships
}

void Ship::Flock(std::vector<Ship*>* Boids)
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
		SetDestLocation({float(GetRandomValue(0 + Sprite.width/5, GetScreenWidth() - Sprite.width/5)), float(GetRandomValue(0 + Sprite.height, GetScreenHeight()))});
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

void Ship::Flee(const Vector2& DestLocation)
{
	
}

Vector2 Ship::Align(std::vector<Ship*>* Boids)
{
	Vector2 SumOfVectors = {0.0f, 0.0f};
	const float PerceptionRadius = 40.0f;
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

Vector2 Ship::Cohere(std::vector<Ship*>* Boids)
{
	Vector2 SumOfVectors = {0.0f, 0.0f};
	const float PerceptionRadius = 50.0f;
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

Vector2 Ship::Separate(std::vector<Ship*>* Boids)
{
	Vector2 SumOfVectors = {0.0f, 0.0f};
	const float DesiredSeparation = float(Sprite.width)/5;
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

Vector2 Ship::Limit(Vector2 V, float Amount) const
{
	if (V.x > Speed)
		V.x = Speed;

	if (V.y > Speed)
		V.y = Speed;

	return V;
}

void Ship::ApplyForce(const Vector2 Force)
{
	Velocity = Limit(Vector2Add(Velocity, Force), MaxSpeed);
	Location = Vector2Add(Location, Velocity);
}

void Ship::ApplyBehaviours(std::vector<Ship*>* Boids)
{
	Vector2 Alignment = Align(Boids);
	Vector2 Cohesion = Cohere(Boids);
	Vector2 Separation = Separate(Boids);
	//Vector2 GoalSeek = Seek(Destination);

	Alignment = Vector2Scale(Alignment, AlignmentForce);
	Cohesion = Vector2Scale(Cohesion, CohesionForce);
	Separation = Vector2Scale(Separation, SeparationForce);		// Arbitrary weight values
	//GoalSeek = Vector2Scale(GoalSeek, GoalSeekForce);

	ApplyForce(Alignment);
	ApplyForce(Cohesion);
	ApplyForce(Separation);
	//ApplyForce(GoalSeek);
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

