#include <experimental/filesystem>

#include <sstream>

#include "animation.h"

namespace stdfs = std::experimental::filesystem;

void throw_if_not_exists(std::string file, std::string msg) {
  if (!stdfs::exists(stdfs::path{file})) {
    throw std::runtime_error(msg);
  }
}

Animation::Animation(std::string texture_file, int nb_col, int nb_row) {
  std::ostringstream error;
  error << "Dino::Dino: texture file at " << texture_file << " does not exists";
  throw_if_not_exists(texture_file, error.str());

  if (!image.loadFromFile(texture_file)) {
    error << "Animation::Animation: image  at " << texture_file << " load fail";
    throw std::runtime_error(error.str());
  }

  auto [width, height] = image.getSize();
  auto w = width / nb_col;
  auto h = height / nb_row;

  for (int col = 0; col < nb_col; col++) {
    for (int row = 0; row < nb_row; row++) {
      _texture.push_back(sf::Texture{});
      if (!_texture.back().loadFromImage(image, sf::IntRect{col * w, row * h, w, h})) {
        error << "Animation::Animation: texture  from image at " << col << " " << row
              << " load fail";
        throw std::runtime_error(error.str());
      }
    }
  }
}

Animation::Animation(std::string texture_dir, std::string prefix) : name{prefix} {
  std::ostringstream error;
  error << "Dino::Dino: texture dir at " << texture_dir << " does not exists";
  throw_if_not_exists(texture_dir, error.str());

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
  for (auto& p : stdfs::directory_iterator(stdfs::path{texture_dir})) {
    std::string file = p.path().generic_string();
    if (p.path().filename().generic_string().find(prefix) == 0) {
      files.insert(file);
    }
  }
  return files;
}

int Animation::texture_index(std::chrono::milliseconds elapsed_p) const {
  auto now = elapsed_p;

  now = (now - start_point) % duration;

  int tex_index = remap(now.count(), 0, duration.count(), 0, _texture.size(), true);
  if (tex_index == _texture.size())
    tex_index--;

  return tex_index;
}

int Animation::frame_index_to_percent(int index) const {
  return remap(index, 0, _texture.size(), 0, 99, true);
}

void Animation::update(std::chrono::milliseconds now_p) {
  int index = texture_index(now_p);

  if (sprite) {
    sprite->setTexture(_texture.at(index));
  }
  if (shape) {
    shape->setTexture(&_texture.at(index));
  }

  if (_sub_anim)
    _sub_anim->update(frame_index_to_percent(index));
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