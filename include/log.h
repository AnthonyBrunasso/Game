#pragma once

#include <string>

namespace log {
  void initialize();
  void kill();
  void write(const std::string& value);
}