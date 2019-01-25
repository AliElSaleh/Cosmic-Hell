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
		PLAYER,
		FIRE,
		LASER,
	};

	void Init();
	void InitFrames();
	void Update();
	void Draw() const;

	void UpdateAnimation(TYPE BulletType);

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

	unsigned short Frames{6};
	unsigned short Damage{};

	bool bIsHit{};
	bool bActive{};

	Rectangle FrameRec{};

	unsigned short SpriteFramesCounter{};
	unsigned short CurrentFrame{};
	unsigned short FramesSpeed{6};

private:
	bool bDebug{};
};
