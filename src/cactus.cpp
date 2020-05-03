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
  // _rectangle.setOutlineThickness(3);
}

void Cactus::resize(int width, int height) {
  _rectangle.setPosition(sf::Vector2f{width - 50., height - _rectangle.getSize().y - _height});
}

void Cactus::draw(sf::RenderTarget& render, int speed) {
  _rectangle.move(-speed, 0);
  if (_rectangle.getPosition().x + _rectangle.getSize().x < 0) {
    auto [width, height] = render.getSize();
    _rectangle.setPosition(sf::Vector2f{2 * width, height - _rectangle.getSize().y - _height});
  }
  render.draw(_rectangle);

  if (0) {
    sf::Transform t;
    t.translate(0, 50);

    sf::RectangleShape col;
    col.setSize(_rectangle.getSize());
    col.setPosition(_rectangle.getPosition());
    col.setOutlineThickness(5);
    col.setOutlineColor(sf::Color::Blue);

    render.draw(col, t);
  }
}

void Cactus::on_timer(std::chrono::milliseconds now) {
  // move
}

sf::FloatRect Cactus::collision_rect() const {
  sf::Transform t;
  t.translate(0, 50);

  sf::FloatRect r(_rectangle.getPosition(), _rectangle.getSize());
  return t.transformRect(r);
}

void CactusFactory::untested(){};