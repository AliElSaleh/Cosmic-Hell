#pragma once
#include <raylib.h>
#include "Bullet.h" // <--- Forward declares player struct

#define MAX_ENEMY_BULLETS 50

struct Enemy
{
	Enemy();

	void Init();
	void Update();
	void Draw();

	Bullet Bullet[MAX_ENEMY_BULLETS];

	Vector2 Location;
	Texture2D Sprite;
	Rectangle Hitbox;

	unsigned short Health;
	unsigned short Damage;

	// References to other classes
	Player* Player = nullptr;
};

