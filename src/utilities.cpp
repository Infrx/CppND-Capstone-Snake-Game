#include "utilities.h"

Utilities::Utilities(Game& game, Renderer& renderer)
	: _game(game), _renderer(renderer)
{
	getGameSpeed();
	setGameSpeed();
}

Utilities::~Utilities()
{
	storeGameResult();
}


void Utilities::storeGameResult()
{
	std::ofstream file;
	file.open("\src\gameresults.txt", std::ios::app); // Open file in append mode

	if (file.is_open()) 
	{
		file << "size:" << _game.GetSize() << ", score:" << _game.GetScore() << std::endl; 
		file.close();
		std::cout << "Game results are stored succesfully!\n";
	}
	else 
	{
		std::cerr << "Unable to open file." << std::endl;
	}
}




int Utilities::getGameSpeed()
{
	
	while (true)
	{
		std::cout << "Please enter game speed as integer between 1 and 3, 2\n";
		std::cout << "1 for normal speed\n";
		std::cout << "2 for double speed\n";
		std::cout << "3 for triple speed\n";
		std::cin >> _gameSpeed;
		if (std::cin.fail())
		{
			std::cin.clear(); // Clear the error flag
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
			std::cout << "Invalid input. Please enter a valid integer.\n";
		}
		else if (_gameSpeed < 1 || _gameSpeed > 3)
			std::cout << "Input out of range. Please try again.\n";
		else
			break; // Valid integer within range, exit the loop
	}
	return _gameSpeed;
}

void Utilities::setGameSpeed()
{
	_game._gameSpeed = _gameSpeed;
	std::cout << "Game speed successfully set!\n";

}

void Utilities::setGameSpeed(int speed)
{
	_game._gameSpeed = speed;
}
