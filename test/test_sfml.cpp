#include <SFML/Graphics.hpp>
#include <catch2/catch.hpp>
#include <string>

#include "animation.h"

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

#include "dino_state.h"

/************************************************
 *               Animations
 * *********************************************/

TEST_CASE("load  animations in dir") {
  std::string resource_dir = RESOURCES_DIR;
  std::string dino_resources = resource_dir + "/dino";

  Animation animation{dino_resources, "Idle"};

  SECTION("get files in alpha order") {
    auto files = Animation::find_files(dino_resources, "Idle");
    REQUIRE(files.size() == 10);
    for (auto file : files) {
      // std::cerr << file << std::endl;
    }
  }

  SECTION("load all textures") { Animation anim(dino_resources, "Idle"); }

  SECTION("remap numbers") { REQUIRE(Animation::remap(50, 0, 100, 50, 100) == 75); }

  SECTION("index texture always return a valid index") {
    Animation anim(dino_resources, "Idle");
    anim.set_repeated(true).set_duration(std::chrono::milliseconds{1000});

    std::chrono::milliseconds now{0};

    anim.start(now);
    using namespace std::chrono_literals;
    for (int i = 0; i < 20; i++) {
      // std::cerr << anim.texture_index(now) << std::endl;
      REQUIRE(anim.texture_index(now) == i % 10);

      now += 100ms;
    }

    std::cerr << anim.texture_index(now) << std::endl;
  }
}

TEST_CASE("load animation from file ") {
  std::string resource_dir = RESOURCES_DIR;

  Animation anim(resource_dir + "/cactus.png", 9, 1);

  REQUIRE(anim.nb_frame() == 9);
}