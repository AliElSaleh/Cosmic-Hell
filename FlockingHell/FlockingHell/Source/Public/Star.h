#pragma once
#include <raylib.h>

#define MAX_SMALL_STARS 30
#define MAX_FAST_STARS 70
#define MAX_CLOSE_STARS 60
#define MAX_FAR_STARS 40

struct Star
{
	void Init();
	void Update();
	void Draw() const;

	Vector2 Location;

	float Speed;
	float Radius;
};