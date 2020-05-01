
#include <SFML/Graphics/RenderTexture.hpp>

#include <catch2/catch.hpp>
#include "cactus.h"
#include "dino.h"
#include "game.h"
#include "ground.h"
#include "replay.h"

TEST_CASE("untested objects -for accurate coverage report") {
  std::string resource_dir = RESOURCES_DIR;
  std::string dino_resources = resource_dir + "/dino";

  CactusFactory::untested();
  Dino dino(dino_resources);
  Ground ground(resource_dir + "/ground.png");
}

TEST_CASE("game e2e render in texture") {
  std::string test_dir = UNITEST_DIR;
  std::string resource_dir = RESOURCES_DIR;
  Game game(resource_dir);

  // auto window = game.createWindow(800, 600, "title");
  auto window = sf::RenderTexture{};
  REQUIRE(window.create(800, 600) == true);
  // window.setFramerateLimit(30);

  Replay replay(test_dir + "/record.gm");

  std::chrono::milliseconds now{};

  while (replay.can_pull()) {
    auto sample = replay.pull();
    game.loop(window, sample.event, sample.timestamp);
    window.display();
  }
  // window.close();
}

TEST_CASE("game e2e replay", "[.integration]") {
  std::string test_dir = UNITEST_DIR;
  std::string resource_dir = RESOURCES_DIR;
  Game game(resource_dir);

  auto window = game.createWindow(800, 600, "title");
  // window.setFramerateLimit(30);

  Replay replay(test_dir + "/record.gm");

  std::chrono::milliseconds now{};

  while (replay.can_pull()) {
    auto sample = replay.pull();
    game.loop(window, sample.event, sample.timestamp);
    window.display();
  }
  window.close();
}

#include <sstream>
#include "replay.h"
TEST_CASE("record/replay") {
  std::stringstream stream;
  Record rec(stream);
  auto ts1 = std::chrono::milliseconds(123);
  auto ts2 = std::chrono::milliseconds(456);
  sf::Event e1, e2;
  e1.type = sf::Event::Resized;
  e1.size.height = 600;
  e1.size.width = 800;

  e2.type = sf::Event::EventType::GainedFocus;

  rec.push({ts1, e1});
  rec.push({ts2, e2});

  Replay rep(stream);
  REQUIRE(rep.can_pull() == true);
  auto st = rep.pull();
  REQUIRE(st.timestamp == ts1);
  REQUIRE(st.event.type == e1.type);
  st = rep.pull();
  REQUIRE(st.timestamp == ts2);
  REQUIRE(st.event.type == e2.type);

  REQUIRE(rep.can_pull() == false);
}

/*********************************************
 *************** State machine ***************
 ********************************************/

TEST_CASE("dino state machine callback") {
  DinoState state;
  int evt_counter{};
  state.on_new_state([&evt_counter]() { evt_counter++; });
  REQUIRE(evt_counter == 0);
  state.on_play();
  REQUIRE(evt_counter == 1);
}

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
    state.on_animation_finished();
    REQUIRE(state.is_pause());

    state.on_play();
    REQUIRE_FALSE(state.is_pause());

    REQUIRE(state.is_walking());

    SECTION("dino switch from walk to run after  animation finished") {
      REQUIRE(state.is_walking());

      state.on_animation_finished();

      REQUIRE(state.is_running());

      SECTION("dino die on collision") {
        REQUIRE(state.is_running());
        state.on_collision();
        REQUIRE(state.is_died());
      }

      SECTION("can jump in running state") {
        REQUIRE(state.is_running());

        state.on_jump();
        REQUIRE(state.is_jumping());

        SECTION("dino die on collision") {
          REQUIRE(state.is_jumping());
          state.on_collision();
          REQUIRE(state.is_died());
        }

        SECTION("back to Running when animation finished") {
          REQUIRE(state.is_jumping());
          state.on_animation_finished();
          REQUIRE(state.is_running());
        }
      }
    }
  }
}