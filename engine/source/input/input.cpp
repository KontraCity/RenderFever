#include "rf/input/input.hpp"

#include <GLFW/glfw3.h>

namespace rf {
    
bool Input::IsSpecialInput(Input input) {
    switch (input) {
        case Input::Special_CursorMove:
        case Input::Special_Scroll:
            return true;
        default:
            return false;
    }
}

const char* Input::InputName(Input input) {
    switch (input) {
        // Keyboard
        case Input::Key_F1:                return "F1";
        case Input::Key_F2:                return "F2";
        case Input::Key_F3:                return "F3";
        case Input::Key_F4:                return "F4";
        case Input::Key_F5:                return "F5";
        case Input::Key_F6:                return "F6";
        case Input::Key_F7:                return "F7";
        case Input::Key_F8:                return "F8";
        case Input::Key_F9:                return "F9";
        case Input::Key_F10:               return "F10";
        case Input::Key_F11:               return "F11";
        case Input::Key_F12:               return "F12";
        case Input::Key_F13:               return "F13";
        case Input::Key_F14:               return "F14";
        case Input::Key_F15:               return "F15";
        case Input::Key_F16:               return "F16";
        case Input::Key_F17:               return "F17";
        case Input::Key_F18:               return "F18";
        case Input::Key_F19:               return "F19";
        case Input::Key_F20:               return "F20";
        case Input::Key_F21:               return "F21";
        case Input::Key_F22:               return "F22";
        case Input::Key_F23:               return "F23";
        case Input::Key_F24:               return "F24";
        case Input::Key_F25:               return "F25";
        case Input::Key_0:                 return "0";
        case Input::Key_1:                 return "1";
        case Input::Key_2:                 return "2";
        case Input::Key_3:                 return "3";
        case Input::Key_4:                 return "4";
        case Input::Key_5:                 return "5";
        case Input::Key_6:                 return "6";
        case Input::Key_7:                 return "7";
        case Input::Key_8:                 return "8";
        case Input::Key_9:                 return "9";
        case Input::Key_A:                 return "A";
        case Input::Key_B:                 return "B";
        case Input::Key_C:                 return "C";
        case Input::Key_D:                 return "D";
        case Input::Key_E:                 return "E";
        case Input::Key_F:                 return "F";
        case Input::Key_G:                 return "G";
        case Input::Key_H:                 return "H";
        case Input::Key_I:                 return "I";
        case Input::Key_J:                 return "J";
        case Input::Key_K:                 return "K";
        case Input::Key_L:                 return "L";
        case Input::Key_M:                 return "M";
        case Input::Key_N:                 return "N";
        case Input::Key_O:                 return "O";
        case Input::Key_P:                 return "P";
        case Input::Key_Q:                 return "Q";
        case Input::Key_R:                 return "R";
        case Input::Key_S:                 return "S";
        case Input::Key_T:                 return "T";
        case Input::Key_U:                 return "U";
        case Input::Key_V:                 return "V";
        case Input::Key_W:                 return "W";
        case Input::Key_X:                 return "X";
        case Input::Key_Y:                 return "Y";
        case Input::Key_Z:                 return "Z";
        case Input::Key_Minus:             return "-";
        case Input::Key_Equal:             return "=";
        case Input::Key_Space:             return "Space";
        case Input::Key_Backspace:         return "Backspace";
        case Input::Key_LeftBracket:       return "[";
        case Input::Key_RightBracket:      return "]";
        case Input::Key_Backtick:          return "`";
        case Input::Key_Tab:               return "Tab";
        case Input::Key_Semicolon:         return ";";
        case Input::Key_Apostrophe:        return "'";
        case Input::Key_Comma:             return ",";
        case Input::Key_Period:            return ".";
        case Input::Key_Slash:             return "/";
        case Input::Key_Backslash:         return "\\";
        case Input::Key_Escape:            return "Escape";
        case Input::Key_Enter:             return "Enter";
        case Input::Key_LeftShift:         return "Left Shift";
        case Input::Key_RightShift:        return "Right Shift";
        case Input::Key_LeftControl:       return "Left Ctrl";
        case Input::Key_RightControl:      return "Right Ctrl";
        case Input::Key_LeftAlt:           return "Left Alt";
        case Input::Key_RightAlt:          return "Right Alt";
        case Input::Key_LeftSuper:         return "Left Super";
        case Input::Key_RightSuper:        return "Right Super";
        case Input::Key_PrintScreen:       return "Print Screen";
        case Input::Key_Pause:             return "Pause";
        case Input::Key_Insert:            return "Insert";
        case Input::Key_Delete:            return "Delete";
        case Input::Key_Home:              return "Home";
        case Input::Key_End:               return "End";
        case Input::Key_PageUp:            return "Page Up";
        case Input::Key_PageDown:          return "Page Down";
        case Input::Key_ArrowUp:           return "Arrow Up";
        case Input::Key_ArrowDown:         return "Arrow Down";
        case Input::Key_ArrowLeft:         return "Arrow Left";
        case Input::Key_ArrowRight:        return "Arrow Right";
        case Input::Key_CapsLock:          return "Caps Lock";
        case Input::Key_ScrollLock:        return "Scroll Lock";
        case Input::Key_NumLock:           return "Num Lock";
        case Input::Key_Numpad0:           return "Numpad 0";
        case Input::Key_Numpad1:           return "Numpad 1";
        case Input::Key_Numpad2:           return "Numpad 2";
        case Input::Key_Numpad3:           return "Numpad 3";
        case Input::Key_Numpad4:           return "Numpad 4";
        case Input::Key_Numpad5:           return "Numpad 5";
        case Input::Key_Numpad6:           return "Numpad 6";
        case Input::Key_Numpad7:           return "Numpad 7";
        case Input::Key_Numpad8:           return "Numpad 8";
        case Input::Key_Numpad9:           return "Numpad 9";
        case Input::Key_NumpadDecimal:     return "Numpad .";
        case Input::Key_NumpadEqual:       return "Numpad =";
        case Input::Key_NumpadEnter:       return "Numpad Enter";
        case Input::Key_NumpadDivide:      return "Numpad /";
        case Input::Key_NumpadMultiply:    return "Numpad *";
        case Input::Key_NumpadSubtract:    return "Numpad -";
        case Input::Key_NumpadAdd:         return "Numpad +";

        // Mouse
        case Input::Mouse_Button1:         return "Mouse button 1 (LMB)";
        case Input::Mouse_Button2:         return "Mouse button 2 (RMB)";
        case Input::Mouse_Button3:         return "Mouse button 3 (MMB)";
        case Input::Mouse_Button4:         return "Mouse button 4";
        case Input::Mouse_Button5:         return "Mouse button 5";
        case Input::Mouse_Button6:         return "Mouse button 6";
        case Input::Mouse_Button7:         return "Mouse button 7";
        case Input::Mouse_Button8:         return "Mouse button 8";

        // Special inputs don't have names
        default: return "[Unknown Input]";
    }
}

Input::Input Input::GlfwMacroToInput(int input) {
    switch (input) {
        // Keyboard
        case GLFW_KEY_F1:               return Input::Key_F1;
        case GLFW_KEY_F2:               return Input::Key_F2;
        case GLFW_KEY_F3:               return Input::Key_F3;
        case GLFW_KEY_F4:               return Input::Key_F4;
        case GLFW_KEY_F5:               return Input::Key_F5;
        case GLFW_KEY_F6:               return Input::Key_F6;
        case GLFW_KEY_F7:               return Input::Key_F7;
        case GLFW_KEY_F8:               return Input::Key_F8;
        case GLFW_KEY_F9:               return Input::Key_F9;
        case GLFW_KEY_F10:              return Input::Key_F10;
        case GLFW_KEY_F11:              return Input::Key_F11;
        case GLFW_KEY_F12:              return Input::Key_F12;
        case GLFW_KEY_F13:              return Input::Key_F13;
        case GLFW_KEY_F14:              return Input::Key_F14;
        case GLFW_KEY_F15:              return Input::Key_F15;
        case GLFW_KEY_F16:              return Input::Key_F16;
        case GLFW_KEY_F17:              return Input::Key_F17;
        case GLFW_KEY_F18:              return Input::Key_F18;
        case GLFW_KEY_F19:              return Input::Key_F19;
        case GLFW_KEY_F20:              return Input::Key_F20;
        case GLFW_KEY_F21:              return Input::Key_F21;
        case GLFW_KEY_F22:              return Input::Key_F22;
        case GLFW_KEY_F23:              return Input::Key_F23;
        case GLFW_KEY_F24:              return Input::Key_F24;
        case GLFW_KEY_F25:              return Input::Key_F25;
        case GLFW_KEY_0:                return Input::Key_0;
        case GLFW_KEY_1:                return Input::Key_1;
        case GLFW_KEY_2:                return Input::Key_2;
        case GLFW_KEY_3:                return Input::Key_3;
        case GLFW_KEY_4:                return Input::Key_4;
        case GLFW_KEY_5:                return Input::Key_5;
        case GLFW_KEY_6:                return Input::Key_6;
        case GLFW_KEY_7:                return Input::Key_7;
        case GLFW_KEY_8:                return Input::Key_8;
        case GLFW_KEY_9:                return Input::Key_9;
        case GLFW_KEY_A:                return Input::Key_A;
        case GLFW_KEY_B:                return Input::Key_B;
        case GLFW_KEY_C:                return Input::Key_C;
        case GLFW_KEY_D:                return Input::Key_D;
        case GLFW_KEY_E:                return Input::Key_E;
        case GLFW_KEY_F:                return Input::Key_F;
        case GLFW_KEY_G:                return Input::Key_G;
        case GLFW_KEY_H:                return Input::Key_H;
        case GLFW_KEY_I:                return Input::Key_I;
        case GLFW_KEY_J:                return Input::Key_J;
        case GLFW_KEY_K:                return Input::Key_K;
        case GLFW_KEY_L:                return Input::Key_L;
        case GLFW_KEY_M:                return Input::Key_M;
        case GLFW_KEY_N:                return Input::Key_N;
        case GLFW_KEY_O:                return Input::Key_O;
        case GLFW_KEY_P:                return Input::Key_P;
        case GLFW_KEY_Q:                return Input::Key_Q;
        case GLFW_KEY_R:                return Input::Key_R;
        case GLFW_KEY_S:                return Input::Key_S;
        case GLFW_KEY_T:                return Input::Key_T;
        case GLFW_KEY_U:                return Input::Key_U;
        case GLFW_KEY_V:                return Input::Key_V;
        case GLFW_KEY_W:                return Input::Key_W;
        case GLFW_KEY_X:                return Input::Key_X;
        case GLFW_KEY_Y:                return Input::Key_Y;
        case GLFW_KEY_Z:                return Input::Key_Z;
        case GLFW_KEY_MINUS:            return Input::Key_Minus;
        case GLFW_KEY_EQUAL:            return Input::Key_Equal;
        case GLFW_KEY_SPACE:            return Input::Key_Space;
        case GLFW_KEY_BACKSPACE:        return Input::Key_Backspace;
        case GLFW_KEY_LEFT_BRACKET:     return Input::Key_LeftBracket;
        case GLFW_KEY_RIGHT_BRACKET:    return Input::Key_RightBracket;
        case GLFW_KEY_GRAVE_ACCENT:     return Input::Key_Backtick;
        case GLFW_KEY_TAB:              return Input::Key_Tab;
        case GLFW_KEY_SEMICOLON:        return Input::Key_Semicolon;
        case GLFW_KEY_APOSTROPHE:       return Input::Key_Apostrophe;
        case GLFW_KEY_COMMA:            return Input::Key_Comma;
        case GLFW_KEY_PERIOD:           return Input::Key_Period;
        case GLFW_KEY_SLASH:            return Input::Key_Slash;
        case GLFW_KEY_BACKSLASH:        return Input::Key_Backslash;
        case GLFW_KEY_ESCAPE:           return Input::Key_Escape;
        case GLFW_KEY_ENTER:            return Input::Key_Enter;
        case GLFW_KEY_LEFT_SHIFT:       return Input::Key_LeftShift;
        case GLFW_KEY_RIGHT_SHIFT:      return Input::Key_RightShift;
        case GLFW_KEY_LEFT_CONTROL:     return Input::Key_LeftControl;
        case GLFW_KEY_RIGHT_CONTROL:    return Input::Key_RightControl;
        case GLFW_KEY_LEFT_ALT:         return Input::Key_LeftAlt;
        case GLFW_KEY_RIGHT_ALT:        return Input::Key_RightAlt;
        case GLFW_KEY_LEFT_SUPER:       return Input::Key_LeftSuper;
        case GLFW_KEY_RIGHT_SUPER:      return Input::Key_RightSuper;
        case GLFW_KEY_PRINT_SCREEN:     return Input::Key_PrintScreen;
        case GLFW_KEY_PAUSE:            return Input::Key_Pause;
        case GLFW_KEY_INSERT:           return Input::Key_Insert;
        case GLFW_KEY_DELETE:           return Input::Key_Delete;
        case GLFW_KEY_HOME:             return Input::Key_Home;
        case GLFW_KEY_END:              return Input::Key_End;
        case GLFW_KEY_PAGE_UP:          return Input::Key_PageUp;
        case GLFW_KEY_PAGE_DOWN:        return Input::Key_PageDown;
        case GLFW_KEY_UP:               return Input::Key_ArrowUp;
        case GLFW_KEY_DOWN:             return Input::Key_ArrowDown;
        case GLFW_KEY_LEFT:             return Input::Key_ArrowLeft;
        case GLFW_KEY_RIGHT:            return Input::Key_ArrowRight;
        case GLFW_KEY_CAPS_LOCK:        return Input::Key_CapsLock;
        case GLFW_KEY_SCROLL_LOCK:      return Input::Key_ScrollLock;
        case GLFW_KEY_NUM_LOCK:         return Input::Key_NumLock;
        case GLFW_KEY_KP_0:             return Input::Key_Numpad0;
        case GLFW_KEY_KP_1:             return Input::Key_Numpad1;
        case GLFW_KEY_KP_2:             return Input::Key_Numpad2;
        case GLFW_KEY_KP_3:             return Input::Key_Numpad3;
        case GLFW_KEY_KP_4:             return Input::Key_Numpad4;
        case GLFW_KEY_KP_5:             return Input::Key_Numpad5;
        case GLFW_KEY_KP_6:             return Input::Key_Numpad6;
        case GLFW_KEY_KP_7:             return Input::Key_Numpad7;
        case GLFW_KEY_KP_8:             return Input::Key_Numpad8;
        case GLFW_KEY_KP_9:             return Input::Key_Numpad9;
        case GLFW_KEY_KP_DECIMAL:       return Input::Key_NumpadDecimal;
        case GLFW_KEY_KP_EQUAL:         return Input::Key_NumpadEqual;
        case GLFW_KEY_KP_ENTER:         return Input::Key_NumpadEnter;
        case GLFW_KEY_KP_DIVIDE:        return Input::Key_NumpadDivide;
        case GLFW_KEY_KP_MULTIPLY:      return Input::Key_NumpadMultiply;
        case GLFW_KEY_KP_SUBTRACT:      return Input::Key_NumpadSubtract;
        case GLFW_KEY_KP_ADD:           return Input::Key_NumpadAdd;

        // Mouse
        case GLFW_MOUSE_BUTTON_1:       return Input::Mouse_Button1;
        case GLFW_MOUSE_BUTTON_2:       return Input::Mouse_Button2;
        case GLFW_MOUSE_BUTTON_3:       return Input::Mouse_Button3;
        case GLFW_MOUSE_BUTTON_4:       return Input::Mouse_Button4;
        case GLFW_MOUSE_BUTTON_5:       return Input::Mouse_Button5;
        case GLFW_MOUSE_BUTTON_6:       return Input::Mouse_Button6;
        case GLFW_MOUSE_BUTTON_7:       return Input::Mouse_Button7;
        case GLFW_MOUSE_BUTTON_8:       return Input::Mouse_Button8;

        // Special inputs don't have names
        default: return Input::None;
    }
}

int Input::InputToGlfwMacro(Input input) {
    switch (input) {
        // Keyboard
        case Input::Key_F1:                return GLFW_KEY_F1;
        case Input::Key_F2:                return GLFW_KEY_F2;
        case Input::Key_F3:                return GLFW_KEY_F3;
        case Input::Key_F4:                return GLFW_KEY_F4;
        case Input::Key_F5:                return GLFW_KEY_F5;
        case Input::Key_F6:                return GLFW_KEY_F6;
        case Input::Key_F7:                return GLFW_KEY_F7;
        case Input::Key_F8:                return GLFW_KEY_F8;
        case Input::Key_F9:                return GLFW_KEY_F9;
        case Input::Key_F10:               return GLFW_KEY_F10;
        case Input::Key_F11:               return GLFW_KEY_F11;
        case Input::Key_F12:               return GLFW_KEY_F12;
        case Input::Key_F13:               return GLFW_KEY_F13;
        case Input::Key_F14:               return GLFW_KEY_F14;
        case Input::Key_F15:               return GLFW_KEY_F15;
        case Input::Key_F16:               return GLFW_KEY_F16;
        case Input::Key_F17:               return GLFW_KEY_F17;
        case Input::Key_F18:               return GLFW_KEY_F18;
        case Input::Key_F19:               return GLFW_KEY_F19;
        case Input::Key_F20:               return GLFW_KEY_F20;
        case Input::Key_F21:               return GLFW_KEY_F21;
        case Input::Key_F22:               return GLFW_KEY_F22;
        case Input::Key_F23:               return GLFW_KEY_F23;
        case Input::Key_F24:               return GLFW_KEY_F24;
        case Input::Key_F25:               return GLFW_KEY_F25;
        case Input::Key_0:                 return GLFW_KEY_0;
        case Input::Key_1:                 return GLFW_KEY_1;
        case Input::Key_2:                 return GLFW_KEY_2;
        case Input::Key_3:                 return GLFW_KEY_3;
        case Input::Key_4:                 return GLFW_KEY_4;
        case Input::Key_5:                 return GLFW_KEY_5;
        case Input::Key_6:                 return GLFW_KEY_6;
        case Input::Key_7:                 return GLFW_KEY_7;
        case Input::Key_8:                 return GLFW_KEY_8;
        case Input::Key_9:                 return GLFW_KEY_9;
        case Input::Key_A:                 return GLFW_KEY_A;
        case Input::Key_B:                 return GLFW_KEY_B;
        case Input::Key_C:                 return GLFW_KEY_C;
        case Input::Key_D:                 return GLFW_KEY_D;
        case Input::Key_E:                 return GLFW_KEY_E;
        case Input::Key_F:                 return GLFW_KEY_F;
        case Input::Key_G:                 return GLFW_KEY_G;
        case Input::Key_H:                 return GLFW_KEY_H;
        case Input::Key_I:                 return GLFW_KEY_I;
        case Input::Key_J:                 return GLFW_KEY_J;
        case Input::Key_K:                 return GLFW_KEY_K;
        case Input::Key_L:                 return GLFW_KEY_L;
        case Input::Key_M:                 return GLFW_KEY_M;
        case Input::Key_N:                 return GLFW_KEY_N;
        case Input::Key_O:                 return GLFW_KEY_O;
        case Input::Key_P:                 return GLFW_KEY_P;
        case Input::Key_Q:                 return GLFW_KEY_Q;
        case Input::Key_R:                 return GLFW_KEY_R;
        case Input::Key_S:                 return GLFW_KEY_S;
        case Input::Key_T:                 return GLFW_KEY_T;
        case Input::Key_U:                 return GLFW_KEY_U;
        case Input::Key_V:                 return GLFW_KEY_V;
        case Input::Key_W:                 return GLFW_KEY_W;
        case Input::Key_X:                 return GLFW_KEY_X;
        case Input::Key_Y:                 return GLFW_KEY_Y;
        case Input::Key_Z:                 return GLFW_KEY_Z;
        case Input::Key_Minus:             return GLFW_KEY_MINUS;
        case Input::Key_Equal:             return GLFW_KEY_EQUAL;
        case Input::Key_Space:             return GLFW_KEY_SPACE;
        case Input::Key_Backspace:         return GLFW_KEY_BACKSPACE;
        case Input::Key_LeftBracket:       return GLFW_KEY_LEFT_BRACKET;
        case Input::Key_RightBracket:      return GLFW_KEY_RIGHT_BRACKET;
        case Input::Key_Backtick:          return GLFW_KEY_GRAVE_ACCENT;
        case Input::Key_Tab:               return GLFW_KEY_TAB;
        case Input::Key_Semicolon:         return GLFW_KEY_SEMICOLON;
        case Input::Key_Apostrophe:        return GLFW_KEY_APOSTROPHE;
        case Input::Key_Comma:             return GLFW_KEY_COMMA;
        case Input::Key_Period:            return GLFW_KEY_PERIOD;
        case Input::Key_Slash:             return GLFW_KEY_SLASH;
        case Input::Key_Backslash:         return GLFW_KEY_BACKSLASH;
        case Input::Key_Escape:            return GLFW_KEY_ESCAPE;
        case Input::Key_Enter:             return GLFW_KEY_ENTER;
        case Input::Key_LeftShift:         return GLFW_KEY_LEFT_SHIFT;
        case Input::Key_RightShift:        return GLFW_KEY_RIGHT_SHIFT;
        case Input::Key_LeftControl:       return GLFW_KEY_LEFT_CONTROL;
        case Input::Key_RightControl:      return GLFW_KEY_RIGHT_CONTROL;
        case Input::Key_LeftAlt:           return GLFW_KEY_LEFT_ALT;
        case Input::Key_RightAlt:          return GLFW_KEY_RIGHT_ALT;
        case Input::Key_LeftSuper:         return GLFW_KEY_LEFT_SUPER;
        case Input::Key_RightSuper:        return GLFW_KEY_RIGHT_SUPER;
        case Input::Key_PrintScreen:       return GLFW_KEY_PRINT_SCREEN;
        case Input::Key_Pause:             return GLFW_KEY_PAUSE;
        case Input::Key_Insert:            return GLFW_KEY_INSERT;
        case Input::Key_Delete:            return GLFW_KEY_DELETE;
        case Input::Key_Home:              return GLFW_KEY_HOME;
        case Input::Key_End:               return GLFW_KEY_END;
        case Input::Key_PageUp:            return GLFW_KEY_PAGE_UP;
        case Input::Key_PageDown:          return GLFW_KEY_PAGE_DOWN;
        case Input::Key_ArrowUp:           return GLFW_KEY_UP;
        case Input::Key_ArrowDown:         return GLFW_KEY_DOWN;
        case Input::Key_ArrowLeft:         return GLFW_KEY_LEFT;
        case Input::Key_ArrowRight:        return GLFW_KEY_RIGHT;
        case Input::Key_CapsLock:          return GLFW_KEY_CAPS_LOCK;
        case Input::Key_ScrollLock:        return GLFW_KEY_SCROLL_LOCK;
        case Input::Key_NumLock:           return GLFW_KEY_NUM_LOCK;
        case Input::Key_Numpad0:           return GLFW_KEY_KP_0;
        case Input::Key_Numpad1:           return GLFW_KEY_KP_1;
        case Input::Key_Numpad2:           return GLFW_KEY_KP_2;
        case Input::Key_Numpad3:           return GLFW_KEY_KP_3;
        case Input::Key_Numpad4:           return GLFW_KEY_KP_4;
        case Input::Key_Numpad5:           return GLFW_KEY_KP_5;
        case Input::Key_Numpad6:           return GLFW_KEY_KP_6;
        case Input::Key_Numpad7:           return GLFW_KEY_KP_7;
        case Input::Key_Numpad8:           return GLFW_KEY_KP_8;
        case Input::Key_Numpad9:           return GLFW_KEY_KP_9;
        case Input::Key_NumpadDecimal:     return GLFW_KEY_KP_DECIMAL;
        case Input::Key_NumpadEqual:       return GLFW_KEY_KP_EQUAL;
        case Input::Key_NumpadEnter:       return GLFW_KEY_KP_ENTER;
        case Input::Key_NumpadDivide:      return GLFW_KEY_KP_DIVIDE;
        case Input::Key_NumpadMultiply:    return GLFW_KEY_KP_MULTIPLY;
        case Input::Key_NumpadSubtract:    return GLFW_KEY_KP_SUBTRACT;
        case Input::Key_NumpadAdd:         return GLFW_KEY_KP_ADD;

        // Mouse
        case Input::Mouse_Button1:         return GLFW_MOUSE_BUTTON_1;
        case Input::Mouse_Button2:         return GLFW_MOUSE_BUTTON_2;
        case Input::Mouse_Button3:         return GLFW_MOUSE_BUTTON_3;
        case Input::Mouse_Button4:         return GLFW_MOUSE_BUTTON_4;
        case Input::Mouse_Button5:         return GLFW_MOUSE_BUTTON_5;
        case Input::Mouse_Button6:         return GLFW_MOUSE_BUTTON_6;
        case Input::Mouse_Button7:         return GLFW_MOUSE_BUTTON_7;
        case Input::Mouse_Button8:         return GLFW_MOUSE_BUTTON_8;

        // Special actions don't have GLWF macros
        default: return -1;
    }
}

} // namespace rf
