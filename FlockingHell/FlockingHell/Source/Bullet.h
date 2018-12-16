#pragma once
#include "raylib.h"

#define MAX_SMALL_BULLETS 20

struct Bullet
{
	Bullet();

	void Init();
	void InitArray(int i);
	void Update();
	void Draw();

	Vector2 Location;

	struct Player* Player;

	float Radius;
	float Speed;

	int FramesCounter;
	int Damage;
	int Spacing;

	bool bIsHit;
	bool bActive;
};