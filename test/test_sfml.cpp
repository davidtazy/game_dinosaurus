#include <SFML/Graphics.hpp>
#include <catch2/catch.hpp>
#include <string>

TEST_CASE("sfml is up") {
  sf::Clock clock;
  clock.getElapsedTime().asSeconds();

  sf::VideoMode videoMode(720, 480);

  sf::RectangleShape shape;
  shape.setFillColor(sf::Color::Cyan);

  REQUIRE(clock.getElapsedTime().asMicroseconds() > 0);
}

TEST_CASE("sfml can load file") {
  sf::Texture tx;
  std::string resource_dir = RESOURCES_DIR;
  REQUIRE(tx.loadFromFile(resource_dir + "/ground.png"));

  tx.setSmooth(true);
  tx.setRepeated(true);

  sf::Sprite sprite;
  sprite.setTexture(tx);
}