#pragma once

namespace rf {

namespace Input {
    enum class Key {
        None,
        F1, F2, F3, F4, F5,
        F6, F7, F8, F9, F10,
        F11, F12, F13, F14, F15,
        F16, F17, F18, F19, F20,
        F21, F22, F23, F24, F25,
        Zero, One, Two, Three, Four,
        Five, Six, Seven, Eight, Nine,
        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Minus, Equal,
        Space, Backspace,
        LeftBracket, RightBracket,
        Backtick, Tab,
        Semicolon, Apostrophe,
        Comma, Period,
        Slash, Backslash,
        Escape, Enter,
        LeftShift, RightShift,
        LeftControl, RightControl,
        LeftAlt, RightAlt,
        LeftSuper, RightSuper,
        PrintScreen, Pause,
        Insert, Delete,
        Home, End,
        PageUp, PageDown,
        ArrowUp, ArrowDown,
        ArrowLeft, ArrowRight,
        CapsLock, ScrollLock, NumLock,
        NumpadZero, NumpadOne, NumpadTwo, NumpadThree, NumpadFour,
        NumpadFive, NumpadSix, NumpadSeven, NumpadEight, NumpadNine,
        NumpadDecimal, NumpadEqual, NumpadEnter,
        NumpadDivide, NumpadMultiply,
        NumpadSubtract, NumpadAdd,
    };

    const char* KeyName(Key key);

    Key GlfwMacroToKey(int key);

    int KeyToGlfwMacro(Key key);
}

} // namespace rf
