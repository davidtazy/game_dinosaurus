#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <string>
#include "animation.h"

struct Cactus {
  void resize(int width, int height);
  void on_timer(std::chrono::milliseconds now);
  void draw(sf::RenderTarget& render, int speed);

  sf::RectangleShape _rectangle;
  int _height{100};

  Cactus(sf::Texture& texture);
};

struct CactusFactory {
  explicit CactusFactory(std::string filename, int nb_col, int nb_row);

  Cactus create();

  Animation _anim;

  static void untested();
};
