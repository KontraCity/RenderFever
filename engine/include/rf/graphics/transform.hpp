#pragma once

// Graphics libraries
#include <glm/glm.hpp>

// Custom modules
#include "rf/graphics/shader.hpp"

namespace rf {

namespace Graphics
{
    class Transform
    {
    protected:
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

    protected:
        /// @brief Apply transformations
        /// @param shader The shader to apply transformations to
        void transform(Shader& shader) const;

    public:
        /// @brief Create transform
        /// @param position Transform position
        /// @param rotation Transform rotation
        /// @param scale Transform scale
        Transform(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));

        /// @brief Get transform position
        /// @return Transform position
        inline const glm::vec3& position() const
        {
            return m_position;
        }

        /// @brief Get transform position
        /// @return Transform position
        inline glm::vec3& position()
        {
            return m_position;
        }

        /// @brief Get transform rotation
        /// @return Transform rotation
        inline const glm::vec3& rotation() const
        {
            return m_rotation;
        }

        /// @brief Get transform rotation
        /// @return Transform rotation
        inline glm::vec3& rotation()
        {
            return m_rotation;
        }

        /// @brief Get transform scale
        /// @return Transform scale
        inline const glm::vec3& scale() const
        {
            return m_scale;
        }

        /// @brief Get transform scale
        /// @return Transform scale
        inline glm::vec3& scale()
        {
            return m_scale;
        }
    };
}

} // namespace rf
