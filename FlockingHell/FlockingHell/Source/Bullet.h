#pragma once
#include "raylib.h"

struct Bullet
{
	Vector2 Location;

	float Radius;
	float Speed;
	int Damage;
	int Spacing;

	bool bIsHit;
	bool bActive;
};