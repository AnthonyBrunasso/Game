#include "terminal.h"
#include "camera.h"
#include "message.h"
#include "constants.h"

#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

namespace {
  bool s_kill = false;

  void execute_help() {
    std::cout << "Commands: " << std::endl;
    std::cout << "  help                 // shows all terminal commands" << std::endl; 
    std::cout << "  camera move <x> <y>  // move camera to x, y pixel coordinates" << std::endl;
    std::cout << "  camera speed <value> // change speed of camera" << std::endl;
    std::cout << "  tile <x> <y> <z>     // view data on tile" << std::endl;
  }

  void execute_default() {
    std::cout << "Command not recognized, type help for options" << std::endl;
  }

  bool parse_tokens(std::vector<std::string> tokens) {
    if (!tokens.size()) {
      execute_default();
      return false;
    }

    if (tokens[0] == "quit" || s_kill) {
      return true;
    }

    if (tokens[0] == "help") {
      execute_help();

      return false;
    }

    if (tokens[0] == "camera") {
      if (tokens[1] == "move") {
        float x = std::stof(tokens[2]);
        float y = std::stof(tokens[3]);
        Message::create(
          new MoveToMessage(camera::get_camera(), sf::Vector2f(x, y)));
      }

      if (tokens[1] == "speed") {
        CAMERA_SPEED = std::stof(tokens[2]);
      }

      return false;
    }

    if (tokens[0] == "tile") {
      int x = std::stof(tokens[1]);
      int y = std::stof(tokens[2]);
      int z = std::stof(tokens[3]);
      Message::create(
        new ViewTileDataMessage(sf::Vector3i(x, y, z)));

      return false;
    }

    return false;
  }

  void run() {
    std::cout << "INTERACTIVE CONSOLE" << std::endl;
    std::cout << "-------------------" << std::endl;
    while (!s_kill) {
      std::cout << std::endl;
      std::string value;
      std::cout << "> ";
      std::getline(std::cin, value);
      if (!value.size()) {
        execute_default();
        continue;
      }
      std::stringstream iss(value);
      std::vector<std::string> tokens;

      // Split string on space
      std::copy(std::istream_iterator<std::string>(iss),
        std::istream_iterator<std::string>(),
        std::back_inserter(tokens));

      if (parse_tokens(tokens)) {
        s_kill = true;
      }
    }
  }

  std::thread s_thread(&run);
}

void terminal::kill() {
  s_kill = true;
  s_thread.join();
}

bool terminal::quit() {
  return s_kill;
}