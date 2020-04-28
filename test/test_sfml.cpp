#include <SFML/Graphics.hpp>
#include <catch2/catch.hpp>

TEST_CASE("sfml is up") {
  sf::Clock clock;
  clock.getElapsedTime().asSeconds();

  sf::VideoMode videoMode(720, 480);

  sf::RectangleShape shape;
  shape.setFillColor(sf::Color::Cyan);

  REQUIRE(clock.getElapsedTime().asMicroseconds() > 0);
}