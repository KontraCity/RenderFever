#pragma once

// Graphics libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Custom graphics modules
#include "rf/graphics/shader.hpp"

namespace rf {

namespace Graphics
{
    namespace CameraConst
    {
        namespace Defaults
        {
            constexpr glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
            constexpr glm::vec3 Direction = glm::vec3(0.0f, 0.0f, -1.0f);
            constexpr glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
            constexpr float Yaw = -90.0f;
            constexpr float Pitch = 0.0f;
            constexpr float Zoom = 1.0f;
        }

        namespace Speed
        {
            constexpr float Slow = 1.0f;
            constexpr float Normal = 2.5f;
            constexpr float Fast = 20.0f;
        }

        namespace Sensivity
        {
            constexpr float Move = 0.1f;
            constexpr float Scroll = 0.1f;
        }

        namespace Pitch
        {
            constexpr float Min = -89.9f;
            constexpr float Max = -Min;
        }

        namespace Zoom
        {
            constexpr float Min = 0.3f;
            constexpr float Max = 20.0f;
        }

        namespace Perspective
        {
            constexpr float Near = 0.1f;
            constexpr float Far = 10000.0f;
        }
    }

    class Camera
    {
    private:
        /* Subscriber IDs */
        size_t m_keyEventSubscriberId = 0;
        size_t m_inputEventSubscriberId = 0;
        size_t m_cursorPosEventSubscriberId = 0;
        size_t m_scrollEventSubscriberId = 0;

        /* Variables */
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
        /// @brief Process single key event
        /// @param key The key event is associated with
        /// @param action Key action (press/repeat/release)
        /// @param mods Bit field describing which modifier keys were held down
        void onKey(int key, int action, int mods);

        /// @brief Process continuous key event
        /// @param window The window that received the event
        /// @param deltaTime Time passed from last frame
        void onInput(GLFWwindow* window, float deltaTime);

        /// @brief Process mouse cursor position event
        /// @param x New cursor X coordinate
        /// @param y New cursor Y coordinate
        void onCursorPos(double x, double y);

        /// @brief Process mouse scroll event
        /// @param xOffset X coordinate scroll offset
        /// @param yOffset y coordinate scroll offset
        void onScroll(double xOffset, double yOffset);

    public:
        /// @brief Capture camera scene
        /// @param shader The shader to capture objects with
        /// @param lightingShader The shader to capture lights with
        /// @param skyboxShader The shader to capture skybox with
        /// @param normalShader The shader to capture normals with
        /// @param width Window width
        /// @param height Window height
        void capture(Shader& shader, Shader& lightingShader, Shader& skyboxShader, Shader& normalShader, unsigned int width, unsigned int height);

        /// @brief Reset camera
        void reset();

    public:
        /// @brief Get camera position
        /// @return Camera position
        inline const glm::vec3& position() const
        {
            return m_position;
        }

        /// @brief Get camera position
        /// @return Camera position
        inline glm::vec3& position()
        {
            return m_position;
        }
    
        /// @brief Get camera direction
        /// @return Camera direction
        inline const glm::vec3& direction() const
        {
            return m_direction;
        }

        /// @brief Get camera direction
        /// @return Camera direction
        inline glm::vec3& direction()
        {
            return m_direction;
        }
    };
}

} // namespace rf
