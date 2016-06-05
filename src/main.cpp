#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

#include "camera.h"
#include "constants.h"
#include "font.h"
#include "game_console.h"
#include "hexagon_shape.h"
#include "message.h"
#include "log.h"

#include "simulation/world_map.h"
#include "simulation/hex.h"
#include "simulation/step.h"
#include "simulation/unique_id.h"
#include "simulation/simulation.h"
#include "simulation/tile.h"
#include "simulation/format.h"

void initialize() {
  camera::initialize(1280.0f, 720.0f);
  font::init_font();
  logging::initialize();

  Camera& camera = camera::get_camera();

  // Center the camera at 0, 0
  camera.move_to(START_PIXEL);

  // Begin the simulation
  simulation::start();
}

std::string coord_str(const Camera& camera) {
  const sf::Vector2i mouse_position = sf::Mouse::getPosition(camera.get_const_window());
  const sf::Vector2f world_position = camera.get_const_window().mapPixelToCoords(mouse_position);
  const sf::Vector2i axial = hex::world_to_axial(world_position, HEX_SIZE);
  const sf::Vector3i cube = hex::world_to_cube(world_position, HEX_SIZE);
  const sf::Vector2i offset = hex::world_to_offset(world_position, HEX_SIZE);

  std::ostringstream ss;
  ss << "Mouse: " << format::vector2(mouse_position) << std::endl
    << "World: "  << format::vector2(world_position) << std::endl 
    << "Axial: "  << format::vector2(axial)  << std::endl
    << "Cube: "   << format::vector3(cube) << std::endl 
    << "Offset: " << format::vector2(offset);

  return std::move(ss.str());
}

int main() {
  initialize();
  game_console::create();

  Camera& camera = camera::get_camera();
  sf::RenderWindow& window = camera.get_window();

  HexagonShape hexagon(HEX_SIZE, START_PIXEL);

  sf::RectangleShape rect;
  rect.setSize(sf::Vector2f(20.0f, 20.0f));
  rect.setFillColor(sf::Color::Blue);

  // Circles are units
  sf::CircleShape circle;
  circle.setRadius(10);
  circle.setFillColor(sf::Color::Green);

  // Function responsible for drawing the hexagon shapes
  std::function<void(const sf::Vector3i&, const Tile&)> draw_hex = [&](const sf::Vector3i& cube_coord, const Tile& tile) {
    const sf::Vector2f world = hex::cube_to_world(cube_coord, HEX_SIZE);
    hexagon.m_polygon.setPosition(world);
    // Give the hexagon some transparency so it's not overwhelming
    hexagon.m_polygon.setOutlineColor(sf::Color(255.0f, 0.0f, 0.0f, 100.0f));
    window.draw(hexagon.get_drawable());

    if (tile.m_city_id != unique_id::INVALID_ID) {
      // Offset slightly left and move up
      rect.setPosition(world - sf::Vector2f(20.0f, 10.0f));
      window.draw(rect);
    }

    if (!tile.m_unit_ids.empty()) {
      // Move slightly up
      circle.setPosition(world - sf::Vector2f(-5.0f, 10.0f));
      window.draw(circle);
    }
  };

  sf::Clock clock;
  std::string coords;
  // For now, display only changes when a command is received from terminal
  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    if (game_console::quit()) {
      window.close();
    }

    sf::Time dt = clock.restart();
    camera.update(dt.asSeconds());

    // This is what step simulation code forward. See: StepSimulationMessage
    message_stream::execute(1);

    window.clear();
    world_map::for_each_tile(draw_hex);
    {
      sf::Vector2f mousePosition = camera.get_window().mapPixelToCoords(sf::Mouse::getPosition(window));
      const sf::Vector3i mouseCube = hex::world_to_cube(mousePosition, HEX_SIZE);
      const sf::Vector2f world = hex::cube_to_world(mouseCube, HEX_SIZE);
      hexagon.m_polygon.setPosition(world);
      hexagon.m_polygon.setOutlineColor(sf::Color::Green);
      // Make it fully transparent so you can see units on the tile
      hexagon.m_polygon.setFillColor(sf::Color(0.0f, 0.0f, 0.0f, 0.0f));
      window.draw(hexagon.get_drawable());
    }

    coords = coord_str(camera);
    font::render_string(window, coords, sf::Vector2f(-630.0f, -350.0f));

    window.display();
  }

  logging::kill();
  game_console::kill();

  return 0;
}