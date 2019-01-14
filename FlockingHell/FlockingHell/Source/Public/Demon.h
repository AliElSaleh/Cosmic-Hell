#pragma once

#include "Enemy.h"
#include "LinearPattern.h"
#include "LinearMultiPattern.h"
#include "CirclePattern.h"
#include "SpiralPattern.h"

struct Demon : Enemy
{
	Demon();

	enum WAVE
	{
		FIRST,
		FIRST_A,
		FIRST_B,
		FIRST_C,
		SECOND,
		SECOND_A,
		SECOND_B,
		SECOND_C,
		THIRD,
		THIRD_A,
		THIRD_B,
		THIRD_C,
		RAGE
	};

	void Init() override;
	void Update() override;
	void Draw() override;

	bool IsBulletSequenceComplete(const BulletPatternGenerator &BulletPattern) override;

	//struct PulseBullet PulseBullet3rdWave;
	//struct PulseBullet BulletRage[MAX_DEMON_RAGE_BULLETS];

	struct SpiralPattern SpiralBullet[4];
	struct LinearMultiPattern LinearMultiBullet[4];
	struct CirclePattern CircleBullet[20];

	Texture2D BulletSprite{};

	Rectangle DemonFrameRec{};

	unsigned short DemonSpriteFramesCounter{};
	unsigned short DemonCurrentFrame{};
	unsigned short FramesSpeed{10};

	WAVE BulletWave;

protected:
	static float Round(float Number);

	void UpdateBullet();
	void UpdateDemonAnimation();
	void DrawBullet();

	void CheckCollisionWithPlayer() override;
	void CheckCollisionWithPlayerBullets() override;
	void CheckHealth() override;

	bool IsAtLocation(const Vector2& GoalLocation) override;

	bool IsLowHealth() const override;
};

