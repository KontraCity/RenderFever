set(RfLinkDependencies "")

# fmt - An open-source formatting library providing a fast and safe alternative to C stdio and C++ iostreams.
# https://github.com/fmtlib/fmt
# vcpkg install fmt
find_package(fmt CONFIG REQUIRED)
list(APPEND RfLinkDependencies fmt::fmt)

# Frozen - Header-only library that provides 0 cost initialization for immutable containers, fixed-size containers, and various algorithms.
# https://github.com/serge-sans-paille/frozen
# vcpkg install frozen
find_package(frozen CONFIG REQUIRED)
list(APPEND RfLinkDependencies frozen::frozen-headers)

# OpenGL - The Industry Standard for High Performance Graphics.
# https://www.opengl.org/
# vcpkg install opengl
find_package(OpenGL REQUIRED)
list(APPEND RfLinkDependencies OpenGL::GL)

# GLEW - The OpenGL Extension Wrangler Library.
# https://github.com/nigels-com/glew
# vcpkg install glew
find_package(GLEW REQUIRED)
list(APPEND RfLinkDependencies GLEW::GLEW)

# GLFW - A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input.
# https://github.com/glfw/glfw
# vcpkg install glfw3
find_package(glfw3 CONFIG REQUIRED)
list(APPEND RfLinkDependencies glfw)

# GLM - OpenGL Mathematics.
# https://github.com/g-truc/glm
# vcpkg install glm
find_package(glm CONFIG REQUIRED)
list(APPEND RfLinkDependencies glm::glm)

# STB - Single-file public domain libraries for C/C++.
# https://github.com/nothings/stb
# vcpkg install stb
find_package(Stb REQUIRED)
include_directories("${Stb_INCLUDE_DIR}")

# flecs - A fast entity component system (ECS) for C & C++.
# https://github.com/SanderMertens/flecs
# vcpkg install flecs
find_package(flecs CONFIG REQUIRED)
list(APPEND RfLinkDependencies flecs::flecs_static)

# Dear ImGui - Bloat-free Graphical User interface for C++ with minimal dependencies.
# https://github.com/ocornut/imgui
# vcpkg install imgui
find_package(imgui CONFIG REQUIRED)
list(APPEND RfLinkDependencies imgui::imgui)

# zlib - A massively spiffy yet delicately unobtrusive compression library.
# https://github.com/madler/zlib
# vcpkg install zlib
find_package(ZLIB REQUIRED)
list(APPEND RfLinkDependencies ZLIB::ZLIB)

# assimp - Open Asset Import Library.
# https://github.com/assimp/assimp
# vcpkg install assimp
find_package(assimp CONFIG REQUIRED)
list(APPEND RfLinkDependencies assimp::assimp)
