

#include <cmath>
#include <iostream>
#include <sstream>

#include "dino.h"

Dino::Dino(const std::string& texture_dir, int height)
    : _anim_idle{texture_dir, "Idle"},
      _anim_walk{texture_dir, "Walk"},
      _anim_run{texture_dir, "Run"},
      _anim_jump{texture_dir, "Jump"},
      _anim_die{texture_dir, "Dead"},
      _height(height),
      _jump_pos_animation(_rectangle) {
  for (auto& anim : {&_anim_idle, &_anim_walk, &_anim_run, &_anim_jump, &_anim_die}) {
    anim->set_duration(std::chrono::milliseconds(2500)).set_shape(_rectangle);
  }

  _anim_die.set_repeated(false);
  _anim_jump.set_repeated(false).set_sub_animation(_jump_pos_animation);
  _anim_walk.set_repeated(false);

  _next_anim = &_anim_idle;
  _anim = nullptr;

  state.on_new_state([&]() {
    _next_anim = nullptr;
    if (state.is_walking())
      _next_anim = &_anim_walk;
    else if (state.is_running()) {
      if (_on_start_running_callback) {
        _on_start_running_callback();
        _on_start_running_callback = nullptr;
      }
      _next_anim = &_anim_run;
    } else if (state.is_jumping())
      _next_anim = &_anim_jump;
    else if (state.is_pause())
      _next_anim = &_anim_idle;
    else if (state.is_died())
      _next_anim = &_anim_die;
    else {
      throw std::runtime_error("dino new state, unhandled unkow state ");
    }

    if (_anim != _next_anim) {
      std::cerr << "next anim is " << _next_anim->get_name() << std::endl;
      _anim = nullptr;
    }
  });
}

void Dino::resize(int width, int height) {
  _rectangle.setSize(sf::Vector2f{200, static_cast<float>(2 * _height)});
  _rectangle.setPosition(sf::Vector2f(0, height - 3 * _height));
}

void Dino::draw(sf::RenderTarget& render, int speed) {
  render.draw(_rectangle);
}

void Dino::on_timer(std::chrono::milliseconds now) {
  // load new animation
  if (_anim == nullptr) {
    _anim = _next_anim;
    _anim->start(now);
  }
  _anim->update(now);

  if (_anim->is_finished(now)) {
    state.on_animation_finished();  // ! state may change
    return;
  }
  // std::cerr << _anim->get_name() << " now is " << now.count() << " index is "
  //           << _anim->texture_index(now) << "/" << _anim->nb_frame() << std::endl;
}
void Dino::on_play() {
  state.on_play();
}
void Dino::on_pause() {
  state.on_pause();
}
void Dino::on_jump() {
  state.on_jump();
}

void JumpPosAnimation::update(int percent) {
  shape.setPosition(init_pos.x, init_pos.y + ::sin(-3.14 * percent / 100.) * 150);
}