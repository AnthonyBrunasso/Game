#pragma once

// NOTE:
// Changing a constant in here requires a clean build because it's a header
// and I don't know how to get Make to pick it up
// 

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <cstdint>

// Some ugly constants

extern const uint32_t WINDOW_WIDTH;
extern const uint32_t WINDOW_HEIGHT;

extern const uint32_t HEX_SIZE;
// Distance in every direction from center, 0,0 grid
extern const int32_t MAP_DISTANCE;
extern const sf::Vector2f START_PIXEL;
extern const sf::Vector3i START_CUBE;

extern float CAMERA_SPEED;