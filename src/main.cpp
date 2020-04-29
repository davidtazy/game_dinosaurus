#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "dino.h"
#include "ground.h"

int main() {
  std::string resource_dir = RESOURCES_DIR;

  sf::RenderWindow window(sf::VideoMode(800, 600), "Dinausaurus");
  window.setFramerateLimit(60);

  Ground ground(resource_dir + "/ground.png");
  Dino dino(resource_dir + "/dino");

  // run the program as long as the window is open
  while (window.isOpen()) {
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (window.pollEvent(event)) {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed)
        window.close();
      if (event.type == sf::Event::Resized) {
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(visibleArea));

        auto [width, height] = window.getSize();
        ground.resize(width, height);
        dino.resize(width, height);
      }
    }

    window.clear();

    ground.draw(window, 5);
    dino.draw(window, 0);

    window.display();
  }

  return EXIT_SUCCESS;
}