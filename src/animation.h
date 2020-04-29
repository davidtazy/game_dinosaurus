#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <set>
struct Animation {
  Animation(std::string texture_dir, std::string prefix);

  static std::set<std::string> find_files(std::string texture_dir, std::string prefix);

  Animation& set_sprite(sf::Sprite& sprite_p) {
    sprite = &sprite_p;
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

  void start(std::chrono::milliseconds now_p) { start_point = now_p; }

  //!
  //!\return index of current texture
  int texture_index(std::chrono::milliseconds elapsed_p) const;

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

 private:
  std::vector<sf::Texture> _texture;

  sf::Sprite* sprite{};
  bool repeat{true};
  std::chrono::milliseconds duration{1000};
  std::chrono::milliseconds start_point{std::chrono::milliseconds::max()};
};