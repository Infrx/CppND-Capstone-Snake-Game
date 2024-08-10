#ifndef GAME_H
#define GAME_H

#include <random>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <future>
#include <chrono>
#include <mutex>
#include <memory>
#include "obstacle.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, std::size_t screen_width, std::size_t screen_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  int _gameSpeed{ 1 };
 private:

  using ObstaclePtr = std::shared_ptr<std::vector<Obstacle>>;
  ObstaclePtr obstacles = std::make_shared<std::vector<Obstacle>>();
  std::mutex _mutex;

  Snake snake;
  SDL_Point food;
  int _gridWidth{};
  int _gridHeight{};
  int _screenWidth{};
  int _screenHeight{};
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update();
  void GenerateObstacle(ObstaclePtr obstacles);
};

#endif