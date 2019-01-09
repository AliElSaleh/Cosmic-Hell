#pragma once
#include <raylib.h>

struct Player;

struct Bullet
{
	void Init();
	void Update();
	void Draw() const;

	void CheckCollisionWithPlayer();

	bool IsLocationYGreaterThan(float Y) const; // Is this bullet greater than (Y) on the Y axis

	void Clear();

	Vector2 Location{};
	Vector2 CollisionOffset{};
	Vector2 Center{};
	Vector2 Direction{};

	Texture2D Sprite{};

	struct Player* Player{};

	float Radius{10};
	float Speed{};

	unsigned short Damage{};

	bool bIsHit{};
	bool bActive{};

private:
	bool bDebug{};
};
