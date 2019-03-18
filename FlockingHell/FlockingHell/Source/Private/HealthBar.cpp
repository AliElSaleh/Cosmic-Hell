#include "HealthBar.h"
#include <cstdio>

HealthBar::HealthBar(const float X, const float Y, const float Width, const float Height, const short Health, const ::Color Color)
{
	Bar.x = X;
	Bar.y = Y;
	Bar.width = Width;
	Bar.height = Height;
	this->Color = Color;

	OriginalWidth = Width;
	OriginalHealth = Health;

	bDebug = false;
}

void HealthBar::Update(const float Health)
{
	if (Bar.width > 0.0f)
		SetSize(Health/OriginalHealth);
	else
		Bar.width = 0.0f;

	if (bDebug)
	{
		printf("Bar width: %f\n", Bar.width);
		printf("Health: %f\n", Health);
		printf("NormalizedHealth: %f\n", Health/OriginalHealth);
	}
}

void HealthBar::Draw()
{
	DrawRectangleRec(Bar, Color);
}

void HealthBar::SetSize(const float NormalizedHealth)
{
	Bar.width = OriginalWidth * NormalizedHealth;
}

