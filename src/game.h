#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <chrono>
#include <string>

#include "cactus.h"
#include "dino.h"
#include "ground.h"
class Game {
  std::string resource_dir;
  int speed = 0;

  Ground ground;
  Dino dino;
  CactusFactory cactus_factory;
  std::vector<Cactus> cactus;

 public:
  explicit Game(std::string resource_dir);

  sf::RenderWindow createWindow(int width, int height, std::string title);

  sf::Event read_events(sf::Window& window_p);

  int loop(sf::RenderTarget& window, sf::Event events, std::chrono::milliseconds now_p);
};
