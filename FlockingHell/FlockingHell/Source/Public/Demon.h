#pragma once

#include "Enemy.h"
#include "PulseBullet.h"

#define MAX_DEMON_BULLETS 10
#define MAX_DEMON_RAGE_BULLETS 5

struct Demon : Enemy
{
	enum WAVE
	{
		FIRST,
		SECOND,
		THIRD,
		RAGE
	};

	void Init() override;
	void Update() override;
	void Draw() override;

	struct PulseBullet PulseBullet;
	struct PulseBullet PulseBullet2ndWave;
	struct PulseBullet PulseBullet3rdWave;
	struct PulseBullet BulletRage[MAX_DEMON_RAGE_BULLETS];

	Vector2 SpawnLocation{};
	Texture2D BulletSprite;

	WAVE BulletWave;

	struct Player* Player = nullptr;

protected:
	void UpdateBullet();
	void DrawBullet() const;

	void CheckCollisionWithPlayer() override;
	void CheckCollisionWithPlayerBullets() override;
	void CheckHealth() override;

	bool IsLowHealth() const override;
};

