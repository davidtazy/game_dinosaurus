#include "cactus.h"

CactusFactory::CactusFactory(std::string filename, int nb_col, int nb_row)
    : _anim(filename, nb_col, nb_row) {
  //
}

Cactus CactusFactory::create() {
  return Cactus(_anim.at(2));
}

Cactus::Cactus(sf::Texture& texture) {
  _rectangle.setTexture(&texture);
  _rectangle.setSize(sf::Vector2f{100, 150});
}

void Cactus::resize(int width, int height) {
  _rectangle.setPosition(sf::Vector2f{width - 50, height - _rectangle.getSize().y - _height});
}

void Cactus::draw(sf::RenderTarget& render, int speed) {
  _rectangle.move(-speed, 0);
  if (_rectangle.getPosition().x + _rectangle.getSize().x < 0) {
    auto [width, height] = render.getSize();
    _rectangle.setPosition(sf::Vector2f{2 * width, height - _rectangle.getSize().y - _height});
  }
  render.draw(_rectangle);
}

void Cactus::on_timer(std::chrono::milliseconds now) {
  // move
}

void CactusFactory::untested(){};