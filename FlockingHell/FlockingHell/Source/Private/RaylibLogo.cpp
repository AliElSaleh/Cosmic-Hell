#include "RaylibLogo.h"


RaylibLogo::RaylibLogo()
{
	Init();
}

void RaylibLogo::Init()
{
	Location = {float(GetScreenWidth())/2 - 128, float(GetScreenHeight())/2 - 128};
	bFinished = false;
}

void RaylibLogo::Update()
{
	if (AnimState == 0)
	{
		FramesCounter++;

        if (FramesCounter/12)       // Every 12 frames, one more letter!
        {
            LettersCount++;
            FramesCounter = 0;
        }

		if (LettersCount >= 20)
		{
			AnimState = 1;
			FramesCounter = 0;
			LettersCount = 0;
		}
	}
	
	if (AnimState == 1)                 // State 0: Small box blinking
    {
        FramesCounter++;

        if (FramesCounter == 120)
        {
            AnimState = 2;
            FramesCounter = 0;      // Reset counter... will be used later...
        }
    }
    
	if (AnimState == 2)            // State 1: Top and left bars growing
    {
        TopSideRecWidth += 4;
        LeftSideRecHeight += 4;

        if (TopSideRecWidth == 256) AnimState = 3;
    }
    
	if (AnimState == 3)            // State 2: Bottom and right bars growing
    {
        BottomSideRecWidth += 4;
        RightSideRecHeight += 4;

        if (BottomSideRecWidth == 256) AnimState = 4;
    }
    
	if (AnimState == 4)            // State 3: Letters appearing (one by one)
    {
        FramesCounter++;

        if (FramesCounter/12)       // Every 12 frames, one more letter!
        {
            LettersCount++;
            FramesCounter = 0;
        }

        if (LettersCount >= 30)     // When all letters have appeared, just fade out everything
        {
            Alpha -= 0.02f;

            if (Alpha <= 0.0f)
            {
                Alpha = 0.0f;
                AnimState = 5;
            }

			bFinished = true;
        }
    }
}

void RaylibLogo::Draw() const
{
	if (AnimState == 0)
	{
		RDrawText(SubText("Developed using", 0, LettersCount), int(Location.x + 40), int(Location.y + 100), 22, Fade(GRAY, Alpha));
	}
	
	if (AnimState == 1)
    {
        if ((FramesCounter/15)%2) DrawRectangle(int(Location.x), int(Location.y), 16, 16, BLACK);
    }
    
	if (AnimState == 2)
    {
        DrawRectangle(int(Location.x), int(Location.y), TopSideRecWidth, 16, BLACK);
        DrawRectangle(int(Location.x), int(Location.y), 16, LeftSideRecHeight, BLACK);
    }
    
	if (AnimState == 3)
    {
        DrawRectangle(int(Location.x), int(Location.y), TopSideRecWidth, 16, BLACK);
        DrawRectangle(int(Location.x), int(Location.y), 16, LeftSideRecHeight, BLACK);

        DrawRectangle(int(Location.x) + 240, int(Location.y), 16, RightSideRecHeight, BLACK);
        DrawRectangle(int(Location.x), int(Location.y) + 240, BottomSideRecWidth, 16, BLACK);
    }
    
	if (AnimState == 4)
    {
        DrawRectangle(int(Location.x), int(Location.y), TopSideRecWidth, 16, Fade(BLACK, Alpha));
        DrawRectangle(int(Location.x), int(Location.y) + 16, 16, LeftSideRecHeight - 32, Fade(BLACK, Alpha));

        DrawRectangle(int(Location.x) + 240, int(Location.y) + 16, 16, RightSideRecHeight - 32, Fade(BLACK, Alpha));
        DrawRectangle(int(Location.x), int(Location.y) + 240, BottomSideRecWidth, 16, Fade(BLACK, Alpha));

        DrawRectangle(GetScreenWidth()/2 - 112, GetScreenHeight()/2 - 112, 224, 224, Fade(RAYWHITE, Alpha));

        RDrawText(SubText("raylib", 0, LettersCount), GetScreenWidth()/2 - 44, GetScreenHeight()/2 + 48, 50, Fade(BLACK, Alpha));
    }
}

