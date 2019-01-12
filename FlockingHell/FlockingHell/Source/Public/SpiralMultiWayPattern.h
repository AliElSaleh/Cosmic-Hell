#pragma once

#include "BulletPatternGenerator.h"

#define Stringify(name) #name

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

struct SpiralMultiWayPattern : BulletPatternGenerator
{
	SpiralMultiWayPattern();

	void Init() override;
	void Update() override;
	void Draw() override;

	void AddDebugSwitchPatternCode() override;
	void DrawDebugInfo() override;
};

