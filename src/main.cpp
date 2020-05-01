
#include <iostream>

#include "game.h"
#include "replay.h"
int main() {
  std::string resource_dir = RESOURCES_DIR;
  Game game(resource_dir);
  Record record("record.gm");

  auto window = game.createWindow(800, 600, "title");
  window.setFramerateLimit(30);

  std::chrono::milliseconds now{};

  for (auto event = game.read_events(window); event.type != sf::Event::Closed;
       event = game.read_events(window)) {
    record.push({now, event});
    game.loop(window, event, now);
    window.display();

    using namespace std::chrono_literals;
    now += 20ms;
  }
  window.close();
  return 0;
}