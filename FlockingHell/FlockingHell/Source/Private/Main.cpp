#include "Globals.h"
#include "Assets.h"
#include "Main.h"

#include <fstream>
#include <iostream>
#include <string>

#pragma comment (lib, "ws2_32.lib")

#define ASSETS Assets::Get()
#define GetAsset(Name) ASSETS.GetSprite(#Name)
#define GetFont(Name) ASSETS.GetFont(#Name)
#define GetSound(Name) ASSETS.GetSound(#Name)
#define GetMusic(Name) ASSETS.GetMusic(#Name)

int main()
{
	Init();
	
	// Main game loop
	while (!WindowShouldClose())
	{
		Update();
		Draw();
	}

	Unload();

	CloseAudioDevice();

	closesocket(Socket);
	WSACleanup();

	RCloseWindow();

    return 0;
}

// Function Definitions
//-----------------------------------------

void Init()
{
	// Window setup
	const unsigned short SCREEN_WIDTH = 700+PANEL_WIDTH; // From 700 to 1050 on X axis is reserved for player stats
	const unsigned short SCREEN_HEIGHT = 800;
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cosmic Hell");
	SetTargetFPS(120);

	InitAudioDevice();

	SetGameState(GAME);

	ASSETS.LoadSprites();
	ASSETS.LoadFonts();
	ASSETS.LoadSounds();
	ASSETS.LoadMusic();

	MenuMusic = GetMusic(Menu);
	GameMusic = GetMusic(DemonStage);

	LoadHighscore();

	//ConnectToServer();

	InitGame();
}

void InitGame()
{
	switch (GameState)
	{
		case SPLASH:
			SplashLogo.Init();
		break;

		case MENU:
			PlayMusicStream(MenuMusic);

			// Initialise sprites
			Background = GetAsset(Background);
			MenuTitle = GetAsset(CosmicHellTitle);

			// Initialise planets
			for (int i = 0; i < 5; ++i)
			{
				BigPlanet[i].Init();
				BigPlanet[i].Sprite = GetAsset(Planet);
			}

			BigPlanet[0].Location = {52.0f, 68.0f};
			BigPlanet[1].Location = {231.0f, 204.0f};
			BigPlanet[2].Location = {584.0f, 99.0f};
			BigPlanet[3].Location = {432.0f, 558.0f};
			BigPlanet[4].Location = {876.0f, 272.0f};

			for (int i = 0; i < 2; ++i)
			{
				RingPlanet[i].Init();
				RingPlanet[i].Sprite = GetAsset(RingPlanet);
			}

			RingPlanet[0].Location = {250.0f, 458.0f};
			RingPlanet[1].Location = {800.0f, 85.0f};

			MenuFramesCounter = 0;
			FadeAlpha = 1.0f;

			bBegan = false;	
			SplashLogo.bFinished = true;
		break;

		case TEST:
			
		break;

		case FLOCKING_TEST:
		break;

		case GAME:
			PlayMusicStream(GameMusic);

			// Initialise Player
			Player.Init();
			Player.GameState = &GameState;

			SendData();
			
			// Initialise enemy manager system
			for (int i = 0; i < EnemyManager.Enemies.size(); i++)
			{
				EnemyManager.Enemies[i]->Player = &Player;
				EnemyManager.Enemies[i]->Init();
			}

			for (int i = 0; i < EnemyManager.FlockOfEnemies.size(); i++)
			{
				EnemyManager.FlockOfEnemies[i]->Player = &Player;				
				EnemyManager.FlockOfEnemies[i]->Init();
			}

			// Initialise stars
			for (int i = 0; i < MAX_SMALL_STARS; i++)
				SmallStars[i].Init();

			for (int i = 0; i < MAX_CLOSE_STARS/2; i++)
			{
				CloseStars[i].Init();
				CloseStars[i].Speed = 300.0f;
			}
			
			for (int i = 30; i < MAX_CLOSE_STARS; i++)
			{
				CloseStars[i].Init();
				CloseStars[i].Speed = 200.0f;
			}

			for (int i = 0; i < MAX_FAR_STARS; i++)
			{
				FarStars[i].Init();
				FarStars[i].Speed = 10.0f;
			}

			// Initialise rplanets
			for (int i = 0; i < 5; ++i)
			{
				BigPlanet[i].Init();
				BigPlanet[i].Sprite = GetAsset(Planet);
			}

			BigPlanet[0].Location = {52.0f, 68.0f};
			BigPlanet[1].Location = {231.0f, 204.0f};
			BigPlanet[2].Location = {584.0f, 99.0f};
			BigPlanet[3].Location = {432.0f, 558.0f};
			BigPlanet[4].Location = {876.0f, 272.0f};

			for (int i = 0; i < 2; ++i)
			{
				RingPlanet[i].Init();
				RingPlanet[i].Sprite = GetAsset(RingPlanet);
				RingPlanet[i].Speed = 40.0f;
			}

			RingPlanet[0].Location = {250.0f, 458.0f};
			RingPlanet[1].Location = {800.0f, 85.0f};

			// Initialise sprites
			Panel = GetAsset(Panel);

			FadeAlpha = 1.0f;

			bDebug = false;
		break;

		default:
		break;
	}
}

