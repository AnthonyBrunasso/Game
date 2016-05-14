#pragma once

// Map will be stored in cube coordinates

#include <cstdint>
#include <functional>
#include <SFML/System/Vector3.hpp>
#include <vector>
#include <string>

#include "camera.h"

class Tile;

namespace map {
  void build(const sf::Vector3i start, int32_t distance);
  void for_each_coord(std::function<void(const sf::Vector3i& cube_coord)> operation);
  Tile& get_tile(sf::Vector3i coord);
  std::string update(const Camera& camera);
}