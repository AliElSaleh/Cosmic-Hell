#include "Explosion.h"
#include "Assets.h"

Explosion::Explosion()
{
	Init();
}

void Explosion::Init()
{
	Location = {300.0f, 300.0f};

	Sprite = GetAsset(Explosion);

	Frames = 10;

	LoopsLeft = 0;

	FrameRec.x = 0.0f;
	FrameRec.y = 0.0f;
	FrameRec.width = Sprite.width/float(Frames);
	FrameRec.height = float(Sprite.height);

	bExploded = false;
}

void Explosion::Draw() const
{
	if (bExploded)
		DrawTextureRec(Sprite, FrameRec, Location, WHITE);
}

void Explosion::Explode(const Vector2 ExplosionLocation, const unsigned short AmountOfExplosions)
{
	Location = ExplosionLocation;

	UpdateAnimation(AmountOfExplosions);
}

void Explosion::UpdateAnimation(const unsigned short Loops)
{
	if (LoopsLeft < Loops * Frames)
	{
 		SpriteFramesCounter++;

		// Move to next frame
		if (SpriteFramesCounter >= (GetFPS()/FramesSpeed))
		{
			SpriteFramesCounter = 0;
			CurrentFrame++;
		
			if (CurrentFrame > Frames-1)
 				CurrentFrame = 0;

			FrameRec.x = float(CurrentFrame)*float(Sprite.width)/Frames;

			bExploded = true;
			LoopsLeft++;
		}
	}
}
