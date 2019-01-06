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
		FIVE_WAY_LINEAR,
		FIVE_WAY_LINEAR_LOCK_ON,
		SIX_WAY_LINEAR,
		SIX_WAY_LINEAR_LOCK_ON,
		SEVEN_WAY,
		EIGHT_WAY_LINEAR,
		EIGHT_WAY_LINEAR_LOCK_ON,
		ELEVEN_WAY_AIMING,
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
	};

	void Init();
	void Update();
	void Draw();

	void Delay();
	void Resume();
	void Pause();

	void StartShotRoutine();
	void SetBulletPattern(Pattern NewPattern);
	void AddBullet();
	void CalculateDirection(int i, Vector2 Target);


	Vector2 Center{}; // The spawn location
	Vector2 DummyLocation{};
	Vector2 PointOnCircle{};

	Pattern CurrentPattern{};

	Texture2D BulletSprite{};
	Texture2D DummySprite{};

	std::vector<struct Bullet> Bullet{};
	std::vector<struct Vector2> Points{}; // Points on circle
	std::vector<float> Angles{};

	unsigned short NumOfBullets{};
	unsigned short NumOfWay{};
	unsigned short NumOfSpiral{};
	unsigned short ShootRate{};
	float BulletSpeed{};
	float RotationSpeed{};
	float BulletRadius{};
	float CircleRadius{};
	float Angle{}; // In Degrees
	float DummySpeed{};

	bool bIsInProgress{};
	bool bIsSpacePressed{};
	bool bDebug{};

private:
	// Bullet pattern updates
	void UpdateBullet();
	void UpdateBulletLockOn();
	void UpdateBulletSpiral(bool Left, bool Right);
	void UpdateBulletSpiralMulti(bool Left, bool Right);
	void AddDebugCode();

	void CheckBulletOutsideWindow();
};

