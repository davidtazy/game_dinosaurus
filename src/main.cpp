#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>

#include <chrono>
#include <iostream>

#include "dino.h"
#include "ground.h"

std::chrono::milliseconds get_now(std::chrono::high_resolution_clock::time_point start) {
  auto now = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
}

int main() {
  auto start = std::chrono::high_resolution_clock::now();

  std::string resource_dir = RESOURCES_DIR;

  sf::RenderWindow window(sf::VideoMode(800, 600), "Dinausaurus");
  window.setFramerateLimit(60);

  Ground ground(resource_dir + "/ground.png");
  Dino dino(resource_dir + "/dino");

  dino.on_timer(get_now(start));

  // run the program as long as the window is open
  while (window.isOpen()) {
    bool key_jump = false;

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
          break;
        }
        case sf::Event::KeyPressed:
          key_jump = true;
          break;

        default:
          break;
      }
    }

    auto now = get_now(start);

    dino.on_timer(now);

    if (key_jump)
      std::cerr << "jummppp" << std::endl;
    else {
      std::cerr << std::endl;
    }

    window.clear();

    ground.draw(window, 5);
    dino.draw(window, 0);

    window.display();
  }

  return EXIT_SUCCESS;
}