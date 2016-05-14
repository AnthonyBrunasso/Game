#include "map.h"

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "constants.h"
#include "format.h"
#include "hex.h"
#include "search.h"
#include "tile.h"

namespace {
  static std::unordered_map<sf::Vector3i, Tile> s_map;
  static std::vector<sf::Vector3i> s_coords;
}

void map::build(const sf::Vector3i start, int32_t distance) {
  search::range(start, distance, s_coords);
  for (auto tile : s_coords) {
    s_map[tile] = Tile();
  }
}

void map::for_each_coord(std::function<void(const sf::Vector3i& cube_coord)> operation) {
  for (auto coord : s_coords) {
    operation(coord);
  }
}

Tile& map::get_tile(sf::Vector3i coord) {
  return s_map[coord];
}

std::string map::update(const Camera& camera) {
  const sf::Vector2i mouse_position = sf::Mouse::getPosition(camera.get_const_window());
  const sf::Vector2f world_position = camera.get_const_window().mapPixelToCoords(mouse_position);
  const sf::Vector2i axial = hex::world_to_axial(world_position, HEX_SIZE);
  const sf::Vector3i cube = hex::world_to_cube(world_position, HEX_SIZE);
  const sf::Vector2i offset = hex::world_to_offset(world_position, HEX_SIZE);

  std::ostringstream ss;
  ss << "World: "  << format::vector2(world_position) << std::endl 
    << "Axial: "  << format::vector2(axial)  << std::endl
    << "Cube: "   << format::vector3(cube) << std::endl 
    << "Offset: " << format::vector2(offset);

  return std::move(ss.str());
}