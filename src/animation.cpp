#include <filesystem>

#include <sstream>

#include "animation.h"

Animation::Animation(std::string texture_dir, std::string prefix) {
  std::ostringstream error;
  if (!std::filesystem::exists(std::filesystem::path{texture_dir})) {
    error << "Dino::Dino: texture dir at " << texture_dir << " does not exists";
    throw std::runtime_error(error.str());
  }

  auto files = find_files(texture_dir, prefix);

  for (auto& file : files) {
    _texture.push_back(sf::Texture{});
    if (!_texture.back().loadFromFile(file)) {
      error << "Animation::Animation: texture  at " << file << " load fail";
      throw std::runtime_error(error.str());
    }
  }
}

std::set<std::string> Animation::find_files(std::string texture_dir, std::string prefix) {
  std::set<std::string> files;  // dir iterator is not known
  for (auto& p : std::filesystem::directory_iterator(std::filesystem::path{texture_dir})) {
    std::string file = p.path().generic_string();
    if (p.path().filename().generic_string().find(prefix) == 0) {
      files.insert(file);
    }
  }
  return files;
}

int Animation::texture_index(std::chrono::milliseconds elapsed_p) const {
  auto now = elapsed_p;
  if (repeat) {
    now = (now - start_point) % duration;
  }

  int tex_index = remap(now.count(), start_point.count(), (start_point + duration).count(), 0,
                        _texture.size(), true);

  return tex_index;
}

void Animation::update(std::chrono::milliseconds now_p) {
  int index = texture_index(now_p);

  if (sprite) {
    sprite->setTexture(_texture.at(index));
  }
}

int Animation::remap(int value, int start1, int stop1, int start2, int stop2, bool withinBounds) {
  auto newval =
      static_cast<int>(((stop2 - start2) * (value - start1) / static_cast<int>(stop1 - start1))) +
      start2;

  if (!withinBounds) {
    return newval;
  }
  if (start2 < stop2) {
    return constrain(newval, start2, stop2);
  } else {
    return constrain(newval, stop2, start2);
  }
  return newval;
}