void SetGameState(const State State)
{
	GameState = State;
}

void Update()
{
	SetWindowTitle(FormatText("Cosmic Hell | FPS: %02i", GetFPS()));

	switch (GameState)
	{
		case SPLASH:
			UpdateSplashScreen();
		break;

		case MENU:
			UpdateMenuScreen();
		break;

		case GAME:
			UpdateGame();
		break;

		case PAUSE:
			UpdatePauseScreen();
		break;

		case DEATH:
			UpdateDeathScreen();
		break;

		case WIN:
			UpdateWinScreen();
		break;

		case STATS:
			UpdateStatsScreen();
		break;

		case HIGHSCORE:
			UpdateHighscoreScreen();
		break;

		case TEST:
			UpdateTestScene();
		break;

		case FLOCKING_TEST:
			UpdateFlockingScene();
		break;

		default:
		break;
	}
}

void Draw()
{
	BeginDrawing();

	switch (GameState)
	{
		case SPLASH:
			DrawSplashScreen();
		break;

		case MENU:
			DrawMenuScreen();
		break;

		case GAME:
			DrawGame();
		break;

		case PAUSE:
			DrawPauseScreen();
		break;

		case DEATH:
			DrawDeathScreen();
		break;

		case WIN:
			DrawWinScreen();
		break;

		case STATS:
			DrawStatsScreen();
		break;

		case HIGHSCORE:
			DrawHighscoreScreen();
		break;

		case TEST:
			DrawTestScene();
		break;

		case FLOCKING_TEST:
			DrawFlockingScene();
		break;

		default:
		break;
	}

	EndDrawing();
}

void UpdateSplashScreen()
{
	SetExitKey(0);

	SplashLogo.Update();

	if (SplashLogo.bFinished)
	{
	    FramesCounter++;

		if (((FramesCounter/240)%2) == 1)
		{
			FramesCounter = 0;
			SetGameState(MENU);
			InitGame();
		}
	}
}

void UpdateMenuScreen()
{	
	SetExitKey(KEY_ESCAPE);

	UpdateMusicStream(MenuMusic);
	StopMusicStream(GameMusic);

	MenuFramesCounter++;

	// Any key press functionality
	for (int i = KEY_A; i < KEY_Z; i++)
		if (IsKeyPressed(i))
		{
			RPlaySound(GetSound(MenuSelect));
			
			if (!bBegan)
				bBegan = true;

			FramesCounter = 0;
			FadeAlpha -= 0.0f;
		}

	if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
	{
		if (!bBegan)
			bBegan = true;

		FramesCounter = 0;
		FadeAlpha -= 0.0f;
	}

	// Fade out to black on key press
	if (bBegan)
	{	
		FramesCounter++;
		
		FadeAlpha += 0.01f;

		if (FadeAlpha >= 1.0f)
			FadeAlpha = 1.0f;

		if (FramesCounter/310%2)
		{
			FramesCounter = 0;

			SetGameState(GAME);
			EnemyManager.Reset();
			EnemyManager.Init();
			InitGame();
		}
	}
	// Fade in from splash screen
	else
	{
		FramesCounter++;

		FadeAlpha -= 0.01f;

		if (FadeAlpha <= 0)
			FadeAlpha = 0.0f;
	}

	// Move planets in the background
	for (int i = 0; i < 5; ++i)
		BigPlanet[i].Update();

	for (int i = 0; i < 2; ++i)
		RingPlanet[i].Update();
}

