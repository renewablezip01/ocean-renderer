#include "camera.hpp"

namespace core {
Camera::Camera() noexcept {}
Camera::Camera(glm::vec3 position) noexcept : m_Position(position) {}
Camera::Camera(glm::vec3 position, glm::vec3 target) noexcept
    : m_Position(position), m_Target(target) {}

void Camera::MoveByKeyboard(const Uint8 *keystates, float deltaTime) {
  static constexpr auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  float cameraSpeed = 0.005f * deltaTime;
  if (keystates[SDL_SCANCODE_LSHIFT])
    cameraSpeed = 0.025f * deltaTime;

  if (keystates[SDL_SCANCODE_UP] || keystates[SDL_SCANCODE_W])
    this->m_Position += cameraSpeed * this->m_Target;
  if (keystates[SDL_SCANCODE_DOWN] || keystates[SDL_SCANCODE_S])
    this->m_Position -= cameraSpeed * this->m_Target;
  if (keystates[SDL_SCANCODE_RIGHT] || keystates[SDL_SCANCODE_D])
    this->m_Position +=
        glm::normalize(glm::cross(this->m_Target, cameraUp)) * cameraSpeed;
  if (keystates[SDL_SCANCODE_LEFT] || keystates[SDL_SCANCODE_A])
    this->m_Position -=
        glm::normalize(glm::cross(this->m_Target, cameraUp)) * cameraSpeed;
}

void Camera::Update(int relX, int relY) {
  constexpr float sensitivity = 0.2f;
  this->m_Yaw += relX * sensitivity;
  this->m_Pitch -= relY * sensitivity;

  if (m_Pitch > 89.0f)
    m_Pitch = 89.0f;
  if (m_Pitch < -89.0f)
    m_Pitch = -89.0f;

  this->m_Target = glm::normalize(
      glm::vec3(cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
                sin(glm::radians(m_Pitch)),
                sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))));
}

const glm::vec3 &Camera::GetTarget() noexcept { return this->m_Target; }
const glm::vec3 &Camera::GetPosition() noexcept { return this->m_Position; }

void Camera::SetPosition(const glm::vec3 position) noexcept {
  this->m_Position = position;
}

void Camera::SetTarget(const glm::vec3 target) noexcept {
  this->m_Target = target;
}

} // namespace core
