#include "Globals.h"
#include "Flock.h"
#include "ArchDemon.h"

#include <raymath.h>

Flock::Flock()
{
	Init();
}

void Flock::Init()
{
	for (int i = 0; i < 20; i++)
		Boids.emplace_back(new Ship());

	for (unsigned short i = 0; i < Boids.size(); i++)
		Boids[i]->Init();

	SetGoalLocation({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH)), float(GetRandomValue(0, GetScreenHeight()))});
}

void Flock::Update()
{
	if (!Boids.empty())
		for (unsigned short i = 0; i < Boids.size(); i++)
		{
			Boids[i]->Flock(&Boids);
			Boids[i]->Update();
			CheckBoidsWindowEdges();
			IsAtLocation(TargetLocation);
		}
}

void Flock::Draw()
{
	if (!Boids.empty())
		for (unsigned short i = 0; i < Boids.size(); i++)
			Boids[i]->Draw();
}

void Flock::CheckBoidsWindowEdges()
{
	if (!Boids.empty())
		for (unsigned short i = 0; i < Boids.size(); i++)
		{
			// X
			if (Boids[i]->Location.x + Boids[i]->Sprite.width/float(Boids[i]->Frames) < 0)
				Boids[i]->Location.x = float(GetScreenWidth()-PANEL_WIDTH);
			else if (Boids[i]->Location.x > GetScreenWidth()-PANEL_WIDTH)
				Boids[i]->Location.x = 0 - Boids[i]->Sprite.width/float(Boids[i]->Frames);

			// Y
			if (Boids[i]->Location.y + Boids[i]->Sprite.height < 0)
				Boids[i]->Location.y = float(GetScreenHeight());
			else if (Boids[i]->Location.y > GetScreenHeight())
				Boids[i]->Location.y = 0 - float(Boids[i]->Sprite.height);
		}
}

void Flock::IsAtLocation(const Vector2 Location)
{
	if (!Boids.empty())
		for (unsigned short i = 0; i < Boids.size(); i++)
		{
			const Vector2 DesiredVelocity = Vector2Subtract(Location, Boids[i]->Location);
			const float Distance = Vector2Length(DesiredVelocity);

			if (Distance < Boids[0]->TargetRadius)
			{
				SetGoalLocation({float(GetRandomValue(0, GetScreenWidth()-PANEL_WIDTH)), float(GetRandomValue(0, GetScreenHeight()))});
			}
		}
}

void Flock::SetGoalLocation(const Vector2 GoalLocation)
{
	TargetLocation = GoalLocation;

	if (!Boids.empty())
		for (unsigned short i = 0; i < Boids.size(); i++)
			Boids[i]->Destination = GoalLocation;
}
