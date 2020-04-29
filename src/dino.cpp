#include "dino.h"

#include <filesystem>
#include <iostream>
#include <sstream>

Dino::Dino(const std::string& texture_dir, int height) : _height(height) {
  std::ostringstream error;
  if (!std::filesystem::exists(std::filesystem::path{texture_dir})) {
    error << "Dino::Dino: texture dir at " << texture_dir << " does not exists";
    throw std::runtime_error(error.str());
  }

  for (auto& p : std::filesystem::directory_iterator(std::filesystem::path{texture_dir})) {
    _texture.push_back(sf::Texture{});
    if (!_texture.back().loadFromFile(p.path().generic_string())) {
      error << "Dino::Dino: texture  at " << texture_dir << " load fail";
      throw std::runtime_error(error.str());
    }
  }

  it = std::begin(_texture);

  //_texture.setSmooth(true);
  //_texture.setRepeated(false);
}

void Dino::resize(int width, int height) {
  // _sprite.setTextureRect(sf::IntRect{0, 0, 2 * width, _height});
  //_sprite.setPosition(sf::Vector2f(width / 2, height - 2 * _height));  // absolute position
  clock.restart();
}

void Dino::draw(sf::RenderTarget& render, int speed) {
  if (clock.getElapsedTime().asMilliseconds() > 200) {
    clock.restart();
    it++;
    if (it == std::end(_texture))
      it = std::begin(_texture);
  }

  _sprite.setTexture(*it);

  _sprite.setScale(0.5, 0.5);

  render.draw(_sprite);
}