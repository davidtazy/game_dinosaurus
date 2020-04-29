#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <string>
#include <vector>
class Dino {
 public:
  Dino(const std::string& texture_dir, int height = 100);
  void draw(sf::RenderTarget& render, int speed);
  void resize(int width, int height);

 private:
  std::vector<sf::Texture> _texture;
  std::vector<sf::Texture>::iterator it;
  sf::Sprite _sprite;
  sf::Clock clock;
  int _height{100};
};