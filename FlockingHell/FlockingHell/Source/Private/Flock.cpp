#include "Flock.h"
#include <raymath.h>

Flock::Flock()
{
	Init();
}

void Flock::Init()
{
	if (!Boids.empty())
		for (unsigned short i = 0; i < Boids.size(); i++)
			Boids[i]->Init();

	if (!Ships.empty())
		for (unsigned short i = 0; i < Ships.size(); i++)
			Ships[i]->Init();
}

void Flock::Update()
{
	if (!Boids.empty())
		for (unsigned short i = 0; i < Boids.size(); i++)
		{
			Boids[i]->Flock(&Boids);
			Boids[i]->Update();
			Boids[i]->Acceleration = {0.0f, 0.0f};
			CheckBoidsWindowEdges();
		}

	if (!Ships.empty())
		for (unsigned short i = 0; i < Ships.size(); i++)
		{
			Ships[i]->Flock(&Ships);
			Ships[i]->Update();
			CheckBoidsWindowEdges();
		}
}

void Flock::Draw()
{
	if (!Boids.empty())
		for (unsigned short i = 0; i < Boids.size(); i++)
			Boids[i]->Draw();

	if (!Ships.empty())
		for (unsigned short i = 0; i < Ships.size(); i++)
			Ships[i]->Draw();
}

void Flock::CheckBoidsWindowEdges()
{
	if (!Boids.empty())
		for (unsigned short i = 0; i < Boids.size(); i++)
		{
			// X
			if (Boids[i]->Location.x + float(Boids[i]->Sprite.width)/5 < 0)
				Boids[i]->Location.x = GetScreenWidth() + float(Boids[i]->Sprite.width)/5;
			else if (Boids[i]->Location.x - float(Boids[i]->Sprite.width)/5 > GetScreenWidth())
				Boids[i]->Location.x = 0 - float(Boids[i]->Sprite.width)/5;

			// Y
			if (Boids[i]->Location.y + Boids[i]->Sprite.height < 0)
				Boids[i]->Location.y = GetScreenHeight() + Boids[i]->Sprite.height;
			else if (Boids[i]->Location.y - Boids[i]->Sprite.height > GetScreenHeight())
				Boids[i]->Location.y = 0 - Boids[i]->Sprite.height;
		}

	if (!Ships.empty())
		for (unsigned short i = 0; i < Ships.size(); i++)
		{
			// X
			if (Ships[i]->Location.x < 0)
				Ships[i]->Location.x = GetScreenWidth();
			else if (Ships[i]->Location.x > GetScreenWidth())
				Ships[i]->Location.x = 0;

			// Y
			if (Ships[i]->Location.y < 0)
				Ships[i]->Location.y = GetScreenHeight();
			else if (Ships[i]->Location.y > GetScreenHeight())
				Ships[i]->Location.y = 0;
		}
}

void Flock::SetNewDirection()
{
	for (unsigned short i = 0; i < Boids.size(); i++)
		Boids[i]->Direction = {float(GetRandomValue(0, GetScreenWidth())), float(GetRandomValue(0, GetScreenHeight()))};
}
