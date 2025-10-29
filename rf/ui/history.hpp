#pragma once

#include <vector>
#include <utility>

namespace rf {

namespace Ui {
    template <typename Item>
    class History {
    private:
        std::vector<Item> m_history;
        size_t m_offset = 0;

    public:
        History(const Item& first) {
            m_history.push_back(first);
        }

        History(const History& other) = default;

        History(History&& other) noexcept
            : m_history(std::move(other.m_history))
            , m_offset(std::exchange(other.m_offset, 0))
        {}

        ~History() = default;

    public:
        History& operator=(const History& other) = default;

        History& operator=(History&& other) noexcept {
            m_history = std::move(other.m_history);
            m_offset = std::exchange(other.m_offset, 0);
        }

    public:
        void add(const Item& item) {
            constexpr size_t MaxSize = 1000;
            if (m_history.at(m_history.size() - m_offset - 1) != item) {
                if (m_offset != 0) {
                    m_history.erase(m_history.end() - m_offset, m_history.end());
                    m_offset = 0;
                }

                m_history.push_back(item);
                if (m_history.size() > MaxSize)
                    m_history.erase(m_history.begin(), m_history.begin() + (m_history.size() - MaxSize));
            }
        }

        bool canGoBack() const {
            return m_history.size() != 1 && m_offset + 1 != m_history.size();
        }

        bool canGoForward() const {
            return m_history.size() != 1 && m_offset != 0;
        }

        const Item* back() {
            if (!canGoBack())
                return nullptr;
            return &m_history.at(m_history.size() - ++m_offset - 1);
        }

        const Item* forward() {
            if (!canGoForward())
                return nullptr;
            return &m_history.at(m_history.size() - --m_offset - 1);
        }


    public:
        const std::vector<Item>& history() const {
            return m_history;
        }

        size_t offset() const {
            return m_offset;
        }
    };
}

} // namespace rf
