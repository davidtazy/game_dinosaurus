#include <catch2/catch.hpp>
#include "cactus.h"
#include "dino.h"
#include "game.h"
#include "ground.h"
TEST_CASE("untested objects -for accurate coverage report") {
  std::string resource_dir = RESOURCES_DIR;
  std::string dino_resources = resource_dir + "/dino";

  CactusFactory::untested();
  Dino dino(dino_resources);
  Ground ground(resource_dir + "/ground.png");

  Game game("dsdsqd");
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