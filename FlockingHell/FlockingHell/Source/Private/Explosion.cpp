#include "Explosion.h"
#include "Assets.h"
#include "Globals.h"

Explosion::Explosion()
{
	Init();
}

void Explosion::Init()
{
	Location = {300.0f, 300.0f};

	Sprite = GetAsset(Explosion);

	Frames = 10;

	Health = 100;
	LoopsLeft = 10;

	FrameRec.x = 0.0f;
	FrameRec.y = 0.0f;
	FrameRec.width = Sprite.width/float(Frames);
	FrameRec.height = float(Sprite.height);

	bExploded = false;
}

void Explosion::Update()
{
	FramesCounter++;

	if (IsKeyDown(KEY_SPACE))
		Health--;
	
	if (!bExploded)
		if (Health < 0)
			Explode();



	//if (((FramesCounter/120)%2) == 1)
	//{
	//	if (!bExploded)
	//		Explode();
	//	else
	//		UpdateAnimation();
	//
	//}
}

void Explosion::Draw()
{
	if (bExploded)
		DrawTextureRec(Sprite, FrameRec, Location, WHITE);

	DrawText(FormatText("Health: %02i", Health), GetScreenWidth()-PANEL_WIDTH + 10, 50, 20, WHITE);
}

bool Explosion::Explode()
{
	UpdateAnimation();
	return true;
}

void Explosion::UpdateAnimation()
{
	SpriteFramesCounter++;

	if (SpriteFramesCounter >= (GetFPS()/FramesSpeed))
	{
		SpriteFramesCounter = 0;
		CurrentFrame++;
	
		if (CurrentFrame > Frames-1)
		{
 			CurrentFrame = 0;
			bExploded = true;
		}
	
		FrameRec.x = float(CurrentFrame)*float(Sprite.width)/Frames;
	}
}
