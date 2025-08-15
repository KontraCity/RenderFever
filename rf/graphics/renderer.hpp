#pragma once

#include "rf/core/dispatcher.hpp"

namespace rf {

class Renderer {
public:
    using UpdateDispatcher = Dispatcher<float>;

private:
    UpdateDispatcher::Pointer m_updateDispatcher;
    mutable float m_deltaTime = 0.0f;
    mutable float m_time = 0.0f;

public:
    Renderer() = default;

private:
    void evaluateTimes() const;

    void clearBuffers() const;

public:
    void run();

public:
    UpdateDispatcher::Pointer& updateDispatcher() {
        return m_updateDispatcher;
    }

    float deltaTime() const {
        return m_deltaTime;
    }

    float time() const {
        return m_time;
    }
};

} // namespace rf
