#pragma once
#include "raylib.h"

#define MAX_SMALL_STARS 30

struct Star
{
	Star();

	void Init();
	void Update();
	void Draw() const;

	Vector2 Location;

	float Speed;
	float Radius;
};