void UpdateGame()
{
	FramesCounter++;
	SetExitKey(0);
	HideCursor();
	
	UpdateMusicStream(GameMusic);
	StopMusicStream(MenuMusic);

	if (GetMousePosition().x > GetScreenWidth()-PANEL_WIDTH)
		RShowCursor();

	FadeAlpha -= 0.03f;

	if (FadeAlpha <= 0.0f)
		FadeAlpha = 0.0f;

	// Stars
	for (int i = 0; i < MAX_FAR_STARS; i++)
		FarStars[i].Update();

	for (int i = 0; i < MAX_SMALL_STARS; i++)
		SmallStars[i].Update();

	// Planets
	for (int i = 0; i < 5; i++)
		BigPlanet[i].UpdateY();

	for (int i = 0; i < 5; i++)
		RingPlanet[i].UpdateY();

	// Stars
	for (int i = 0; i < MAX_CLOSE_STARS; i++)
		CloseStars[i].Update();

	// Player
	Player.Update();

	// Enemy Manager system
	EnemyManager.Update();

	if (IsKeyPressed(KEY_ESCAPE))
		SetGameState(PAUSE);
}

void UpdatePauseScreen()
{
	DisableCursor();

	if (IsKeyPressed(KEY_Q))
	{
		SetGameState(MENU);
		EnemyManager.Reset();
		EnemyManager.Init();
		InitGame();
	}
	
	if (IsKeyPressed(KEY_ESCAPE))
		SetGameState(GAME);
}

void UpdateDeathScreen()
{
	SetExitKey(0);

	StopMusicStream(GameMusic);

	if (IsKeyPressed(KEY_ENTER))
	{
		SetGameState(GAME);
		EnemyManager.Reset();
		EnemyManager.Init();
		InitGame();
	}

	if (IsKeyPressed(KEY_ESCAPE))
	{
		SetGameState(MENU);
		InitGame();
	}

	TotalScore = Player.Score + Player.GrazingScore;

	if (ScoreCounter < TotalScore)
		ScoreCounter += 2 * 10;
	else
		ScoreCounter = TotalScore;

	for (int i = KEY_A; i < KEY_Z; i++)
		if (IsKeyPressed(i))
			ScoreCounter = TotalScore;

	// Highscore check
	if (Highscore < TotalScore)
	{
		Highscore = TotalScore;

		SaveHighscore();
		SendData();
	}
}

void UpdateWinScreen()
{
	SetExitKey(0);

	if (IsKeyPressed(KEY_ENTER))
	{
		SetGameState(CREDITS);
		EnemyManager.Reset();
		InitGame();
	}

	if (IsKeyPressed(KEY_ESCAPE))
	{
		SetGameState(MENU);
		EnemyManager.Reset();
		EnemyManager.Init();
		InitGame();
	}
}

void UpdateStatsScreen()
{
}

void UpdateHighscoreScreen()
{
}

void UpdateTestScene()
{
	FramesCounter++;
	SetExitKey(KEY_ESCAPE);

	//if (IsKeyPressed(KEY_SPACE))
	//	Pattern<BulletType>.bRelease = true;

	//Pattern<BulletType>.Location = {GetMousePosition().x, GetMousePosition().y};
	//Pattern<BulletType>.Update();
}

void UpdateFlockingScene()
{
	SetExitKey(KEY_ESCAPE);

}

void DrawSplashScreen()
{
	ClearBackground(WHITE);

	SplashLogo.Draw();
}

void DrawMenuScreen()
{
	// Space background
	DrawTexture(Background, 0, 0, WHITE);

	// Planets
	for (int i = 0; i < 5; i++)
		BigPlanet[i].Draw();

	for (int i = 0; i < 2; i++)
		RingPlanet[i].Draw();

	// Title
	DrawTexture(MenuTitle, int(TitleLocation.x), int(TitleLocation.y), WHITE);

	// PRESS ANY KEY
	 if ((MenuFramesCounter/60)%2)
		 DrawTexture(GetAsset(PressAnyKey), 353, 665, WHITE);

	// Fade screen out on key press
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, FadeAlpha));
}

