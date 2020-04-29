

#include <filesystem>
#include <iostream>
#include <sstream>

#include "dino.h"

Dino::Dino(const std::string& texture_dir, int height)
    : _anim_idle{texture_dir, "Idle"},
      _anim_walk{texture_dir, "Walk"},
      _anim_run{texture_dir, "Run"},
      _anim_jump{texture_dir, "Jump"},
      _anim_die{texture_dir, "Dead"},
      _height(height) {
  for (auto& anim : {&_anim_idle, &_anim_walk, &_anim_run, &_anim_jump, &_anim_die}) {
    anim->set_duration(std::chrono::milliseconds(1000)).set_sprite(_sprite);
  }
  _anim_die.set_repeated(false);
  _anim_jump.set_repeated(false);
  _anim_walk.set_repeated(false);

  _anim = nullptr;
}

void Dino::resize(int width, int height) {
  int h = _sprite.getTextureRect().height;
  //_sprite.setOrigin(0, h / 3);

  // _sprite.setTextureRect(sf::IntRect{0, 0, 2 * width, _height});

  _sprite.setPosition(sf::Vector2f(width / 4, height - h));
}

void Dino::draw(sf::RenderTarget& render, int speed) {
  //_sprite.setScale(0.5, 0.5);

  render.draw(_sprite);
}

void Dino::on_timer(std::chrono::milliseconds now) {
  if (_anim == nullptr) {
    _anim = &_anim_idle;
    _anim->start(now);
  }
  _anim->update(now);
  // std::cerr << "now is " << now.count() << " index is " << _anim->texture_index(now);
}