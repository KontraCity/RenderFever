#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "rf/graphics/shader.hpp"

namespace rf {

namespace Graphics {
    namespace CameraConst {
        namespace Defaults {
            constexpr glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
            constexpr glm::vec3 Direction = glm::vec3(0.0f, 0.0f, -1.0f);
            constexpr glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
            constexpr float Yaw = -90.0f;
            constexpr float Pitch = 0.0f;
            constexpr float Zoom = 1.0f;
        }

        namespace Speed {
            constexpr float Slow = 1.0f;
            constexpr float Normal = 2.5f;
            constexpr float Fast = 20.0f;
        }

        namespace Sensivity {
            constexpr float Move = 0.1f;
            constexpr float Scroll = 0.1f;
        }

        namespace Pitch {
            constexpr float Min = -89.9f;
            constexpr float Max = -Min;
        }

        namespace Zoom {
            constexpr float Min = 0.3f;
            constexpr float Max = 20.0f;
        }

        namespace Perspective {
            constexpr float Near = 0.1f;
            constexpr float Far = 10000.0f;
        }
    }

    class Camera {
    private:
        size_t m_keyEventSubscriberId = 0;
        size_t m_inputEventSubscriberId = 0;
        size_t m_cursorPosEventSubscriberId = 0;
        size_t m_scrollEventSubscriberId = 0;

        glm::vec3 m_position;
        glm::vec3 m_direction;
        glm::vec3 m_up;
        float m_yaw = 0.0f;
        float m_pitch = 0.0f;
        float m_zoom = 0.0f;

    public:
        Camera();

        Camera(const Camera& other) = delete;

        Camera(Camera&& other) noexcept = delete;

        ~Camera();

    private:
        void onKey(int key, int action, int mods);

        void onInput(GLFWwindow* window, float deltaTime);

        void onCursorPos(double x, double y);

        void onScroll(double xOffset, double yOffset);

    public:
        void capture(Shader& shader, Shader& lightingShader, Shader& skyboxShader, Shader& normalShader, unsigned int width, unsigned int height);

        void reset();

    public:
        inline const glm::vec3& position() const {
            return m_position;
        }

        inline glm::vec3& position() {
            return m_position;
        }
    
        inline const glm::vec3& direction() const {
            return m_direction;
        }

        inline glm::vec3& direction() {
            return m_direction;
        }
    };
}

} // namespace rf
