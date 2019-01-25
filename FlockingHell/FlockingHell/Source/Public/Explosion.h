#pragma once

#include <raylib.h>

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

struct Explosion
{
	Explosion();

	void Init();
	void Update();
	void Draw();

	bool Explode();

	bool bExploded{};
private:
	Vector2 Location{};

	Texture2D Sprite{};

	Rectangle FrameRec{};

	unsigned short SpriteFramesCounter{};
	unsigned short CurrentFrame{};
	unsigned short Frames{};
	unsigned short FramesSpeed{30};

	int Health{};
	int FramesCounter{};
	int LoopsLeft{};

	void UpdateAnimation();
};

