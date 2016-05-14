#include "log.h"

#include <fstream>
#include <mutex>

namespace {
  std::fstream s_file;
  std::mutex s_mutex;
}

void logging::initialize() {
  s_file.open("debug.log", std::fstream::out);
}

void logging::kill() {
  s_file.close();
}

void logging::write(const std::string& value) {
  std::lock_guard<std::mutex> lock(s_mutex);

  s_file << value << std::endl;
}