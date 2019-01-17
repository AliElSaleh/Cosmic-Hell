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
	MaxVelocity = 2.0f;
	MaxForce = 0.2f;
	Mass = 10.0f; // 10Kg
	TargetRadius = 10.0f;

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

	SetDestLocation({float(GetRandomValue(0, GetScreenWidth())), float(GetRandomValue(0, GetScreenHeight()))});
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

		if (bFlock)
		{
			Velocity = Vector2Add(Velocity, Acceleration);
			Velocity = Limit(Velocity, MaxSpeed);
			Location = Vector2Add(Location, Velocity);
		}
		else
		{
			Seek(Destination);

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				SetDestLocation(GetMousePosition());
				bLeftMousePressed = true;
				bRightMousePressed = false;
			}

			if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
			{
				SetDestLocation(GetMousePosition());
				bRightMousePressed = true;
				bLeftMousePressed = false;
			}

			if (bLeftMousePressed)
				Seek(Destination);

			if (bRightMousePressed)
				Flee(Destination);

			CheckWindowCollision();
		}

		const auto Lerp = [&](const float From, const float To, const float Time)
		{
			return From + Time * (To - From);
		};

		// Rotate towards the target
		Rotation = atan2(Direction.y, Direction.x)*RAD2DEG;// Lerp(CurrentRotation, atan2(Direction.y, Direction.x)*RAD2DEG, GetFrameTime()/0.1f);
		CurrentRotation = Rotation;
		
		BoidDestFrameRec.x = Location.x;
		BoidDestFrameRec.y = Location.y;

		UpdateBoidAnimation();
	}
}

void Boid::Draw()
{
	// Draw the demon sprite
	if (bActive && !bIsDead)
		DrawTexturePro(Sprite, BoidFrameRec, BoidDestFrameRec, Origin, Rotation + 180.0f, WHITE);  // Draw part of the demon texture

	DrawCircle(Destination.x, Destination.y, 3.0f, WHITE);

	if (!bFlock)
		DrawText(FormatText("Rotation: %f", Rotation), 10, 70, 18, WHITE);
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

void Boid::Seek(const Vector2 &DestLocation)
{
	const auto Limit = [](Vector2 V, const float Speed)
	{
		if (V.x > Speed)
			V.x = Speed;

		if (V.y > Speed)
			V.y = Speed;

		return V;
	};

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

	Velocity = Limit(Vector2Add(Velocity, Steering), MaxSpeed);

	Location = Vector2Add(Location, Velocity);
}

void Boid::Flee(const Vector2 & DestLocation)
{
	const auto Limit = [](Vector2 V, const float Speed)
	{
		if (V.x > Speed)
			V.x = Speed;

		if (V.y > Speed)
			V.y = Speed;

		return V;
	};

	// Arrival logic
	DesiredVelocity = Vector2Subtract(DestLocation, Location);
	const float Distance = Vector2Length(DesiredVelocity);

	if (Distance > TargetRadius)
	{
		// Outside the slowing area
		DesiredVelocity = Vector2Scale(Vector2Normalize(Vector2Subtract(Location, DestLocation)), MaxVelocity);
	}
	else
	{
		// Inside the slowing area
		DesiredVelocity = Vector2Scale(Vector2Normalize(Vector2Subtract(Location, DestLocation)), MaxVelocity * (Distance / TargetRadius));
	}

	Steering = Vector2Subtract(DesiredVelocity, Velocity);

	Direction = Velocity;

	Steering = Limit(Steering, MaxForce);
	Steering = Vector2Divide(Steering, Mass);

	Velocity = Limit(Vector2Add(Velocity, Steering), MaxSpeed);

	Location = Vector2Add(Location, Velocity);
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

void Boid::CheckWindowCollision()
{
	// X
	if (Location.x + float(Sprite.width)/5 < 0)
		Location.x = GetScreenWidth() + float(Sprite.width)/5;
	else if (Location.x - float(Sprite.width)/5 > GetScreenWidth())
		Location.x = 0 - float(Sprite.width)/5;

	// Y
	if (Location.y + Sprite.height < 0)
		Location.y = GetScreenHeight() + Sprite.height;
	else if (Location.y - Sprite.height > GetScreenHeight())
		Location.y = 0 - Sprite.height;
}

Vector2 Boid::Align(std::vector<Boid*> *Boids)
{
	Vector2 Steering{0.0f, 0.0f};

	const float PerceptionRadius = 50.0f;
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
			Steering = Vector2Add(Steering, Other->Velocity);

			// Set this boid's direction to the steering direction of the flock
			Direction = Vector2Add(Direction, Steering);
			Direction = Vector2Normalize(Direction);

			Total++;
		}
	}

	if (Total > 0)
	{
		Steering = Vector2Divide(Steering, Total);
		Steering = Vector2Normalize(Steering);
		Steering = Vector2Subtract(Steering, Velocity);
		Steering = Limit(Steering, AlignMaxForce);
	}
	else
	{
		Direction = Vector2Add(Direction, Velocity);
		Direction = Vector2Normalize(Direction);
	}


	return Steering;
}

Vector2 Boid::Cohere(std::vector<Boid*>* Boids)
{
	// Seek function maybe??

	Vector2 Steering{0.0f, 0.0f};

	const float PerceptionRadius = 30.0f;
	unsigned short Total = 0;

	const auto SetMag = [](Vector2 V, const float Speed)
	{
		V.x = Speed;
		V.y = Speed;

		const float Magnitude = sqrtf((V.x*V.x) + (V.y*V.y));

		V.x = Magnitude;
		V.y = Magnitude;

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
			Direction = Vector2Add(Direction, Steering);
			Direction = Vector2Normalize(Direction);
			
			Total++;
		}
	}

	if (Total > 0)
	{
		Steering = Vector2Divide(Steering, Total);
		Steering = Vector2Normalize(Steering);
		Steering = Vector2Subtract(Steering, Location);
		Steering = SetMag(Steering, MaxSpeed);
		Steering = Limit(Steering, CohereMaxForce);
	}
	else
	{
		Direction = Vector2Add(Direction, Velocity);
		Direction = Vector2Normalize(Direction);
	}

	return Steering;
}

Vector2 Boid::Separate(std::vector<Boid*>* Boids)
{
	// Flee function maybe??

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
			const Vector2 Difference = Vector2Subtract(Other->Location, this->Location);

			Steering = Vector2Add(Steering, Difference);	
			Steering = Vector2Negate(Steering);

			Total++;
		}
	}

	if (Total > 0)
	{
		Steering = Vector2Divide(Steering, Total);
		Steering = Vector2Normalize(Steering);
		Steering = Vector2Subtract(Steering, Velocity);
		Steering = Limit(Steering, SeparateMaxForce);
	}
	
	return Steering;
}
