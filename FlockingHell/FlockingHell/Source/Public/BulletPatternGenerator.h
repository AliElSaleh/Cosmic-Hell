#pragma once

#include "Bullet.h"

#include <raylib.h>
#include <vector>

struct BulletPatternGenerator
{
	virtual ~BulletPatternGenerator();

	enum Pattern
	{
		LINEAR,
		LINEAR_LOCK_ON,
		LINEAR_AIMING,
		SPIRAL_RIGHT,
		SPIRAL_LEFT,
		SPIRAL_DOUBLE,
		SPIRAL_MULTI_RIGHT,
		SPIRAL_MULTI_LEFT,
		SPIRAL_MULTI_DOUBLE,
		SPIRAL_MIX,
		FIVE_WAY_LINEAR,
		FIVE_WAY_LINEAR_LOCK_ON,
		SIX_WAY_LINEAR,
		SIX_WAY_LINEAR_LOCK_ON,
		SEVEN_WAY,
		EIGHT_WAY_LINEAR,
		EIGHT_WAY_LINEAR_LOCK_ON,
		ELEVEN_WAY_AIMING,
		TWENTY_WAY,
		THIRTY_WAY,
		SPIRAL_FOUR_WAY,
		SPIRAL_FIVE_WAY,
		SPIRAL_SIX_WAY,
		SPIRAL_SEVEN_WAY,
		SPIRAL_EIGHT_WAY,
		SPIRAL_MULTI_THREE_WAY,
		SPIRAL_MULTI_FOUR_WAY,
		SPIRAL_MULTI_FIVE_WAY,
		SPIRAL_MULTI_SIX_WAY,
		SPIRAL_MULTI_SEVEN_WAY,
		SPIRAL_MULTI_EIGHT_WAY,
		SPREAD_EIGHT_WAY,
		SPREAD_EIGHT_WAY_LOCK_ON,
		CIRCLE,
		CIRCLE_HOLE,
		CIRCLE_HOLE_LOCK_ON,
		RANDOM,
		RANDOM_ALL_RANGE,
		RANDOM_AIMING,
		RANDOM_SPIRAL,
		RANDOM_SPIRAL_MULTI
		// Possibility
		// WAVING_FOUR_WAY
		// WAVING_EIGHT_WAY (ALL ROUND THE CIRCLE)
		// SPREAD_SEVEN_WAY_LOCK_ON
	};

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	virtual void Delay(float Seconds);
	virtual void Resume();
	virtual void Pause();

	void StartShotRoutine();

	void UpdateAnimation();

	void SetBulletPattern(Pattern NewPattern);
	void SetDelayAmount(float Seconds);
	void SetDebug(bool Condition);

	Texture2D BulletSprite{};
	Texture2D DummySprite{};

	std::vector<struct Bullet> Bullet{};
	std::vector<struct Vector2> Points{}; // Points on circle
	std::vector<float> Angles{};

	unsigned short NumOfBullets{};
	unsigned short NumOfSpiral{};
	unsigned short NumOfWay{};
	unsigned short ShootRate{};
	unsigned short FramesCounter{};

	float BulletSpeed{};
	float RotationSpeed{};
	float BulletRadius{};
	float CircleRadius{};
	float Angle{0.0f}; // In Degrees
	float DummySpeed{};
	float Offset{};

	Vector2 Location{GetMousePosition().x, GetMousePosition().y}; // Can be changed by other classes
	Vector2 Center{}; // The spawn location
	Vector2 TargetLocation{};

	struct Enemy *Enemy;

	bool bRelease{};

protected:	
	Pattern CurrentPattern{};

	float DelayAmount{};

	// Pattern initialisations
	void CreateLinearPattern(unsigned short AmountOfBullets, float Speed);
	void CreateSpiralPattern(bool Double, unsigned short AmountOfBullets, float Speed, float RotSpeed, float Radius);
	void CreateSpiralMultiPattern(bool Double, unsigned short AmountOfBullets, unsigned short AmountOfSpirals, float Speed, float RotSpeed, float Radius);
	void CreateLinearMultiPattern(unsigned short AmountOfBullets, unsigned short AmountOfWays, float Speed, float Radius);
	void CreateSpiralMultiPattern(bool MultiWay, unsigned short AmountOfBullets, unsigned short AmountOfSpirals, unsigned short AmountOfWays, float Speed, float RotSpeed, float Radius);
	void CreateSpreadPattern(unsigned short AmountOfBullets, unsigned short AmountOfWays, float Speed, float Radius);
	void CreateCirclePattern(bool Hole, unsigned short AmountOfBullets, float Speed, float Radius);
	void CreateRandomPattern(bool Spiral, unsigned short AmountOfBullets, unsigned short AmountOfSpirals, float Speed, float RotSpeed, float Radius);

	// Bullet updates
	void UpdateLinearBullet(bool LockOn);
	void UpdateSpiralBullet(bool Double);
	void UpdateSpiralMultiBullet();
	void UpdateLinearMultiBullet(bool Aiming);
	void UpdateSpiralMultiBullet(bool MultiWay);
	void UpdateSpreadBullet(bool LockOn);
	void UpdateCircleBullet(bool LockOn);
	void UpdateRandomBullet(bool Spiral);

	void DestroyActiveBullets();

	// Draw functions
	void DrawDummy() const;
	void DrawDebugPoint() const;
	void DrawDebugPoints(unsigned short Amount);
	virtual void DrawDebugInfo();

	void CheckBulletOutsideWindow();

	// Debug
	void AddDebugInitCode();
	void AddDebugUpdateCode();
	virtual void AddDebugSwitchPatternCode();

	bool bDelayed{};
	bool bIsInProgress{};
	bool bDebug{};

private:
	// Useful functions
	void AddBullet();
	void ApplyBulletMovement();
	void CalculateDirection(int i, Vector2 Target);

	// Bullet pattern updates
	void UpdateLinearPattern();
	void UpdateLinearTargetPattern(bool Aiming);
	void UpdateSpiralPattern(bool Left, bool Right);
	void UpdateSpiralMultiPattern(bool Left, bool Right);
	void UpdateLinearMultiPattern(bool Aiming);
	void UpdateSpiralMultiPattern(bool MultiWay);
	void UpdateSpreadPattern();
	void UpdateCirclePattern();
	void UpdateRandomPattern(bool AllRange, bool Aiming, bool Spiral);
};

