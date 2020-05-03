#include "game.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>

#include <algorithm>
#include <chrono>
#include <iostream>

// std::chrono::milliseconds get_now(std::chrono::high_resolution_clock::time_point start) {
//   auto now = std::chrono::high_resolution_clock::now();
//   return std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
// }

Game::Game(std::string resource_dir)
    : resource_dir(resource_dir),
      ground(resource_dir + "/ground.png", 120),
      dino(resource_dir + "/dino"),
      cactus_factory(resource_dir + "/cactus.png", 9, 1)

{
  std::cout << "start game" << std::endl;
  // auto start = std::chrono::high_resolution_clock::now();

  cactus.push_back(cactus_factory.create());

  dino.on_start_running([this]() { speed = 5; });
}

sf::RenderWindow Game::createWindow(int width, int height, std::string title) {
  return sf::RenderWindow(sf::VideoMode(width, height), title);
}

int Game::loop(sf::RenderTarget& window, sf::Event event, std::chrono::milliseconds now) {
  // run the program as long as the window is open

  switch (event.type) {
    case sf::Event::Resized: {
      sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
      window.setView(sf::View(visibleArea));

      auto [width, height] = window.getSize();
      ground.resize(width, height);
      dino.resize(width, height);
      for (auto& c : cactus)
        c.resize(width, height);
      break;
    }
    case sf::Event::KeyPressed:
      if (dino.get_state().is_pause()) {
        dino.on_play();
        speed = 2;
      } else
        dino.on_jump();
      break;

    default:
      break;
  }

  for (auto& c : cactus)
    c.on_timer(now);
  dino.on_timer(now);

  window.clear();

  ground.draw(window, speed);
  for (auto& c : cactus)
    c.draw(window, speed);
  dino.draw(window, 0);

  auto dino_bound = dino.collision_rect();
  if (std::any_of(cactus.begin(), cactus.end(),
                  [dino_bound](Cactus& c) { return dino_bound.intersects(c.collision_rect()); })) {
    // std::cerr << " collisionssssss\n";

    speed = 0;
    dino.on_collision();
  }

  return EXIT_SUCCESS;
}

sf::Event Game::read_events(sf::Window& window_p) {
  sf::Event event;
  window_p.pollEvent(event);
  return event;
}