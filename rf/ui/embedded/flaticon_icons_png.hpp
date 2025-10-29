#pragma once

#include <cstdint>
#include <vector>

namespace rf {

namespace Ui {
    namespace Embedded {
        // Directory regular (100x100) icon in PNG format
        const std::vector<uint8_t>& DirectoryRegularIconPng();

        // Directory tiny (14x14) icon in PNG format
        const std::vector<uint8_t>& DirectoryTinyIconPng();

        // File regular (100x100) icon in PNG format
        const std::vector<uint8_t>& FileRegularIconPng();

        // File tiny (14x14) icon in PNG format
        const std::vector<uint8_t>& FileTinyIconPng();

        // Gear regular (100x100) icon in PNG format
        const std::vector<uint8_t>& GearRegularIconPng();

        // Gear tiny (14x14) icon in PNG format
        const std::vector<uint8_t>& GearTinyIconPng();

        // Image regular (100x100) icon in PNG format
        const std::vector<uint8_t>& ImageRegularIconPng();

        // Image tiny (14x14) icon in PNG format
        const std::vector<uint8_t>& ImageTinyIconPng();

        // Cube regular (100x100) icon in PNG format
        const std::vector<uint8_t>& CubeRegularIconPng();

        // Cube tiny (14x14) icon in PNG format
        const std::vector<uint8_t>& CubeTinyIconPng();

        // Question regular (100x100) icon in PNG format
        const std::vector<uint8_t>& QuestionRegularIconPng();

        // Question tiny (14x14) icon in PNG format
        const std::vector<uint8_t>& QuestionTinyIconPng();

        // Left regular (14x14) icon in PNG format
        const std::vector<uint8_t>& LeftTinyIconPng();

        // Right regular (14x14) icon in PNG format
        const std::vector<uint8_t>& RightTinyIconPng();

        // Up regular (14x14) icon in PNG format
        const std::vector<uint8_t>& UpTinyIconPng();

        // Vertex file regular (100x100) icon in PNG format
        const std::vector<uint8_t>& FileVertRegularIconPng();

        // Geometry file regular (100x100) icon in PNG format
        const std::vector<uint8_t>& FileGeomRegularIconPng();

        // Fragment file regular (100x100) icon in PNG format
        const std::vector<uint8_t>& FileFragRegularIconPng();
    }
}

} // namespace rf
