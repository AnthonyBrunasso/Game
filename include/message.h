#pragma once

#include <cstdint>

#include "camera.h"

#include "Simulation/step.h"

class Message {
public:
  Message() : id(INVALID_ID) {};
  virtual ~Message() {};

  // Returns id
  static uint32_t create(Message* message);

  virtual bool execute() = 0;
  uint32_t get_id() { return id; };

  static const uint32_t INVALID_ID = UINT32_MAX;

private:
  uint32_t id;
};

class MoveToMessage : public Message {
public:
  MoveToMessage(Camera& camera, const sf::Vector2f position) :
    m_position(position)
    , m_camera(&camera) {};

  virtual bool execute() override;

private:
  sf::Vector2f m_position;
  Camera* m_camera;
};

class ViewTileDataMessage : public Message {
public:
  ViewTileDataMessage(sf::Vector3i coord) : 
    m_coord(coord) {};

  virtual bool execute() override;

private:
  sf::Vector3i m_coord;
};

class StepSimulationMessage : public Message {
public:
  StepSimulationMessage(Step* step) :
    m_step(step) {};
  ~StepSimulationMessage() { delete m_step; }

  virtual bool execute() override;

private:
  Step* m_step;
};

namespace message_stream {
  // Returns number of messages actually executed
  uint32_t execute(uint32_t count);
}