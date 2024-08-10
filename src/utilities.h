#pragma once
#include <iostream>
#include "game.h"
#include "renderer.h"
#include "controller.h"
#include <sstream>
#include <fstream>

class Utilities
{
public:
	Utilities(Game& game, Renderer& renderer);
	~Utilities();
	void storeGameResult();
private:
	int getGameSpeed();
	void setGameSpeed();
	void setGameSpeed(int speed);
	int _gameSpeed{};
	Game& _game;
	Renderer& _renderer;
};