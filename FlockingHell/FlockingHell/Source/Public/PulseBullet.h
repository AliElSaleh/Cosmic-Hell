#pragma once
#include "Bullet.h"

#define MAX_PULSE_BULLETS 100

struct PulseBullet
{
	void Init();
	void Update();
	void Draw() const;

	struct Bullet Bullet[MAX_PULSE_BULLETS];

	Vector2 SpawnLocation{}; // Center of circle
	Vector2 SpawnPoint[MAX_PULSE_BULLETS]{};
	Vector2 Direction[MAX_PULSE_BULLETS]{};

	unsigned short Spacing{};
	unsigned short SpawnOffset{};
	unsigned short AmountToSpawn{};
	unsigned short LoopAmount{};

	float Speed{};
	float CircleRadius{};

	bool bKeyPressed{};
	bool bDebug{};

private:
	void CheckBulletsOutsideWindow();
};

