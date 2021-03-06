#pragma once
#include <raylib.h>
#include <vector>

#include "State.h"
#include "Bullet.h"
#include "Stats.h"

#define MAX_PLAYER_BULLETS 100

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

	struct Rectangle Hitbox{};
	struct Rectangle Spritebox{};
	struct Rectangle PlayerFrameRec{};

	State* GameState = nullptr;

	struct Stats Stats{};

	signed short Health{};
	signed short HeartsLeft = 0;
	signed short BombsLeft = 0;
	signed int Score = 0;
	unsigned short Deaths = 0;
	unsigned short Hits = 0;
	unsigned short BombsUsed = 0;
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
	unsigned short HealthRegenTimer = 0;
	unsigned short BombRegenTimer = 0;
	unsigned short BombCooldownTimer = 0;
	unsigned int GrazingScore = 0;

	bool bChangeMusic{};
	bool bInvincible{};
	bool bWasBombUsed{};
	bool bCanUseBomb{};
	bool bIsHit{};
	bool bIsDead{};
	bool bWarshipDefeated{};

	std::string Name{};

private:
	void InitBulletLevel(signed short Level);
	void UpdatePlayerAnimation(); // Loop through the 4 frames of the player sprite sheet
	void UpdateBullet(); // Release bullets when space bar is pressed
	void CheckBulletOutsideWindow();
	void CheckCollisionWithWindow();
	void CheckHealth();

	void Invincibility(bool Mode, float Seconds);

	unsigned short FramesCounter{};
	unsigned short PlayerHitFramesCounter{};

	bool bWasHit{false};
	bool bDebug{};
};
