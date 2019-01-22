#pragma once
#include <raylib.h>

#define Stringify(name) #name

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

struct Player;

struct Bullet
{
	enum TYPE
	{
		NORMAL,
		FIRE,
		LASER,
	};

	void Init();
	void Update();
	void Draw() const;

	void UpdateBulletAnimation(TYPE BulletType);

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
	Rectangle BulletFrameRec{};

	unsigned short BulletSpriteFramesCounter{};
	unsigned short BulletCurrentFrame{};
	unsigned short FramesSpeed{6};

	bool bDebug{};
};
