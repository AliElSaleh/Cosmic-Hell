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
	void Draw();

	Player& GetCurrentPlayer();

	struct Bullet Bullet[50];

	Vector2 Location;
	Vector2 Rotation;
	Vector2 BulletSpawnLocation;

	Texture2D Texture;
	Rectangle Hitbox;
	Rectangle FrameRec;

	State* GameState = nullptr;

	signed short XOffset;
	signed short YOffset;
	signed short Health;
	signed short SpriteFramesCounter = 0;
	signed short CurrentFrame = 0;
	signed short FramesSpeed = 10;
	unsigned short ShootRate = 0;

	bool bIsHit;
	bool bIsDead;
	bool bFirstLaunch;

	const char* Name;
};
