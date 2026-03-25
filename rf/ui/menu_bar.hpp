#pragma once

#include <rf/ui/window.hpp>

namespace rf {

namespace Ui {
    class MenuBar {
    private:
        Window::SharedList m_windows;
        bool m_allWindowsHidden = false;

    public:
        MenuBar(const Window::SharedList& windows);

    private:
        void updateShortcuts();

        void updateWindow();

    public:
        void update();

    public:
        bool areAllWindowsHidden() const {
            return m_allWindowsHidden;
        }
    };
}

} // namespace rf
