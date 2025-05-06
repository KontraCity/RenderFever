#pragma once

#include "rf/core/dispatcher.hpp"

namespace rf {

namespace Graphics {
    class Renderer {
    public:
        using UpdateDispatcher = Dispatcher<float>;

    private:
        UpdateDispatcher::Pointer m_updateDispatcher;

    public:
        Renderer() = default;

    private:
        float evaluateDeltaTime() const;

        void clearBuffers() const;

    public:
        void run();

    public:
        const UpdateDispatcher::Pointer& updateDispatcher() const {
            return m_updateDispatcher;
        }

        UpdateDispatcher::Pointer& updateDispatcher() {
            return m_updateDispatcher;
        }
    };
}

} // namespace rf
