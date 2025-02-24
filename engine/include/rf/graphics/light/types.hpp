#pragma once

namespace rf {

namespace Graphics {
    namespace Light {
        struct Properties {
            float ambient = 0.2f;
            float diffuse = 0.5f;
            float specular = 1.0f;
        };

        struct Attenuation {
            float constant = 0.2f;
            float linear = 0.09f;
            float quadratic = 0.032f;
        };

        struct Cutoff {
            float inner = 10.0f;
            float outer = 12.0f;
        };
    }
}

} // nameespace rf
