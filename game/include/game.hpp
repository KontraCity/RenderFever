#pragma once

#include <rf/input/event.hpp>

class Game {
public:
    Game();

private:
    void onCloseWindow(const rf::Input::Event& event);

    void onSayHelloWorld(const rf::Input::Event& event);

    void onCursorMove(const rf::Input::Event& event);

    void onScroll(const rf::Input::Event& event);

    void onLeftMouseButton(const rf::Input::Event& event);

public:
    void run() const;
};
