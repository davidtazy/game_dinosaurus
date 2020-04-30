#pragma once

#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <chrono>
#include <set>

struct SubAnimation {
  virtual void on_start() = 0;
  virtual void update(int percent) = 0;
};

struct Animation {
  Animation(std::string texture_dir, std::string prefix);
  Animation(std::string texture_file, int nb_col, int nb_row);

  static std::set<std::string> find_files(std::string texture_dir, std::string prefix);

  void set_sub_animation(SubAnimation& sub_anim) { _sub_anim = &sub_anim; }

  Animation& set_sprite(sf::Sprite& sprite_p) {
    sprite = &sprite_p;
    return *this;
  }
  Animation& set_shape(sf::Shape& shape_p) {
    shape = &shape_p;
    return *this;
  }
  Animation& set_repeated(bool repeated = true) {
    repeat = repeated;
    return *this;
  }

  Animation& set_duration(std::chrono::milliseconds d) {
    duration = d;
    return *this;
  }

  void start(std::chrono::milliseconds now_p) {
    start_point = now_p;
    if (_sub_anim)
      _sub_anim->on_start();
  }

  //!
  //!\return index of current texture
  int texture_index(std::chrono::milliseconds elapsed_p) const;
  int frame_index_to_percent(int index) const;

  void update(std::chrono::milliseconds now_p);

  bool is_finished(std::chrono::milliseconds elapsed_p) const {
    if (repeat)
      return false;  // never finish
    return (elapsed_p - start_point) > duration;
  }

  static int constrain(int value, int low, int high) {
    return std::max(std::min(value, high), low);
  }

  // Re-maps a number from one range to another.
  static int remap(int value,
                   int start1,
                   int stop1,
                   int start2,
                   int stop2,
                   bool withinBounds = false);

  std::string get_name() const { return name; }
  int nb_frame() const { return _texture.size(); }

  sf::Texture& at(int index) { return _texture[index]; }

 private:
  std::vector<sf::Texture> _texture;

  sf::Sprite* sprite{};
  sf::Shape* shape{};
  bool repeat{true};
  sf::Image image;
  std::chrono::milliseconds duration{1000};
  std::chrono::milliseconds start_point{std::chrono::milliseconds::max()};
  std::string name;
  SubAnimation* _sub_anim{};
};