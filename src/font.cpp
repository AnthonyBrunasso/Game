#include "font.h"

#include <SFML/Graphics.hpp>

namespace font
{
  ::sf::Font s_font;
  bool s_ready;
}

void font::init_font()
{
  // Load it from a file
  if (s_font.loadFromFile("arial.ttf"))
  {
   s_ready = true;
  }
}

bool font::render_string(sf::RenderWindow& renderWindow, std::string& displayText, const sf::Vector2f& displayLocation)
{
  if (s_ready)
  {
    sf::Text renderText(displayText.c_str(), s_font);
    renderText.setColor(sf::Color::White);
    renderText.setPosition(displayLocation);
    renderWindow.draw(renderText);
  }

  return s_ready;
}