#pragma once
#include <vector>
#include "Pilot.h"
class Game
{
public:
	enum GameState
	{
		GAME_ACTIVE,
		GAME_MENU,
		GAME_WIN
	};
	//GameState
	GameState State;
	bool Keys[1024];
	glm::vec2 MousePos;
	bool MouseButtons[7];
	unsigned int Width, Height;

	Game(unsigned int width, unsigned int height);
	~Game();

	//Initialize the game state (load all shaders/textures/levels)
	void Init();

	//game loop
	void ProcessInput(float dt);
	void Update(float dt);
	void Render(float dt);

	//update loop
	void DoCollisions();
};

