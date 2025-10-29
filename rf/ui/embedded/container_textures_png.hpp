#pragma once

#include <cstdint>
#include <vector>

namespace rf {

namespace Ui {
    namespace Embedded {
        // Container diffuse texture in PNG format
        const std::vector<uint8_t>& ContainerDiffuseTexturePng();

        // Container specular texture in PNG format
        const std::vector<uint8_t>& ContainerSpecularTexturePng();
    }
}

} // namespace rf
