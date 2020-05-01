#pragma once
#include <SFML/Window/Event.hpp>
#include <chrono>
#include <cstring>
#include <iostream>

struct GameState {
  std::chrono::milliseconds timestamp;
  sf::Event event;
};

std::ostream& operator<<(std::ostream& out, const GameState& st) {
  out << st.timestamp.count() << " ";

  return out.write(reinterpret_cast<const char*>(&st.event), sizeof(sf::Event));
}

std::istream& operator>>(std::istream& in, GameState& st) {
  int ts;
  in >> ts;
  in.ignore();
  st.timestamp = std::chrono::milliseconds{ts};
  return in.read(reinterpret_cast<char*>(&st.event), sizeof(sf::Event));
}

#include <fstream>

//! dev tool,  serialization is minimal (use memcpy  for sf::event... )
struct Record {
  std::ofstream fout;
  std::ostream& out;
  Record(std::ostream& out_p) : out(out_p) {
    // can memcpy
    static_assert(std::is_trivially_copyable<sf::Event>::value);
  }

  Record(std::string filename) : fout(filename), out(fout) {}

  void push(const GameState& st) { out << st << std::endl; }
};

struct Replay {
  std::ifstream fin;
  std::istream& in;
  Replay(std::istream& in_p) : in(in_p) {}

  Replay(std::string filename) : fin{filename}, in(fin) {}

  [[nodiscard]] GameState pull() {
    GameState st;
    in >> st;
    in.ignore();  // consume endl from push
    return st;
  }

  bool can_pull() const {
    auto c = in.peek();
    return !in.eof();
  }
};