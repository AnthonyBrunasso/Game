#include "hexagon_shape.h"

#include "simulation/hex.h"

HexagonShape::HexagonShape(uint32_t size, sf::Vector2f position) {
  // Hexagons have 6 points :O
  m_polygon.setPointCount(6);

  for (uint32_t i = 0; i < 6; ++i) {
    m_polygon.setPoint(i, hex::hex_corner(size, i));
  }
  
  // Temporary cruft
  m_polygon.setOutlineColor(sf::Color::Red);
  m_polygon.setOutlineThickness(2);
  m_polygon.setFillColor(sf::Color::Black);
  m_polygon.setPosition(position);
}

const sf::Drawable& HexagonShape::get_drawable() const {
  return m_polygon;
}