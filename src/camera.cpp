#include "camera.h"

#include "constants.h"

Camera::Camera(sf::RenderWindow& window, sf::View& view) :
  m_window(window)
  , m_view(view) {
}

void Camera::move_to(const sf::Vector2f& position) {
  m_view.setCenter(position);
  m_window.setView(m_view);
}

void Camera::translate(const sf::Vector2f& offset) {
  m_view.move(offset);
  m_window.setView(m_view);
}

void Camera::update(float dt) {
  sf::Vector2f offset(0.0f, 0.0f);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    offset.x = -100.0f;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    offset.x = 100.0f;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    offset.y = -100.0f;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    offset.y = 100.0f;
  }

  offset *= dt;
  translate(offset);
}

sf::RenderWindow& Camera::get_window() {
  return m_window;
}

sf::View& Camera::get_view() {
  return m_view;
}

const sf::RenderWindow& Camera::get_const_window() const {
  return m_window;
}

const sf::View& Camera::get_const_view() const {
  return m_view;
}

namespace camera {
  sf::RenderWindow s_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Foobar");
  sf::View s_view;
  Camera s_camera(s_window, s_view);
}

void camera::initialize(float width, float height) {
  s_view.reset(sf::FloatRect(0, 0, width, height));
  s_view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));

  s_window.setView(s_view);
}

Camera& camera::get_camera() {
  return s_camera;
}