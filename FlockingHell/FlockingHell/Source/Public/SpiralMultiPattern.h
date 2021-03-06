#pragma once

#include "BulletPatternGenerator.h"
#include "SpiralMultiWayPattern.h"

#define Stringify(name) #name

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)


struct SpiralMultiPattern : BulletPatternGenerator
{
	SpiralMultiPattern();

	void Init() override;
	void Update() override;
	void Draw() override;

	void Delay(float Seconds) override;

	void AddDebugSwitchPatternCode() override;
	void DrawDebugInfo() override;

private:
	typedef SpiralMultiWayPattern BulletType;

	// For spiral mix pattern
	BulletType SpiralPattern;
};

