#pragma once

#include "BulletPatternGenerator.h"

#define Stringify(name) #name

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

struct SpiralPattern : BulletPatternGenerator
{
	SpiralPattern();

	void Init() override;
	void Update() override;
	void Draw() override;

	void DrawDebugInfo() override;
};

