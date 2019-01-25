#pragma once

#include <raylib.h>

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

struct Explosion
{
	Explosion();

	void Init();
	void Draw() const;

	void Explode(Vector2 ExplosionLocation, unsigned short AmountOfExplosions);

private:
	Vector2 Location{};

	Texture2D Sprite{};

	// Animation
	Rectangle FrameRec{};
	unsigned short SpriteFramesCounter{};
	unsigned short CurrentFrame{};
	unsigned short Frames{};
	unsigned short FramesSpeed{30};

	unsigned short LoopsLeft{};
	
	bool bExploded{};

	void UpdateAnimation(unsigned short Loops);
};

