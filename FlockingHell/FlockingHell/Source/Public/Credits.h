#pragma once
#include <string>
#include <raylib.h>
#include <map>
#include <vector>

#include "State.h"

enum class Category
{
	NONE, PROGRAMMING_ART, MUSIC
};

struct Credits
{
	Credits();

	void Init();
	void AddName(const std::string& Name, Category Category);

	void Update();
	void Draw();

	State *GameState = nullptr;
	Music Music{};

private:
	std::string GetCategory(const std::string& Name);

	Vector2 Location{};
	Vector2 Spacing{};

	Font CreditsFont{};

	float CategorySpacing{};
	float Speed{};
	float Alpha{};
	float Volume{0.5f};

	std::map<std::string, Category> List; // A map of the persons name tied to their category

	bool bFadeOut{false};
	bool bFinished{false};
	bool bShowMessage{false};

	bool IsOutsideWindow() const;
};


