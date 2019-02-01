#pragma once
#include <raylib.h>
#include "State.h"
#include "Bullet.h"
#include <vector>
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
	Vector2 Center{};
	Vector2 BulletSpawnLocation{};

	Texture2D Sprite{};

	std::vector<Texture2D> Heart;
	std::vector<Texture2D> Bomb;

	Rectangle Hitbox{};
	Rectangle Spritebox{};
	Rectangle PlayerFrameRec{};

	State* GameState = nullptr;

	signed short Health{};
	unsigned short XOffset{}; // Offset sprite location by X amount of pixels from mouse position
	unsigned short YOffset{}; // Offset sprite location by Y amount of pixels from mouse position
	unsigned short BulletXOffset = 5;
	unsigned short PlayerSpriteFramesCounter = 0;
	unsigned short PlayerCurrentFrame = 0;
	unsigned short FramesSpeed = 10;
	unsigned short ShootRate = 0;
	unsigned short BulletLevel = 1;
	unsigned short BossKilled = 0;
	unsigned short EnemiesKilled = 0;
	unsigned short BulletDamage = 0;
	unsigned int Score = 0;
	unsigned int GrazingScore = 0;

	bool bInvincible{};
	bool bIsHit{};
	bool bIsDead{};

	const char* Name{};

private:
	void InitBulletLevel(signed short Level);
	void UpdatePlayerAnimation(); // Loop through the 4 frames of the player sprite sheet
	void UpdateBullet(); // Release bullets when space bar is pressed
	void CheckBulletOutsideWindow();
	void CheckCollisionWithWindow();
	void CheckHealth();
	void CheckBulletLevel();

	void Invincibility(bool Mode, float Seconds);

	unsigned short FramesCounter{};
	unsigned short PlayerHitFramesCounter{};

	bool bDebug{};
};
