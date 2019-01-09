#pragma once
#include "BulletPatternGenerator.h"

struct LinearPattern : BulletPatternGenerator
{
	LinearPattern();

	void Init() override;
	void Update() override;
	void Draw() override;
};

