#pragma once
#include <raylib.h>

#define Stringify(name) #name

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

struct Player;

struct Bullet
{
	void Init();
	void Update();
	void Draw() const;

	void CheckCollisionWithPlayerHitbox();
	void CheckCollisionWithPlayer() const;

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
