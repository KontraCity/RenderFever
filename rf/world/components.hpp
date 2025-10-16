#pragma once

#include <functional>

#include <rf/graphics/lighting.hpp>
#include <rf/graphics/material.hpp>
#include <rf/graphics/transform.hpp>
#include <rf/resources/library.hpp>
#include <rf/world/entity.hpp>

namespace rf {

namespace World {
    struct LogicComponent {
        using Callback = std::function<void(Entity&, float)>;
        Callback onStart;
        Callback onUpdate;
    };
    
    struct CameraComponent {
    private:
        friend class Scene;
        struct ActiveCameraTag {};  // Use this camera to capture scene

    public:
        Graphics::Camera camera;
    };

    struct LightComponent {
        Graphics::Light light;
    };

    struct DrawComponent {
        Graphics::Transform transform;
        Graphics::Material material;
        Resources::Mesh mesh;
    };
}

} // namespace rf
