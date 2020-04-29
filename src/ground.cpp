#include "ground.h"
#include <filesystem>
#include <iostream>
#include <sstream>

Ground::Ground(const std::string& texture_filepath, int height) : _height(height) {
  if (!std::filesystem::exists(std::filesystem::path{texture_filepath})) {
    std::ostringstream error;
    error << "Ground::Ground: texture at " << texture_filepath << " does not exists";
    throw std::runtime_error(error.str());
  }

  if (!_texture.loadFromFile(texture_filepath)) {
    throw std::runtime_error("Ground::Ground: texture.loadFromFile failed");
  }

  //_texture.setSmooth(true);
  _texture.setRepeated(true);

  _sprite.setTexture(_texture);
}

void Ground::resize(int width, int height) {
  _sprite.setTextureRect(sf::IntRect{0, 0, 2 * width, _height});
  _sprite.setPosition(sf::Vector2f(0, height - _height));  // absolute position
}

void Ground::draw(sf::RenderTarget& render, int speed) {
  if (_sprite.getPosition().x + render.getSize().x < 0) {
    _sprite.setPosition(sf::Vector2f(0, render.getSize().y - _height));
  }

  _sprite.move(0 - speed, 0);

  render.draw(_sprite);
}