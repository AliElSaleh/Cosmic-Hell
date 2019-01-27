#pragma once
#include <raylib.h>

struct RaylibLogo
{
	RaylibLogo();

	void Init();
	void Update();
	void Draw() const;

	bool bFinished{};
private:
	Vector2 Location{};

	unsigned short FramesCounter{};
	unsigned short LettersCount{};
	unsigned short TopSideRecWidth{16};
	unsigned short LeftSideRecHeight{16};
	unsigned short BottomSideRecWidth{16};
	unsigned short RightSideRecHeight{16};
	unsigned short AnimState{0};   // Tracking animation states

	float Alpha{1.0f};             // Used for fading the logo out
};

