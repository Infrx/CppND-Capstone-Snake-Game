#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "snake.h"
#include <memory>
#include "obstacle.h"



class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const snake, SDL_Point const &food);
  void UpdateWindowTitle(int score, int fps);

  void RenderObstacles(const std::shared_ptr<std::vector<Obstacle>>& obstacles);
  void RenderBonusFood(Snake const snake, SDL_Point const& food);

 //private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  
  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif