#include "terminal.h"
#include "camera.h"
#include "message.h"

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
    std::cout << "  help                // shows all terminal commands" << std::endl; 
    std::cout << "  camera move <x> <y> // move camera to x, y pixel coordinates" << std::endl;
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
    }

    if (tokens[0] == "camera") {
      if (tokens[1] == "move") {
        float x = std::stof(tokens[2]);
        float y = std::stof(tokens[3]);
        Message::create(new MoveToMessage(camera::get_camera(), sf::Vector2f(x, y)));
      }
    }

    return false;
  }

  void run() {
    std::cout << "INTERACTIVE CONSOLE" << std::endl;
    std::cout << "-------------------" << std::endl;
    while (!s_kill) {
      std::cout << std::endl;
      std::cout << "> ";
      std::string value;
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