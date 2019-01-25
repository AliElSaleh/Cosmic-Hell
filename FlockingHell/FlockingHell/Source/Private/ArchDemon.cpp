#include "ArchDemon.h"
#include "Assets.h"

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

ArchDemon::ArchDemon()
{
	ArchDemon::Init();
}

void ArchDemon::Init()
{
	Location = {300.0f, 300.0f};
	Sprite = GetAsset(ArchDemon);

	TargetRadius = 10.0f;

	Health = 100;
	Speed = 140.0f;
	Damage = GetRandomValue(1, 3);
	ShootRate = 5;
	bActive = true;
	bIsDead = false;
	bDebug = false;

	FrameRec.x = 0.0f;
	FrameRec.y = 0.0f;
	FrameRec.width = float(Sprite.width)/10; // 10 frames
	FrameRec.height = float(Sprite.height);
}

void ArchDemon::Update()
{
	SpriteFramesCounter++;

	UpdateDemonAnimation();
}

void ArchDemon::Draw()
{
	if (bActive && !bIsDead)
		DrawTextureRec(Sprite, FrameRec, Location, WHITE);
}

void ArchDemon::Flock(std::vector<Enemy*>* Boids)
{
	
}

void ArchDemon::UpdateDemonAnimation()
{
	// Arch Demon sprite animation
	if (SpriteFramesCounter >= (GetFPS()/FramesSpeed))
	{
		SpriteFramesCounter = 0;
		CurrentFrame++;
	
		if (CurrentFrame > 4)
			CurrentFrame = 0;
	
		FrameRec.x = float(CurrentFrame)*float(Sprite.width)/10;
	}
}
