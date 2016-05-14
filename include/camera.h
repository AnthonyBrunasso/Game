#pragma once

#include <SFML/Graphics.hpp>

#include <cstdint>

class Camera {
public:
  Camera(sf::RenderWindow& window, sf::View& view);
  Camera(const Camera& copy) = delete;

  void move_to(const sf::Vector2f& position);
  void translate(const sf::Vector2f& offset);

  void update(float dt);

  sf::RenderWindow& get_window();
  sf::View& get_view();

  const sf::RenderWindow& get_const_window() const;
  const sf::View& get_const_view() const;

private:
  sf::RenderWindow& m_window;
  sf::View& m_view;
};

namespace camera {
  void initialize(float width, float height);
  Camera& get_camera();
}