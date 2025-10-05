#include "bindings_hint.hpp"

#include <vector>
#include <utility>
#include <algorithm>

#include <rf/core/engine.hpp>
#include <rf/ui/imutil.hpp>

namespace rf {

struct Binding {
    Input::KeyBindingId id = 0;
    bool keyDown = false;
    std::string key;
    std::string description;
};

static std::vector<Binding> GetSortedBindings() {
    std::vector<Binding> bindings;
    const Input::InputMap& inputMap = Engine::InputMap();
    for (const auto& bind : inputMap.binds()) {
        const Input::KeyBinding* keyBinding = inputMap.getKeyBinding(bind.second.handle);
        bindings.emplace_back(keyBinding->id, inputMap.keyDown(bind.first), Input::GetKeyEntry(bind.first).name, keyBinding->description);
    }
    std::sort(bindings.begin(), bindings.end(), [](const Binding& left, const Binding& right) { return left.id < right.id; });
    return bindings;
}

struct FieldsWidths {
    float key = 0.0f;
    float description = 0.0f;
};

static FieldsWidths GetLongestFieldsWidths(const std::vector<Binding>& binding) {
    FieldsWidths fieldsWidths = {};
    for (const Binding& binding : binding) {
        fieldsWidths.key = std::max(fieldsWidths.key, ImGui::CalcTextSize(binding.key.c_str()).x);
        fieldsWidths.description = std::max(fieldsWidths.description, ImGui::CalcTextSize(binding.description.c_str()).x);
    }
    return fieldsWidths;
}

void Ui::BindingsHint::draw() const {
    if (ImGui::BeginTable("BindingsTable", 2, ImGuiTableFlags_Borders)) {
        std::vector<Binding> bindings = GetSortedBindings();
        FieldsWidths longestFieldsWidths = GetLongestFieldsWidths(bindings);

        ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, longestFieldsWidths.key, 0);
        ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthFixed, longestFieldsWidths.description, 1);
        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
        for (int column = 0; column < ImGui::TableGetColumnCount(); column++) {
            ImGui::TableSetColumnIndex(column);
            ImUtil::TextUnformattedShiftedRight(ImGui::TableGetColumnName(column), 0.5f);
        }

        for (const Binding& binding : bindings) {
            ImGui::TableNextRow();
            if (Engine::Window().getCursorMode() == Graphics::CursorMode::Disabled && binding.keyDown)
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::GetColorU32(ImGuiCol_TextSelectedBg));
            ImGui::TableNextColumn();
            ImUtil::TextUnformattedShiftedRight(binding.key.c_str(), 0.5f);
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(binding.description.c_str());
        }
        ImGui::EndTable();
    }
}

} // namespace rf
