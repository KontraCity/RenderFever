#pragma once

#include <rf/input/action.hpp>

class Game {
private:
    bool m_stop = false;

public:
    Game();

private:
    void closeWindow(rf::Input::Action action);

    void sayHelloWorld(rf::Input::Action action);

public:
    void run();
};
