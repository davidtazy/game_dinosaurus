#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>

#include <array>
#include <chrono>
#include <string>
#include <vector>

#include "animation.h"
#include "dino_state.h"

struct JumpPosAnimation : public SubAnimation {
  sf::Shape& shape;
  sf::Vector2f init_pos{-1, -1};
  JumpPosAnimation(sf::Shape& shape) : shape(shape) {}

  void on_start() override { init_pos = shape.getPosition(); }

  void update(int percent) override;
};

class Dino {
 public:
  Dino(const std::string& texture_dir, int height = 100);
  void draw(sf::RenderTarget& render, int speed);
  void resize(int width, int height);

  void on_start_running(std::function<void()> callback) { _on_start_running_callback = callback; }

  void on_timer(std::chrono::milliseconds elapsed);
  void on_play();
  void on_pause();
  void on_jump();

  const DinoState& get_state() const { return state; }

 private:
  Animation _anim_idle;
  Animation _anim_walk;
  Animation _anim_run;
  Animation _anim_jump;
  Animation _anim_die;
  Animation *_anim{}, *_next_anim{};
  sf::RectangleShape _rectangle;
  int _height{100};
  std::function<void()> _on_start_running_callback;
  DinoState state;
  JumpPosAnimation _jump_pos_animation;
};