#pragma once
#include "raylib.h"

struct Star
{
	Star();

	void Init();
	void Update();
	void Draw();

	Vector2 Location;

	float Speed;
	float Radius;
};