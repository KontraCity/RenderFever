> Judge not, and you will not be judged;
> condemn not, and you will not be condemned;
> forgive, and you will be forgiven.

God help us all.


# RenderFever Engine

## Build
#### Dependencies
* [\{fmt}](https://github.com/fmtlib/fmt) - A modern formatting library
* [spdlog](https://github.com/gabime/spdlog) - Fast C++ logging library
* [OpenGL](https://www.khronos.org/opengl/) - The OpenGL specification
* [GLEW](https://github.com/nigels-com/glew) - OpenGL Extention Wrangler
* [GLFW](https://github.com/glfw/glfw) - OpenGL Framework
* [GLM](https://github.com/g-truc/glm) - OpenGL Mathematics
* [Assimp](https://github.com/assimp/assimp) - Open Asset Import Library

#### Windows
Using `vcpkg` to install the dependencies:
```ps
> vcpkg install fmt
> vcpkg install spdlog
> vcpkg install opengl
> vcpkg install glew
> vcpkg install glfw3
> vcpkg install glm
> vcpkg install assimp
```
Using `Visual Studio` to build:
1. Set `REAL_VCPKG_ROOT` environment variable to path of the root directory of `vcpkg`
2. Open Visual Studio and clone https://github.com/KontraCity/RenderFever.git
3. Open directory in solution explorer, let CMake configure the project
4. Select `Windows Release` configuration, build the project

#### Linux
Using `apt` to install the dependencies:
```sh
$ apt install libfmt-dev
$ apt install libspdlog-dev
$ apt install libopengl-dev
$ apt install libglew-dev
$ apt install libglfw3-dev
$ apt install libglm-dev
$ apt install libassimp-dev
```
Using `make` to build:
```sh
$ git clone https://github.com/KontraCity/RenderFever.git && cd RenderFever
$ mkdir build && cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make -j
```
