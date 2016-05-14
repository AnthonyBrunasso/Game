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
#include "terminal.h"

int main() {
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Foobar");

  font::init_font();

  sf::View view;
  view.reset(sf::FloatRect(0, 0, 1280.0f, 720.0f));
  view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));

  window.setView(view);
  Camera camera(window, view);

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

    std::string mapPosition = map::update(camera);
    
    if (!font::render_string(window, mapPosition, sf::Vector2f(-630.0f, -350.0f)))
    {
      std::cout << mapPosition << std::endl;
    }
    map::for_each_tile(draw_hex);
    
    //draw mouse-over tile
    {
      sf::Vector2f mousePosition = camera.get_window().mapPixelToCoords(sf::Mouse::getPosition(window));
      const sf::Vector3i mouseCube = hex::world_to_cube(mousePosition, HEX_SIZE);
      const sf::Vector2f world = hex::cube_to_world(mouseCube, HEX_SIZE);
      hexagon_shape.m_polygon.setPosition(world);
      hexagon_shape.m_polygon.setOutlineColor(sf::Color::Green);
      window.draw(hexagon_shape.get_drawable());
    }

    window.display();
    window.clear();
  }

  terminal::kill();

  return 0;
}