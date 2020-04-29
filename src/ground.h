#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string>

class Ground {
 public:
  Ground(const std::string& texture_filepath, int height = 100);
  void draw(sf::RenderTarget& render, int speed);
  void resize(int width, int height);

 private:
  sf::Texture _texture;
  sf::Sprite _sprite;
  int _height{100};
};