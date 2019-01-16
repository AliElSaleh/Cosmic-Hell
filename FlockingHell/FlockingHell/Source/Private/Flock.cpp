#include "Flock.h"
#include <raymath.h>

Flock::Flock()
{
	Init();
}

void Flock::Init()
{
	for (unsigned short i = 0; i < Boids.size(); i++)
		Boids[i]->Init();
}

void Flock::Update()
{
	for (unsigned short i = 0; i < Boids.size(); i++)
	{
		Boids[i]->Flock(&Boids);
		Boids[i]->Update();
		Boids[i]->Acceleration = {0.0f, 0.0f};
		CheckBoidsWindowEdges();
	}
}

void Flock::Draw()
{
	for (unsigned short i = 0; i < Boids.size(); i++)
		Boids[i]->Draw();
}

void Flock::CheckBoidsWindowEdges()
{
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
}

void Flock::SetNewDirection()
{
	for (unsigned short i = 0; i < Boids.size(); i++)
		Boids[i]->Direction = {float(GetRandomValue(0, GetScreenWidth())), float(GetRandomValue(0, GetScreenHeight()))};
}
