#pragma once

namespace game_console {
  // Signals the game console to resume activity, it will wait after
  // a command is issued until it is finished being processed in message_stream.
  void signal();
  bool quit();
  void kill();
}