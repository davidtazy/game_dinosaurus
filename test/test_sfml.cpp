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

TEST_CASE("dino state machine") {
  DinoState state;

  SECTION("initial state is Pause") {
    REQUIRE(state.is_pause());
    REQUIRE_FALSE(state.is_died());
    REQUIRE_FALSE(state.is_jumping());
    REQUIRE_FALSE(state.is_running());
    REQUIRE_FALSE(state.is_walking());
  }

  SECTION("Pause ignore all event except StartEvent") {
    state.on_collision();
    REQUIRE(state.is_pause());
    state.on_jump();
    REQUIRE(state.is_pause());
    state.on_pause();
    REQUIRE(state.is_pause());
    state.on_timer(std::chrono::milliseconds::zero());
    REQUIRE(state.is_pause());

    state.on_play();
    REQUIRE_FALSE(state.is_pause());

    REQUIRE(state.is_walking());
  }

  state.on_play();
  state.on_timer(std::chrono::milliseconds{0});

  SECTION("dino switch from walk to run after 100% animation") {
    REQUIRE(state.is_walking());
    std::chrono::milliseconds elapsed{0};
    int percent = 0;
    while (state.is_walking()) {
      elapsed += std::chrono::milliseconds{50};
      REQUIRE(percent < state.animation_percent(elapsed));
      percent = state.animation_percent(elapsed);
      // std::cerr << "percent is " << percent << std::endl;
      state.on_timer(elapsed);
    }

    REQUIRE(state.is_running());
  }

  state.on_timer(std::chrono::milliseconds{1000});

  SECTION("dino die on collision") {
    REQUIRE(state.is_running());
    state.on_collision();
    REQUIRE(state.is_died());
  }

  SECTION("can jump in running state") {
    REQUIRE(state.is_running());

    state.on_jump();
    REQUIRE(state.is_jumping());
    SECTION("dino back to run after 100% animation") {
      std::chrono::milliseconds elapsed{0};
      int count = 0;
      while (state.is_jumping()) {
        elapsed += std::chrono::milliseconds{50};
        state.on_timer(elapsed);
        count++;
      }
      REQUIRE(count > 18);

      REQUIRE(state.is_running());
    }
    SECTION("dino die on collision") {
      REQUIRE(state.is_jumping());
      state.on_collision();
      REQUIRE(state.is_died());
    }
  }
}

TEST_CASE("load dino animations") {
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