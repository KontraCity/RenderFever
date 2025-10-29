#include "perfomance_monitor.hpp"

#include <vector>
#include <algorithm>

#include <fmt/format.h>

#include <rf/core/engine.hpp>
#include <rf/ui/font.hpp>
#include <rf/ui/imutil.hpp>

namespace rf {

struct Record {
    float time = 0.0f;  // in seconds
    float fps = 0.0f;
    float ft = 0.0f;    // in milliseconds
};

Record& operator+=(Record& left, const Record& right) {
    left.time += right.time;
    left.fps += right.fps;
    left.ft += right.ft;
    return left;
}

template <typename Scalar>
Record& operator/=(Record& left, Scalar right) {
    left.time /= right;
    left.fps /= right;
    left.ft /= right;
    return left;
}

struct UpdateInfo {
    const std::vector<Record>& history;
    Record lastRecord = {};
};

static UpdateInfo MakeRecord() {
    constexpr float HistoryInterval = 5.0f;
    constexpr float RecordInterval  = 0.05f;

    static std::vector<Record> s_history;
    static std::vector<Record> s_accomulator;
    static float s_lastUpdateTime = 0.0f;

    const Engine::FrameInfo& frame = Engine::GetFrameInfo();
    std::erase_if(s_history, [&frame](const Record& record) {
        return (frame.time - record.time) > HistoryInterval;
    });

    if (!s_accomulator.empty() && (frame.time - s_lastUpdateTime) > RecordInterval) {
        Record meanRecord = {};
        for (const Record& record : s_accomulator)
            meanRecord += record;
        meanRecord /= s_accomulator.size();

        s_accomulator.clear();
        s_history.push_back(meanRecord);
        s_lastUpdateTime = frame.time;
    }
    else {
        // First few frames contain inaccurate delta time, ignore them.
        if (frame.index >= 5) {
            s_accomulator.push_back({
                .time = frame.time,
                .fps  = 1.0f / frame.deltaTime,
                .ft   = frame.deltaTime * 1000
            });
        }
    }

    return {
        .history = s_history,
        .lastRecord = s_history.empty() ? Record{} : s_history.at(s_history.size() - 1),
    };
}

struct MaxValues {
    float fps = 0.0f;
    float ft = 0.0f;
};

static MaxValues GetMaxValues(const std::vector<Record>& records) {
    MaxValues maxValues = {};
    for (const Record& record : records) {
        maxValues.fps = std::max(maxValues.fps, record.fps);
        maxValues.ft = std::max(maxValues.ft, record.ft);
    }
    return maxValues;
}

static float LinearInterpolation(float previous, float current, float riseSpeed, float fallSpeed) {
    float speed = (current > previous) ? riseSpeed : fallSpeed;
    return previous + (current - previous) * speed;
}

static MaxValues GetMaxPlotScales(const std::vector<Record>& records) {
    constexpr float FpsPlotRiseSpeed = 0.001f;
    constexpr float FpsPlotFallSpeed = 0.05f;
    constexpr float FtPlotRiseSpeed  = 0.01f;
    constexpr float FtPlotFallSpeed  = 0.005f;

    MaxValues currentMaxValues = GetMaxValues(records);
    static MaxValues s_previousMaxValues = currentMaxValues;

    MaxValues maxPlotScales = {};
    maxPlotScales.fps = LinearInterpolation(s_previousMaxValues.fps, currentMaxValues.fps, FpsPlotRiseSpeed, FpsPlotFallSpeed);
    maxPlotScales.ft = LinearInterpolation(s_previousMaxValues.ft, currentMaxValues.ft, FtPlotRiseSpeed, FtPlotFallSpeed);

    s_previousMaxValues = maxPlotScales;
    return maxPlotScales;
}

void Ui::Windows::PerfomanceMonitor::update() {
    constexpr float RowHeight = FontSize(Font::Normal) + FontSize(Font::Huge);
    UpdateInfo info = MakeRecord();
    MaxValues maxPlotScales = GetMaxPlotScales(info.history);

    if (ImGui::BeginTable("##perfomancemon_fps_table", 2, ImGuiTableFlags_SizingStretchSame)) {
        ImGui::TableSetupColumn("##perfomancemon_fps_table_plot", ImGuiTableColumnFlags_WidthFixed, 200.0f);
        ImGui::TableSetupColumn("##perfomancemon_fps_table_text", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableNextRow(ImGuiTableRowFlags_None, RowHeight);

        ImGui::TableSetColumnIndex(0);
        ImGui::PlotLines(
            "##perfomancemon_fps_plot",
            [](void* data, int index) { return (reinterpret_cast<Record*>(data) + index)->fps; },
            const_cast<void*>(reinterpret_cast<const void*>(info.history.data())),
            info.history.size(), 0, nullptr, 0, maxPlotScales.fps,
            ImVec2(200, RowHeight + ImGui::GetStyle().CellPadding.y * 2)
        );

        ImGui::TableSetColumnIndex(1);
        ImGui::BeginGroup();
        ImUtil::TextShifted("Frames Per Second", 1.0f);
        WithFont(Font::Huge, [this, &info]() {
            std::string text = fmt::format("{:.1f}", info.lastRecord.fps);
            ImUtil::TextShifted(text.c_str(), 1.0f);
        });
        ImGui::EndGroup();

        ImGui::EndTable();
    }

    if (ImGui::BeginTable("##perfomancemon_ft_table", 2, ImGuiTableFlags_SizingStretchSame)) {
        ImGui::TableSetupColumn("##perfomancemon_ft_table_plot", ImGuiTableColumnFlags_WidthFixed, 200.0f);
        ImGui::TableSetupColumn("##perfomancemon_ft_table_text", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableNextRow(ImGuiTableRowFlags_None, RowHeight);

        ImGui::TableSetColumnIndex(0);
        ImGui::PlotLines(
            "##perfomancemon_ft_plot",
            [](void* data, int index) { return (reinterpret_cast<Record*>(data) + index)->ft; },
            const_cast<void*>(reinterpret_cast<const void*>(info.history.data())),
            info.history.size(), 0, nullptr, 0, maxPlotScales.ft,
            ImVec2(200, RowHeight + ImGui::GetStyle().CellPadding.y * 2)
        );

        ImGui::TableSetColumnIndex(1);
        ImGui::BeginGroup();
        ImUtil::TextShifted("Frame Time", 1.0f);
        WithFont(Font::Huge, [this, &info]() {
            std::string text = fmt::format(fmt::runtime(
                info.lastRecord.ft < 99.95f ? "{:.1f} ms" : "{:.0f} ms"
            ), info.lastRecord.ft);
            ImUtil::TextShifted(text.c_str(), 1.0f);
        });
        ImGui::EndGroup();

        ImGui::EndTable();
    }
}

} // namespace rf
