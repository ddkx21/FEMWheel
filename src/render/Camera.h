#pragma once

#include <glm/glm.hpp>

namespace Renderer {
    class Camera {
    public:
        explicit Camera(const glm::vec3& position, float yaw = -90.0f, float pitch = 0.0f);

        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix(float aspect) const;


        void move(const glm::vec3& direction, float deltaTime);

        void rotate(float dx, float dy);

        float speed = 3.0f;
        float sensitivity = 0.1f;
        float fov = 60.0f;

    private:
        void updateVectors();

        glm::vec3 m_position;
        glm::vec3 m_front{0.0f, 0.0f, -1.0f};
        glm::vec3 m_right{1.0f, 0.0f, 0.0f};
        float m_yaw;
        float m_pitch;
    };
}
