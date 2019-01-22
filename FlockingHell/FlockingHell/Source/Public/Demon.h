#pragma once

#include "Enemy.h"
#include "LinearPattern.h"
#include "LinearMultiPattern.h"
#include "CirclePattern.h"
#include "SpiralPattern.h"
#include "SpiralMultiPattern.h"

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

	Vector2 Seek(const Vector2& DestLocation);
	Vector2 Limit(Vector2 V, const float Amount) const;

	bool IsBulletSequenceComplete(const BulletPatternGenerator &BulletPattern) override;

	struct SpiralPattern SpiralBullet[4];
	struct SpiralMultiPattern RageBullet;
	struct LinearMultiPattern LinearMultiBullet[4];
	struct CirclePattern CircleBullet[20];

	Texture2D BulletSprite{};

	Rectangle DemonFrameRec{};

	unsigned short DemonSpriteFramesCounter{};
	unsigned short DemonCurrentFrame{};
	unsigned short FramesSpeed{10};

	WAVE BulletWave;

protected:
	Vector2 DesiredVelocity{};
	Vector2 Steering{};

	float Mass{};
	float MaxForce{};
	float MaxVelocity{};

	void ApplyForce(const Vector2 Force);

	void UpdateBullet();
	void UpdateDemonAnimation();
	void DrawBullet();

	void CheckCollisionWithPlayer() override;
	void CheckCollisionWithPlayerBullets() override;
	void CheckHealth() override;

	bool IsAtLocation(const Vector2& GoalLocation) override;

	bool IsLowHealth() const override;
};

