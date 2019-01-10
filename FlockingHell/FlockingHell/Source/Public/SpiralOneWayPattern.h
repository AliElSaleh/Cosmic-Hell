#pragma once

#include "BulletPatternGenerator.h"

#define Stringify(name) #name

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)

struct SpiralOneWayPattern : BulletPatternGenerator
{
	SpiralOneWayPattern();

	void Init() override;
	void Update() override;
	void Draw() override;

	void AddDebugSwitchPatternCode() override;
	void DrawDebugInfo() override;
};

