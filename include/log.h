#pragma once

#include <string>

namespace logging {
  void initialize();
  void kill();
  void write(const std::string& value);
}