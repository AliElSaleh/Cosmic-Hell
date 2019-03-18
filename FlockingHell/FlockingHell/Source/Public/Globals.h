#pragma once

#include "Assets.h"

#define PANEL_WIDTH 350

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)
#define GetFont(Name) ASSETS.GetFont(#Name)
#define GetSound(Name) ASSETS.GetSound(#Name)
#define GetMusic(Name) ASSETS.GetMusic(#Name)
#define GetImage(Name) ASSETS.GetImage(#Name)