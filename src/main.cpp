
#include <iostream>

#include "game.h"

int main() {
  std::string resource_dir = RESOURCES_DIR;

  Game game(resource_dir);

  return game.loop();
}