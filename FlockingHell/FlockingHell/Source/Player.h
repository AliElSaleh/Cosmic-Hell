#pragma once
#include "raylib.h"
#include "Bullet.h"
#include "State.h"

#define MAX_PLAYER_BULLETS 50

struct Player
{
	Player();

	void Init();
	void Update();
	void Draw();

	struct Bullet Bullet[50];

	Vector2 Location;
	Vector2 Rotation;
	Vector2 BulletSpawnLocation;

	Texture2D Texture;
	Rectangle Hitbox;
	Rectangle FrameRec;

	State* GameState = nullptr;

	float Size;
	signed short XOffset;
	signed short YOffset;
	signed short Health;
	signed short SpriteFramesCounter = 0;
	signed short CurrentFrame = 0;
	signed short FramesSpeed = 10;
	unsigned short ShootRate = 0;

	bool bIsHit;
	bool bIsDead;

	const char* Name;
};
