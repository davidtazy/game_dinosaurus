#include "game.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>

#include <chrono>
#include <iostream>

#include "cactus.h"
#include "dino.h"
#include "ground.h"
std::chrono::milliseconds get_now(std::chrono::high_resolution_clock::time_point start) {
  auto now = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
}

Game::Game(std::string resource_dir) : resource_dir(resource_dir) {
  std::cout << "start game" << std::endl;
}

int Game::loop() const {
  auto start = std::chrono::high_resolution_clock::now();
  int speed = 0;

  sf::RenderWindow window(sf::VideoMode(800, 600), "Dinausaurus");
  window.setFramerateLimit(60);

  Ground ground(resource_dir + "/ground.png", 120);
  Dino dino(resource_dir + "/dino");
  CactusFactory cactus_factory(resource_dir + "/cactus.png", 9, 1);
  Cactus cactus = cactus_factory.create();

  dino.on_start_running([&speed]() { speed = 5; });

  // run the program as long as the window is open
  while (window.isOpen()) {
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::Resized: {
          sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
          window.setView(sf::View(visibleArea));

          auto [width, height] = window.getSize();
          ground.resize(width, height);
          dino.resize(width, height);
          cactus.resize(width, height);
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
    }

    auto now = get_now(start);

    cactus.on_timer(now);
    dino.on_timer(now);

    window.clear();

    ground.draw(window, speed);
    cactus.draw(window, speed);
    dino.draw(window, 0);

    window.display();
  }

  return EXIT_SUCCESS;
}
