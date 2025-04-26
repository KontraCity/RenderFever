#pragma once

namespace rf {

namespace Input {
    enum class Action {
        None,
        Press,
        Release,
        Repeat,
    };

    const char* ActionName(Action action);

    Action GlfwMacroToAction(int action);

    int ActionToGlfwMacro(Action action);
}

} // namespace rf
