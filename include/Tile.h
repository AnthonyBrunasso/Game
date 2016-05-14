#pragma once



class Tile {
public:
  Tile() : m_occupied_id(0) {};

  uint32_t get_occupied_id() { return m_occupied_id; };

private:
  uint32_t m_occupied_id;
};

namespace std {
  template <>
  struct hash<sf::Vector3i> {
    std::size_t operator()(const sf::Vector3i& tile) const {
      const size_t h1 = 0x8da6b343;
      const size_t h2 = 0xd8163841;
      const size_t h3 = 0xcb1ab31f;
      return tile.x * h1 + tile.y * h2 + tile.z * h3;
    }
  };
}