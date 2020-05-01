#pragma once
#include <string>

class Game {
  std::string resource_dir;

 public:
  explicit Game(std::string resource_dir);

  int loop() const;
};