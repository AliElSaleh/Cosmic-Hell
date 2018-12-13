#pragma once
#include "raylib.h"
#include "Bullet.h"

struct Player
{
	struct Bullet Bullet[100];

	Vector2 Location;
	Vector2 Rotation;

	float Size;
	signed short Health;

	bool bIsHit;
	bool bIsDead;
};

