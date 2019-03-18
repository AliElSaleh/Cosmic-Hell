#pragma once
#include <raylib.h>

struct HealthBar
{
	HealthBar() = default;
	HealthBar(float X, float Y, float Width, float Height, short Health, Color Color);

	void Update(float Health);
	void Draw();

private:
	void SetSize(float NormalizedHealth);

	Rectangle Bar{};
	Color Color{};

	float OriginalWidth{};
	float OriginalHealth{};

	bool bDebug{};
};

