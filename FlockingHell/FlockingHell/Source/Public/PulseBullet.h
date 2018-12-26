#pragma once
#include "Bullet.h"

#define MAX_PULSE_BULLETS 100

struct PulseBullet
{
	enum TYPE
	{
		PULSE, // Repeatedly firing bullets
		ONELOOP, // Fire bullets once
		MULTILOOP // Loop one set of bullets depending on loop amount
	};

	void Init();
	void Update();
	void Draw() const;

	struct Bullet Bullet[MAX_PULSE_BULLETS];

	Vector2 SpawnLocation{}; // Center of circle
	Vector2 SpawnPoint[MAX_PULSE_BULLETS]{};
	Vector2 Direction[MAX_PULSE_BULLETS]{};

	TYPE BulletType{};

	void SetBulletType(TYPE BulletType);

	unsigned short Spacing{};
	unsigned short SpawnOffset{};
	unsigned short AmountToSpawn{};
	unsigned short LoopAmount{};

	float Speed{};
	float CircleRadius{};

	bool bRelease{};
	bool bDebug{};

	bool IsOutsideWindow() const;

private:
	void CheckBulletWindowCollision();
};