void DrawGame()
{
	// Space background
	DrawTexture(GetAsset(GameBackground), 0, 0, GRAY);

	// Stars
	for (int i = 0; i < MAX_SMALL_STARS; i++)
		SmallStars[i].Draw();

	for (int i = 0; i < MAX_CLOSE_STARS; i++)
		CloseStars[i].Draw();
	
	for (int i = 0; i < MAX_FAR_STARS; i++)
		FarStars[i].Draw();

	// Planets
	for (int i = 0; i < 5; i++)
		BigPlanet[i].Draw();

	for (int i = 0; i < 2; i++)
		RingPlanet[i].Draw();

	// Player
	Player.Draw();

	// Enemy manager system
	EnemyManager.Draw();

	Player.DrawBullets();

	// Stats panel
	DrawTexture(Panel, GetScreenWidth()-PANEL_WIDTH, 0, WHITE);
 	DrawTexture(GetAsset(Divider), GetScreenWidth()-PANEL_WIDTH-5, 0, WHITE); // Divider
	DrawFPS(GetScreenWidth()-80, GetScreenHeight() - 25);
	DrawGameUI();

	// Fade screen in when entering this state
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, FadeAlpha));
}

void DrawGameUI()
{
	unsigned short Spacing  = 0;

	for (int i = 0; i < Player.Heart.size(); i++)
	{
		DrawTexture(Player.Heart[i], GetScreenWidth()-PANEL_WIDTH + 140 + Spacing, 436, WHITE);
		Spacing += HeartSpriteSpacing;
	}

	Spacing = 0;
	
	for (int i = 0; i < Player.Bomb.size(); i++)
	{
		DrawTexture(Player.Bomb[i], GetScreenWidth()-PANEL_WIDTH + 154 + Spacing, 487, WHITE);
		Spacing += 40;
	}

	Spacing = 0;

	unsigned int OutputScore = Highscore;

	for (int i = 0; i < 3; i++)
	{
		if (Highscore > 999999)
		{
			Highscore = 999999;
			OutputScore = Highscore;
		}

		RDrawTextEx(GetFont(VCR), FormatText("%06i", OutputScore), {float(GetScreenWidth()-PANEL_WIDTH+211), 223.0f + Spacing}, 31.0f, 0.0f, WHITE);

		// Grazing
		if (i > 0)
		{
			OutputScore = Player.GrazingScore;
			Spacing = GrazingScorePosition;
		}
		// Score
		else
		{
			OutputScore = Player.Score;
			Spacing = ScorePosition;
		}
	}
}

void DrawPauseScreen()
{
	DrawGame();

	DrawRectangle(0, 0, GetScreenWidth()-PANEL_WIDTH, GetScreenHeight(), Fade(BLACK, 0.7f));

	DrawTexture(GetAsset(PauseState), 0, 0, WHITE);

	// Stats panel
	DrawTexture(Panel, GetScreenWidth()-PANEL_WIDTH, 0, WHITE);
 	DrawTexture(GetAsset(Divider), GetScreenWidth()-PANEL_WIDTH-5, 0, WHITE); // Divider
	DrawFPS(GetScreenWidth()-80, GetScreenHeight() - 25);
	DrawGameUI();
}

void DrawDeathScreen()
{
	DrawTexture(GetAsset(DeathState), 0, 0, WHITE);

	// Highscore
	RDrawTextEx(GetFont(VCR), FormatText("%06i", Highscore), {284.0f, 223.0f}, 31.0f, 0.0f, WHITE);

	// Score
	RDrawTextEx(GetFont(VCR), FormatText("%06i", Player.Score), {284.0f, 325.0f}, 31.0f, 0.0f, WHITE);

	// Grazing
	RDrawTextEx(GetFont(VCR), FormatText("%06i", Player.GrazingScore), {284.0f, 430.0f}, 31.0f, 0.0f, WHITE);

	// Total
	RDrawTextEx(GetFont(VCR), FormatText("%06i", ScoreCounter), {266.0f, 580.0f}, 42.0f, 0.0f, WHITE);

	// Stats panel
	DrawTexture(Panel, GetScreenWidth()-PANEL_WIDTH, 0, WHITE);
 	DrawTexture(GetAsset(Divider), GetScreenWidth()-PANEL_WIDTH-5, 0, WHITE); // Divider
	DrawFPS(GetScreenWidth()-80, GetScreenHeight() - 25);
	DrawGameUI();
}

