#pragma once
#include "SDL_scancode.h"

namespace core {
class Camera {
public:
  Camera() noexcept;
  Camera(glm::vec3 position) noexcept;
  Camera(glm::vec3 position, glm::vec3 target) noexcept;

public:
  void Update(int relX, int relY);
  void MoveByKeyboard(const Uint8 *keystates, float deltaTime);

  const glm::vec3 &GetTarget() noexcept;
  const glm::vec3 &GetPosition() noexcept;
  void SetPosition(const glm::vec3 position) noexcept;
  void SetTarget(const glm::vec3 target) noexcept;

private:
  glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 m_Target = glm::vec3(0.0f, 0.0f, 0.0f); // right axis
  float m_Yaw = -90.0f;
  float m_Pitch = 0.0f;
};
} // namespace core
