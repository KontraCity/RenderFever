#include "engine.hpp"

namespace rf {

// std::make_unique requires public constructor, hence here operator new is used instead
std::unique_ptr<Engine> Engine::Instance(new Engine);

Engine::Engine()
    : m_window("RenderFever Engine", { 1280, 720 })
{}

} // namespace rf
