#pragma once
#include "raylib.h"

#define WAVE_ONE_BULLETS 20
#define WAVE_TWO_BULLETS 30

struct Bullet
{
	Bullet();
	Bullet(Vector2 Location, struct Player* Player, float Radius, float Speed, int FramesCounter, int Damage, int Spacing, bool bIsHit, bool bActive);

	void Init();
	void InitArray(int i);
	void InitWave(int Wave);
	void Update();
	void Draw() const;

	bool IsOutsideWindow() const;
	bool IsLocationYGreaterThan(float Y); // Is this bullet greater than (Y) on the Y axis

	Vector2 Location;

	struct Player* Player;

	float Radius;
	float Speed;

	int FramesCounter;
	int Damage;
	int Spacing;

	bool bIsHit;
	bool bActive;

private:
	void CheckCollisionWithPlayerBullets();
	void CheckCollisionWithPlayer();
};