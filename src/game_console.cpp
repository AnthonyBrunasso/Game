#include "game_console.h"

#include <iostream>
#include <thread>
#include <condition_variable>

#include "message.h"

#include "simulation/terminal.h"

namespace {
  bool s_kill = false;
  bool s_continue = false;
  std::mutex m_mutex;
  std::condition_variable m_condition;

  void initialize();
  void run();

  void initialize() {
    terminal::add_query("camera", "camera <x> <y>", [](const std::vector<std::string>& tokens) {
      if (tokens.size() != 3) {
        return true;
      }
      float x = std::stof(tokens[2]);
      float y = std::stof(tokens[3]);
      Message::create(new MoveToMessage(camera::get_camera(), sf::Vector2f(x, y)));
      return true;
    });
  }

  void run() {
    initialize();
    while (!s_kill) {
      // Make sure condition is set up to wait
      s_continue = false;

      // Parse input until a valid simulation step is created
      Step* step = terminal::parse_input();

      // If it was a quit signal this thread to quit
      if (step->m_command == COMMAND::QUIT) {
        s_kill = true;
      }

      // Queue simulation step to be processed
      Message::create(new StepSimulationMessage(step));

      // Wait for the worker to finish (step processed) to continue.
      // This will make sure IO from simulation won't be malformed by this threads text.
      {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, []{ return s_continue; });
      }
    }
  }

  std::thread* s_thread;
}

void game_console::create() {
  s_thread = new std::thread(&run);
}

void game_console::signal() {
  s_continue = true;
  m_condition.notify_one();
}

bool game_console::quit() {
  return s_kill;
}

void game_console::kill() {
  s_continue = true;
  s_kill = true;
  m_condition.notify_one();
  s_thread->join();
  delete s_thread;
  s_thread = nullptr;
}