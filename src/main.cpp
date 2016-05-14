#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

#include "camera.h"
#include "constants.h"
#include "font.h"
#include "format.h"
#include "hex.h"
#include "hexagon_shape.h"
#include "map.h"
#include "message.h"
#include "log.h"
#include "terminal.h"

int main() {
  camera::initialize(1280.0f, 720.0f);
  font::init_font();
  log::initialize();
  Camera& camera = camera::get_camera();
  sf::RenderWindow& window = camera.get_window();

  // Center the camera at 0, 0
  camera.move_to(START_PIXEL);

  HexagonShape hexagon_shape(HEX_SIZE, START_PIXEL);

  map::build(START_CUBE, MAP_DISTANCE);

  // Function responsible for drawing the hexagon shapes
  std::function<void(const sf::Vector3i&)> draw_hex = [&](const sf::Vector3i& cube_coord) {
    const sf::Vector2f world = hex::cube_to_world(cube_coord, HEX_SIZE);
    hexagon_shape.m_polygon.setPosition(world);
    hexagon_shape.m_polygon.setOutlineColor(sf::Color::Red);
    window.draw(hexagon_shape.get_drawable());
  };

  sf::Clock clock;

  while (window.isOpen()) {
    sf::Event event;
    
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    if (terminal::quit()) {
      window.close();
    }

    sf::Time dt = clock.restart();
    std::string mapPosition = map::update(camera);

    map::for_each_coord(draw_hex);
    camera.update(dt.asSeconds());
    message_stream::execute(1);
    
    //draw mouse-over tile
    {
      sf::Vector2f mousePosition = camera.get_window().mapPixelToCoords(sf::Mouse::getPosition(window));
      const sf::Vector3i mouseCube = hex::world_to_cube(mousePosition, HEX_SIZE);
      const sf::Vector2f world = hex::cube_to_world(mouseCube, HEX_SIZE);
      hexagon_shape.m_polygon.setPosition(world);
      hexagon_shape.m_polygon.setOutlineColor(sf::Color::Green);
      window.draw(hexagon_shape.get_drawable());
    }

    if (!font::render_string(window, mapPosition, sf::Vector2f(-630.0f, -350.0f)))
    {
      std::cout << mapPosition << std::endl;
    }

    window.display();
    window.clear();
  }

  log::kill();
  terminal::kill();

  return 0;
}