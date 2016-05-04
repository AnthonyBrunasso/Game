#include <SFML/Graphics.hpp>

#include <iostream>

#include "camera.h"
#include "constants.h"
#include "format.h"
#include "hex.h"
#include "hexagon_shape.h"
#include "map.h"

int main() {
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Foobar");

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
  std::function<void(const sf::Vector3i&)> draw_hex = [&](const sf::Vector3i& coord) {
    const sf::Vector2i offset = hex::cube_to_offset(coord);
    const sf::Vector2f pixel = hex::offset_to_pixel(offset, HEX_SIZE);
    hexagon_shape.m_polygon.setPosition(pixel);
    window.draw(hexagon_shape.get_drawable());
  };

  while (window.isOpen()) {
    sf::Event event;
    
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    map::update(camera);

    window.clear();
    map::for_each_tile(draw_hex);
    window.display();
  }

  return 0;
}