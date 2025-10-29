#include "bindings_hint.hpp"

#include <vector>
#include <algorithm>

#include <rf/core/engine.hpp>
#include <rf/ui/imutil.hpp>

namespace rf {

struct Binding {
    Input::KeyBindingId id = 0;
    Input::Key key = Input::Key::None;
    std::string keyName;
    std::string description;
};

static std::vector<Binding> GetSortedBindings() {
    std::vector<Binding> bindings;
    const Input::InputMap& inputMap = Engine::InputMap();
    for (const auto& bind : inputMap.binds()) {
        const Input::KeyBinding* keyBinding = inputMap.getKeyBinding(bind.second.handle);
        bindings.emplace_back(keyBinding->id, bind.first, Input::GetKeyEntry(bind.first).name, keyBinding->description);
    }
    std::sort(bindings.begin(), bindings.end(), [](const Binding& left, const Binding& right) { return left.id < right.id; });
    return bindings;
}

struct LongestWidths {
    float keyName = 0.0f;
    float description = 0.0f;
};

static LongestWidths GetLongestWidths(const std::vector<Binding>& bindings) {
    LongestWidths longestWidths = {};
    for (const Binding& binding : bindings) {
        longestWidths.keyName = std::max(longestWidths.keyName, ImGui::CalcTextSize(binding.keyName.c_str()).x);
        longestWidths.description = std::max(longestWidths.description, ImGui::CalcTextSize(binding.description.c_str()).x);
    }
    return longestWidths;
}

void Ui::Windows::BindingsHint::update() {
    if (ImGui::BeginTable("##bindingshint_table", 2, ImGuiTableFlags_Borders)) {
        static std::vector<Binding> s_bindings = GetSortedBindings();
        static LongestWidths s_longestWidths = GetLongestWidths(s_bindings);

        ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, s_longestWidths.keyName, 0);
        ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthFixed, s_longestWidths.description, 1);
        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
        for (int column = 0; column < ImGui::TableGetColumnCount(); column++) {
            ImGui::TableSetColumnIndex(column);
            ImUtil::TextShifted(ImGui::TableGetColumnName(column), 0.5f);
        }

        for (const Binding& binding : s_bindings) {
            ImGui::TableNextRow();
            if (Engine::Window().getCursorMode() == Graphics::CursorMode::Disabled && Engine::InputMap().keyDown(binding.key))
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::GetColorU32(ImGuiCol_TextSelectedBg));
            ImGui::TableNextColumn();
            ImUtil::TextShifted(binding.keyName.c_str(), 0.5f);
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(binding.description.c_str());
        }
        ImGui::EndTable();
    }
}

} // namespace rf
