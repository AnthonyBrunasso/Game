#include "log.h"

#include <fstream>
#include <mutex>

namespace {
  std::fstream s_file;
  std::mutex s_mutex;
}

void log::initialize() {
  s_file.open("debug.log", std::fstream::out);
}

void log::kill() {
  s_file.close();
}

void log::write(const std::string& value) {
  std::lock_guard<std::mutex> lock(s_mutex);

  s_file << value << std::endl;
}