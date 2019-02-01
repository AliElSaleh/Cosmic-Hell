#include "Globals.h"
#include "Flock.h"
#include "ArchDemon.h"

#include <raymath.h>

template <class T>
Flock<T>::Flock()
{
	for (int i = 0; i < 10; i++)
		Boids.emplace_back(new T());
}

template <class T>
Flock<T>::Flock(const unsigned short AmountOfBoids)
{
	for (int i = 0; i < AmountOfBoids; i++)
		Boids.emplace_back(new T());
}

template <class T>
void Flock<T>::Init()
{
	for (unsigned short i = 0; i < Boids.size(); i++)
	{
		Boids[i]->Player = Player;
		Boids[i]->Init();
	}

	SetGoalLocation({float(GetRandomValue(150, GetScreenWidth()-PANEL_WIDTH - 150)), float(GetRandomValue(100, 200))});
}

template <class T>
void Flock<T>::Update()
{
	if (!Boids.empty())
		for (unsigned short i = 0; i < Boids.size(); i++)
		{
			Boids[i]->Flock(&Boids);
			Boids[i]->Update();
			IsAtLocation(TargetLocation);
		}
}

template <class T>
void Flock<T>::Draw()
{
	if (!Boids.empty())
		for (unsigned short i = 0; i < Boids.size(); i++)
			Boids[i]->Draw();
}

template <class T>
void Flock<T>::CheckBoidsWindowEdges()
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

template <class T>
void Flock<T>::IsAtLocation(const Vector2 Location)
{
	if (!Boids.empty())
		for (unsigned short i = 0; i < Boids.size(); i++)
		{
			const Vector2 DesiredVelocity = Vector2Subtract(Location, Boids[i]->Location);
			const float Distance = Vector2Length(DesiredVelocity);

			if (Distance < Boids[0]->TargetRadius)
			{
				SetGoalLocation({float(GetRandomValue(150, GetScreenWidth()-PANEL_WIDTH - 150)), float(GetRandomValue(100, 200))});
			}
		}
}

template <class T>
void Flock<T>::SetGoalLocation(const Vector2 GoalLocation)
{
	TargetLocation = GoalLocation;

	if (!Boids.empty())
		for (unsigned short i = 0; i < Boids.size(); i++)
			Boids[i]->Destination = GoalLocation;
}