void DrawWinScreen()
{
	DrawTexture(GetAsset(WinState), 0, 0, WHITE);

	// Highscore
	RDrawTextEx(GetFont(VCR), FormatText("%06i", Highscore), {284.0f, 223.0f}, 31.0f, 0.0f, WHITE);

	// Score
	RDrawTextEx(GetFont(VCR), FormatText("%06i", Player.Score), {284.0f, 325.0f}, 31.0f, 0.0f, WHITE);

	// Grazing
	RDrawTextEx(GetFont(VCR), FormatText("%06i", Player.GrazingScore), {284.0f, 430.0f}, 31.0f, 0.0f, WHITE);

	// Total
	RDrawTextEx(GetFont(VCR), FormatText("%06i", ScoreCounter), {266.0f, 580.0f}, 42.0f, 0.0f, WHITE);

	// Stats panel
	DrawTexture(Panel, GetScreenWidth()-PANEL_WIDTH, 0, WHITE);
 	DrawTexture(GetAsset(Divider), GetScreenWidth()-PANEL_WIDTH-5, 0, WHITE); // Divider
	DrawFPS(GetScreenWidth()-80, GetScreenHeight() - 25);
	DrawGameUI();
}

void DrawStatsScreen()
{
	ClearBackground(WHITE);
}

void DrawHighscoreScreen()
{
	ClearBackground(WHITE);
}

void DrawTestScene()
{
	ClearBackground(BLACK);
	RDrawText("Testing Playground!", GetScreenWidth()-PANEL_WIDTH + 10, 20, 20, WHITE);

	DrawLine(700, 0, 700, 800, WHITE);

	//Pattern<BulletType>.Draw();
}

void DrawFlockingScene()
{
	ClearBackground(BLACK);
	DrawFPS(10, 40);
	RDrawText("Flocking Simulation!", 10, 10, 20, WHITE);

	DrawLine(700, 0, 700, 800, WHITE);
}

void LoadHighscore()
{
	std::ifstream LoadHighscore;
	LoadHighscore.open("Highscore.score", std::ios::in | std::ios::binary);

	if (LoadHighscore.fail())
	{
		std::cerr << "\nERROR: No highscore file found.\n";
		std::cout << "INFO: Creating new file...\n";

		LoadHighscore.open("Highscore.score", std::ios::in | std::ios::binary);

		std::cout << "INFO: File created successfully.\n";
	}

	LoadHighscore.seekg(0, std::ios::end);
	const int Size = int(LoadHighscore.tellg());
	LoadHighscore.seekg(0, std::ios::beg);

	while (LoadHighscore.tellg() < Size)
		LoadHighscore.read(reinterpret_cast<char*>(&Highscore), sizeof(Highscore));
	
	LoadHighscore.close();
}

void SaveHighscore()
{
	system("attrib +h +s -r Highscore.score");

	std::ofstream SaveHighscore;
	SaveHighscore.open("Highscore.score", std::ios::out | std::ios::binary | std::ios::app);

	if (SaveHighscore.fail())
	{
		std::cerr << "ERROR: Unable to save highscore.\n";
		exit(EXIT_FAILURE);
	}

	SaveHighscore.write(reinterpret_cast<char*>(&Highscore), sizeof(Highscore));
	SaveHighscore.close();

	system("attrib +h +s +r Highscore.score");
}

bool ConnectToServer()
{
	std::string IP = "199.192.24.86"; // Address of server
	const int Port = 54000; // Listening port number on server

	// Initialise Win socket
	Version = MAKEWORD(2, 2);
	const int WSResult = WSAStartup(Version, &Data);
	if (WSResult != 0)
	{
		std::cerr << "Can't start Win-sock, Error# " << WSResult << ".\n";
		exit(EXIT_FAILURE);
	}

	// Create socket
	Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Socket == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Error# " << WSAGetLastError() << "\n";
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	// Fill in hint structure
	Hint.sin_family = AF_INET;
	Hint.sin_port = htons(Port); // htons "Host to network short"
	inet_pton(AF_INET, IP.c_str(), &Hint.sin_addr);

	// Connect to server
	const int ConnectionResult = connect(Socket, reinterpret_cast<sockaddr*>(&Hint), sizeof(Hint));
	if (ConnectionResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Error# " << WSAGetLastError() << "\n";
		closesocket(Socket);
		WSACleanup();
		return false;
	}

	return true;
}

void SendData()
{
	DataToSend = std::to_string(Highscore);

	send(Socket, Player.Name.c_str(), int(Player.Name.length()), 0);
	send(Socket, DataToSend.c_str(), int(DataToSend.length()) + 1, 0);
}

void Unload()
{
	ASSETS.UnloadSprites();
	ASSETS.UnloadFonts();
	ASSETS.UnloadSounds();
	ASSETS.UnloadMusic();
}
