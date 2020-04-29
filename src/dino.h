#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <array>
#include <chrono>
#include <string>
#include <vector>
#include "animation.h"
#include "dino_state.h"

class Dino {
 public:
  Dino(const std::string& texture_dir, int height = 100);
  void draw(sf::RenderTarget& render, int speed);
  void resize(int width, int height);

  void on_timer(std::chrono::milliseconds elapsed);
  void on_play();
  void on_pause();
  void on_jump();

 private:
  Animation _anim_idle;
  Animation _anim_walk;
  Animation _anim_run;
  Animation _anim_jump;
  Animation _anim_die;
  Animation* _anim;
  sf::Sprite _sprite;
  int _height{100};

  DinoState state;
};