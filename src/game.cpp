#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t screen_width, std::size_t screen_height)
    : snake(grid_width, grid_height), _gridWidth(grid_width), _gridHeight(grid_height), _screenWidth(screen_width), _screenHeight(screen_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  //PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  
  std::future<void> future = std::async(std::launch::async, &Game::GenerateObstacle, this, obstacles);
  future.wait();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  PlaceFood();
  
  int k{};
  while (running) 
  {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food);
    renderer.RenderBonusFood(snake, bonusFood);
    renderer.RenderObstacles(obstacles);
    

    frame_end = SDL_GetTicks();
   
   
    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    int c = 0;
 
    if(obstacles) // checks for obstacles before placing food
    {
        for (const auto& obs : *obstacles)
        {
            if (obs.x / 20 == x && obs.y / 20 == y)
                c++;
        }
    }
   
    if (!snake.SnakeCell(x, y) && c == 0)
    {
        food.x = x;
        food.y = y;
        return;
    }
    else
        PlaceFood();
  }
}

void Game::Update() 
{  
    SDL_Point cell;
    for(int i = 0; i < _gameSpeed; ++i)
    {
        if (!snake.alive) return;

        cell = snake.Update();
        snake.checkObstacle(obstacles, cell);

        int new_x = static_cast<int>(snake.head_x);
        int new_y = static_cast<int>(snake.head_y);

        // Check if there's food over here
        if (!is_bonus_food_active)
        { // Check if bonus food is already active
            PlaceBonusFood();
            is_bonus_food_active = true;
            bonusFoodThread = std::thread(&Game::BonusFoodTimer, this);
            bonusFoodThread.detach();
            already_appeared = true;
        }
        if (bonusFood.x == new_x && bonusFood.y == new_y) {
            score += 2;
            snake.GrowBody();
            snake.speed += 0.02;
            snake.defaultSpeed += 0.02;
            snake.boostedSpeed += 0.04;
            bonusFood.x = 1200;
            bonusFood.y = 1200;
            break;
        }
        if (food.x == new_x && food.y == new_y) {
            score++;
            PlaceFood();
            // Grow snake and increase speed.
            snake.GrowBody();
            snake.speed += 0.02;
            snake.defaultSpeed += 0.02;
            snake.boostedSpeed += 0.04;
        }
        
        
    }
}

void Game::GenerateObstacle(ObstaclePtr obstacles)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, static_cast<int>(_gridWidth - 1));
    std::uniform_int_distribution<> disY(0, static_cast<int>(_gridHeight - 1));
    int _tempX{};
    int _tempY{};
    int tempX{};
    int tempY{};
    // Simulate obstacle generation
    for (int i = 0; i < 10; ++i)
    {
        
        tempX = disX(gen);
        tempY = disY(gen);
        if (tempX != _tempX && tempY != _tempY) // checks for duplicate obstacles
        {
            Obstacle obs = { disX(gen) * _screenWidth / _gridWidth, disY(gen) * _screenWidth / _gridHeight };
            obstacles->push_back(obs);
        }
        else
            --i;
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Simulate delay
        _tempX = tempX;
        _tempY = tempY;
    }
}

void Game::PlaceBonusFood()
{
    int x, y;
    while (true) {
        x = random_w(engine);
        y = random_h(engine);
        // Check that the location is not occupied by a snake item before placing
        // food.
        int c = 0;

        if (obstacles) // checks for obstacles before placing food
        {
            for (const auto& obs : *obstacles)
            {
                if (obs.x / 20 == x && obs.y / 20 == y)
                    c++;
            }
        }

        if (!snake.SnakeCell(x, y) && c == 0)
        {
            bonusFood.x = x;
            bonusFood.y = y;
            return;
        }
        else
            PlaceBonusFood();
    }
}


int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }




void Game::BonusFoodTimer()
{
    const int bonusSeconds = 15;
    auto startTime = std::chrono::high_resolution_clock::now();
    std::unique_lock<std::mutex> lock(_mutex);
    while (is_bonus_food_active)
    {
        lock.unlock();
        auto current_Time = std::chrono::high_resolution_clock::now();
        auto elapsed_Seconds = std::chrono::duration_cast<std::chrono::seconds>(current_Time - startTime).count();
        int new_x = static_cast<int>(snake.head_x);
        int new_y = static_cast<int>(snake.head_y);
        if (elapsed_Seconds >= bonusSeconds)
        {
            // Bonus food time is up
            is_bonus_food_active = false;
            bonusFood.x = 1;
            bonusFood.y = 1;
            break;
        }
        lock.lock();
        // Wait for a short interval or until the condition_variable is notified
        _cond.wait_for(lock, std::chrono::milliseconds(800));
    }
}
