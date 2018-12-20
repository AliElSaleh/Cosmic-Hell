#pragma once
#include "raylib.h"
#include "State.h"
#include "Bullet.h"

#define MAX_PLAYER_BULLETS 50

struct Player
{
	Player();

	void Init();
	void Update();
	void Draw() const;

	Bullet Bullet[50];

	Vector2 Location{};
	Vector2 Rotation{};
	Vector2 BulletSpawnLocation{};

	Texture2D Sprite{};
	Texture2D BulletSprite{};
	Rectangle Hitbox{};
	Rectangle PlayerFrameRec{};
	Rectangle BulletFrameRec{};

	State* GameState = nullptr;

	unsigned short XOffset;
	unsigned short YOffset;
	signed short Health;
	unsigned short PlayerSpriteFramesCounter = 0;
	unsigned short BulletSpriteFramesCounter = 0;
	unsigned short PlayerCurrentFrame = 0;
	unsigned short BulletCurrentFrame = 0;
	unsigned short FramesSpeed = 10;
	unsigned short ShootRate = 0;

	bool bIsHit;
	bool bIsDead;
	bool bFirstLaunch;
	bool bDebug;

	const char* Name;

private:
	void UpdatePlayerAnimation(); // Loop through the 4 frames of the player sprite sheet
	void UpdateBulletAnimation(); // Loop through the 4 frames of the bullet sprite sheet
	void UpdateBullet(); // Update movement when space bar is pressed
	void CheckBulletOutsideWindow();
	void CheckCollisionWithWindow();
	void CheckPlayerHealth();
};
