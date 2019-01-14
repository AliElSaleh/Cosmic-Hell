#pragma once

#include "Enemy.h"
#include "PulseBullet.h"
#include "LinearPattern.h"
#include "LinearMultiPattern.h"
#include "CirclePattern.h"

#define MAX_DEMON_BULLETS 10
#define MAX_DEMON_RAGE_BULLETS 5

struct Demon : Enemy
{
	Demon();

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

	bool IsBulletSequenceComplete(const BulletPatternGenerator &BulletPattern) override;

	//struct PulseBullet PulseBullet;
	//struct PulseBullet PulseBullet2ndWave;
	//struct PulseBullet PulseBullet3rdWave;
	//struct PulseBullet BulletRage[MAX_DEMON_RAGE_BULLETS];

	struct LinearPattern LinearBullet;
	struct LinearMultiPattern LinearMultiBullet;
	struct CirclePattern CircleBullet[5];

	Texture2D BulletSprite{};

	Rectangle DemonFrameRec{};

	unsigned short DemonSpriteFramesCounter{};
	unsigned short DemonCurrentFrame{};
	unsigned short FramesSpeed{10};

	WAVE BulletWave;

protected:

	void UpdateBullet();
	void UpdateDemonAnimation();
	void DrawBullet();

	void CheckCollisionWithPlayer() override;
	void CheckCollisionWithPlayerBullets() override;
	void CheckHealth() override;

	bool IsAtLocation(const Vector2& GoalLocation) override;

	bool IsLowHealth() const override;
};

