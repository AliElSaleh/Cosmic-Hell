#pragma once
#include <raylib.h>
#include "State.h"
#include "Bullet.h"

#define MAX_PLAYER_BULLETS 50

struct Player
{
	void Init();
	void Update();
	void Draw() const;
	void DrawBullets() const;

	void ResetBullet(short Index);

	struct Bullet Bullet[MAX_PLAYER_BULLETS];

	Vector2 Location{};
	Vector2 Rotation{};
	Vector2 BulletSpawnLocation{};

	Texture2D Sprite{};
	Texture2D BulletSprite{};
	Rectangle Hitbox{};
	Rectangle PlayerFrameRec{};
	Rectangle BulletFrameRec{};

	State* GameState = nullptr;

	unsigned short XOffset{}; // Offset sprite location by X amount of pixels from mouse position
	unsigned short YOffset{}; // Offset sprite location by Y amount of pixels from mouse position
	unsigned short BulletXOffset = 5;
	signed short Health{};
	unsigned short PlayerSpriteFramesCounter = 0;
	unsigned short BulletSpriteFramesCounter = 0;
	unsigned short PlayerCurrentFrame = 0;
	unsigned short BulletCurrentFrame = 0;
	unsigned short FramesSpeed = 10;
	unsigned short ShootRate = 0;
	unsigned short BulletLevel = 1;
	unsigned short EnemiesKilled = 0;
	unsigned short BulletDamage = 0;

	bool bIsHit{};
	bool bIsDead{};
	bool bFirstLaunch{true};

	const char* Name{};

private:
	void InitBulletLevel(signed short Level);
	void UpdatePlayerAnimation(); // Loop through the 4 frames of the player sprite sheet
	void UpdateBulletAnimation(); // Loop through the 4 frames of the bullet sprite sheet
	void UpdateBullet(); // Release bullets when space bar is pressed
	void CheckBulletOutsideWindow();
	void CheckCollisionWithWindow();
	void CheckHealth();
	void CheckBulletLevel();

	bool bDebug{};
};
