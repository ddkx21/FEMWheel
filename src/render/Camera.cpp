#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace {
    const glm::vec3 WORLD_UP(0.0f, 1.0f, 0.0f);
    constexpr float MAX_PITCH = 89.0f;
}

namespace Renderer {
    Camera::Camera(const glm::vec3& position, float yaw, float pitch)
        : m_position(position), m_yaw(yaw), m_pitch(pitch) {
        updateVectors();
    }

    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(m_position, m_position + m_front, WORLD_UP);
    }

    glm::mat4 Camera::getProjectionMatrix(float aspect) const {
        return glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
    }

    void Camera::move(const glm::vec3& direction, float deltaTime) {
        if (direction == glm::vec3(0.0f)) {
            return;
        }

        const glm::vec3 dir = glm::normalize(direction);
        m_position += (m_right * dir.x + WORLD_UP * dir.y + m_front * dir.z) * speed * deltaTime;
    }

    void Camera::rotate(float dx, float dy) {
        m_yaw += dx * sensitivity;
        m_pitch = glm::clamp(m_pitch - dy * sensitivity, -MAX_PITCH, MAX_PITCH); // экранный Y растёт вниз
        updateVectors();
    }

    void Camera::updateVectors() {
        const float yaw = glm::radians(m_yaw);
        const float pitch = glm::radians(m_pitch);
        m_front = glm::normalize(glm::vec3(cos(yaw) * cos(pitch),
                                           sin(pitch),
                                           sin(yaw) * cos(pitch)));
        m_right = glm::normalize(glm::cross(m_front, WORLD_UP));
    }
}
