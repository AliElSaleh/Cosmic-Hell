#pragma once

#include "BulletPatternGenerator.h"

struct SpiralPattern : BulletPatternGenerator
{
	SpiralPattern();

	void Init() override;
	void Update() override;
	void Draw() override;

private:
	void DrawDebugInfo();
};

