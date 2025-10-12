#pragma once

#include <functional>

#include <rf/graphics/lighting.hpp>
#include <rf/graphics/material.hpp>
#include <rf/graphics/mesh.hpp>
#include <rf/graphics/transform.hpp>
#include <rf/resources/library.hpp>
#include <rf/world/entity.hpp>

namespace rf {

namespace World {
    struct StartComponent {
        using Callback = std::function<void(Entity&)>;
        Callback onStart;
    };

    struct UpdateComponent {
        using Callback = std::function<void(Entity&, float)>;
        Callback onUpdate;
    };

    struct DrawComponent {
        Graphics::Transform transform;
        Graphics::Material material;
        Resources::Mesh mesh;
    };

    // TODO: Standardize components and actually use this!
    struct LightComponent {
        Graphics::Light light;
    };
}

} // namespace rf
