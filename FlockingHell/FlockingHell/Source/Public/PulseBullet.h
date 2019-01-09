#pragma once
#include "Bullet.h"

#define MAX_PULSE_BULLETS 100

struct PulseBullet
{
	enum TYPE
	{
		PULSE, // Repeatedly firing bullets
		ONELOOP, // Fire bullets once
		MULTILOOP, // Loop one set of bullets depending on loop amount
		RAGE // When low health, enemy will burst out bullets in a fit of RAGE!!!
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

	float Spacing{};
	float SpawnOffset{};

	unsigned short AmountToSpawn{};
	unsigned short LoopAmount{};
	unsigned short ReleaseAmount{};

	float Speed{};
	float CircleRadius{};

	bool bRelease{};
	bool bDebug{};

	bool IsOutsideWindow() const;

	void CheckBulletWindowCollision();
};

