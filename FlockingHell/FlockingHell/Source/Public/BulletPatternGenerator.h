#pragma once

#include <raylib.h>
#include <vector>

struct BulletPatternGenerator
{
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
		SIX_WAY_LINEAR,
		SEVEN_WAY,
		EIGHT_WAY_LINEAR,
		ELEVEN_WAY_AIMING,
		TWENTY_WAY,
		THIRTY_WAY,
		SPIRAL_FOUR_WAY,
		SPIRAL_MULTI_FOUR_WAY,
		SPREAD_EIGHT_WAY,
		SPREAD_EIGHT_WAY_LOCK_ON,
		CIRCLE,
		CIRCLE_HOLE,
		CIRCLE_HOLE_LOCK_ON,
		RANDOM,
		RANDOM_AIMING,
		RANDOM_SPIRAL,
		SINE_WAVE_MULTI_WAY_AIMING
		// Possibility
		// WAVING_FOUR_WAY
		// WAVING_EIGHT_WAY (ALL ROUND THE CIRCLE)
		// SPREAD_SEVEN_WAY_LOCK_ON
	};

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	virtual void Delay();
	virtual void Resume();
	virtual void Pause();

	void StartShotRoutine();
	void SetBulletPattern(Pattern NewPattern);

	Texture2D BulletSprite{};
	Texture2D DummySprite{};

	std::vector<struct Bullet> Bullet{};
	std::vector<struct Vector2> Points{}; // Points on circle
	std::vector<float> Angles{};

	unsigned short NumOfBullets{};
	unsigned short NumOfSpiral{};
	unsigned short NumOfWay{};
	unsigned short ShootRate{};

	float BulletSpeed{};
	float RotationSpeed{};
	float BulletRadius{};
	float CircleRadius{};
	float Angle{0.0f}; // In Degrees
	float DummySpeed{};

protected:
	Pattern CurrentPattern{};

	// Debug
	void AddDebugInitCode();
	void AddDebugUpdateCode();

	// Pattern initialisations
	void CreateLinearPattern(unsigned short AmountOfBullets, float Speed);
	void CreateSpiralPattern(bool Double, unsigned short AmountOfBullets, float Speed, float RotSpeed, float Radius);
	void CreateSpiralMultiPattern(bool Double, unsigned short AmountOfBullets, unsigned short AmountOfSpirals, float Speed, float RotSpeed, float Radius);
	void CreateSpiralMixPattern(bool Left, unsigned short AmountOfBullets, unsigned short AmountOfSpirals, float Speed, float RotSpeed, float Radius);
	void CreateLinearMultiPattern(unsigned short AmountOfBullets, unsigned short AmountOfWays, float Speed, float Radius);

	// Bullet updates
	void UpdateLinearBullet(bool LockOn);
	void UpdateSpiralBullet(bool Double);
	void UpdateSpiralMultiBullet();
	void UpdateLinearMultiBullet(bool Aiming);

	// Bullet pattern updates
	void UpdateLinearPattern();
	void UpdateLinearTargetPattern();
	void UpdateSpiralPattern(bool Left, bool Right);
	void UpdateSpiralMultiPattern(bool Left, bool Right);
	void UpdateLinearMultiPattern(bool Aiming);

	// Draw functions
	void DrawDummy() const;
	void DrawDebugPoint() const;
	void DrawDebugPoints(unsigned short Amount);

	// Useful functions
	void AddBullet();
	void ApplyBulletMovement();
	void CalculateDirection(int i, Vector2 Target);
	void CheckBulletOutsideWindow();
	
	Vector2 Center{}; // The spawn location
	Vector2 DummyLocation{};
	Vector2 PointOnCircle{};

	unsigned short FramesCounter{0};

	bool bIsInProgress{};
	bool bIsSpacePressed{};
	bool bDebug{};
};

