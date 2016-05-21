#include "message.h"

#include "Tile.h"
#include "font.h"
#include "log.h"

#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>


namespace {
  std::queue<Message*> s_command_queue;
  std::mutex s_mutex;
  uint32_t s_unique_ids = 0;
}

uint32_t Message::create(Message* message) {
  std::lock_guard<std::mutex> lock(s_mutex);

  if (!message) {
    return INVALID_ID;
  }

  message->id = s_unique_ids++;

  s_command_queue.push(message);

  return message->id;
}

bool MoveToMessage::execute() {
  m_camera->move_to(m_position);
  return true;
}

bool ViewTileDataMessage::execute() {
  //Tile& tile = map::get_tile(m_coord);
  //std::ostringstream ss;
  //ss << "  id: " << tile.get_occupied_id();
  // Log here
  //logging::write(ss.str());

  //return true;
}

uint32_t message_stream::execute(uint32_t count) {
  std::lock_guard<std::mutex> lock(s_mutex);

  uint32_t executed = 0;
  for (uint32_t i = 0; i < count; ++i) {
    if (s_command_queue.size() == 0) break;
    Message* message = s_command_queue.front();
    message->execute();
    ++executed;
    s_command_queue.pop();
    delete message;
  }

  return executed;
}