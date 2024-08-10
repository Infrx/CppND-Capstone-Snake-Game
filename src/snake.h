#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <memory>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "obstacle.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  SDL_Point Update();

  void GrowBody();
  bool SnakeCell(int x, int y);
  void checkObstacle(const std::shared_ptr<std::vector<Obstacle>>& obstacles, SDL_Point& current_head_cell);
  Direction direction = Direction::kUp;

  float speed{0.1f};
  float defaultSpeed{0.1f};
  float boostedSpeed{0.2f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif