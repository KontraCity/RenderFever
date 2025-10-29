#include "resource_browser.hpp"

#include <rf/auxiliary/fs.hpp>

#include <rf/core/engine.hpp>
#include <rf/ui/context_menus.hpp>
#include <rf/ui/drag_drop_types.hpp>
#include <rf/ui/hints.hpp>
#include <rf/ui/history.hpp>
#include <rf/ui/imutil.hpp>
#include <rf/ui/styles.hpp>
#include <rf/ui/tooltips.hpp>
using namespace rf::Ui;

namespace rf {

enum class FileType {
    Unknown,
    ShaderExtention,

    Directory,
    Mesh,
    Shader,
    Texture,
};

static FileType GetFileType(const fs::path& path) {
    fs::path extention = path.extension();
    if (extention == ".glsl" || extention == ".vert" || extention == ".geom" || extention == ".frag")
        return FileType::ShaderExtention;
    if (extention == ".glb")
        return FileType::Mesh;
    if (extention == ".rfs")
        return FileType::Shader;
    if (extention == ".jpg" || extention == ".png")
        return FileType::Texture;
    return fs::is_directory(path) ? FileType::Directory : FileType::Unknown;
}

static ImTextureID GetFileTypeIcon(FileType type, bool tiny) {
    const IconMap& iconMap = Engine::Overlay().iconMap();
    switch (type) {
        case FileType::ShaderExtention: return tiny ? iconMap.tinyFile().handle()       : iconMap.regularFile().handle();
        case FileType::Directory:       return tiny ? iconMap.tinyDirectory().handle()  : iconMap.regularDirectory().handle();
        case FileType::Mesh:            return tiny ? iconMap.tinyCube().handle()       : iconMap.regularCube().handle();
        case FileType::Shader:          return tiny ? iconMap.tinyGear().handle()       : iconMap.regularGear().handle();
        case FileType::Texture:         return tiny ? iconMap.tinyImage().handle()      : iconMap.regularImage().handle();
        default:                        return tiny ? iconMap.tinyQuestion().handle()   : iconMap.regularQuestion().handle();
    }
}

static bool IsFileTypeLoadable(FileType type) {
    switch (type) {
        default:
            return false;
        case FileType::Directory:
        case FileType::Shader:
        case FileType::Texture:
        case FileType::Mesh:
            return true;
    }
}

static ImTextureID GetFileTypePreviewIcon(FileType type, const fs::path& resourcePath) {
    Resources::Library& library = Engine::Library();
    if (type == FileType::Shader) {
        Resources::Shader shader = library.shaders().get(resourcePath);
        if (shader)
            return Engine::Overlay().previewMap().getShaderPreview(shader).handle();
    }
    else if (type == FileType::Texture) {
        Resources::Texture texture = library.textures().get(resourcePath);
        if (texture)
            return texture->handle();
    }
    else if (type == FileType::Mesh) {
        Resources::Mesh mesh = library.meshes().get(resourcePath);
        if (mesh)
            return Engine::Overlay().previewMap().getMeshPreview(mesh).handle();
    }
    return GetFileTypeIcon(type, false);
}

using PathHistory = History<fs::path>;

static fs::path GetCurrentDirectory(const fs::path& path) {
    FileType fileType = GetFileType(path);
    return fileType == FileType::Directory ? path : path.parent_path();
}

static bool DoesPathContain(const fs::path& haystack, const fs::path& needle) {
    fs::path relative = haystack.lexically_relative(needle);
    return !relative.empty() && *relative.begin() != "..";
}

static void SelectPath(const fs::path& path, fs::path& selectedPath, PathHistory& history) {
    selectedPath = path;
    fs::path directory = GetCurrentDirectory(selectedPath);
    if (GetFileType(directory) == FileType::Directory)
        history.add(directory);
}

static void DrawResourceContext(FileType fileType, const fs::path& resourcePath, bool touch = false) {
    Resources::Library& library = Engine::Library();
    if (fileType == FileType::Shader) {
        Resources::Shader shader = library.shaders().get(resourcePath);
        ContextMenus::DrawShaderContextMenu(shader, false, resourcePath);
        
        if (touch && shader)
            library.reloadShader(shader);
        else if (touch && !shader)
            library.loadShader(resourcePath);

        if (shader && !ImUtil::SendDragDropPayload(DragDropTypes::Shader, shader, [&shader]() { Hints::DrawShaderHint(shader); }))
            Tooltips::DrawShaderTooltip(shader);
        else if (!shader)
            Tooltips::DrawUnloadedResourceTooltip(resourcePath, "shader");
    }
    else if (fileType == FileType::Texture) {
        Resources::Texture texture = library.textures().get(resourcePath);
        ContextMenus::DrawTextureContextMenu(texture, false, resourcePath);

        if (touch && texture)
            library.reloadTexture(texture);
        else if (touch && !texture)
            library.loadTexture(resourcePath);

        if (texture && !ImUtil::SendDragDropPayload(DragDropTypes::Texture, texture, [&texture]() { Hints::DrawTextureHint(texture); }))
            Tooltips::DrawTextureTooltip(texture);
        else if (!texture)
            Tooltips::DrawUnloadedResourceTooltip(resourcePath, "texture");
    }
    else if (fileType == FileType::Mesh) {
        Resources::Mesh mesh = library.meshes().get(resourcePath);
        ContextMenus::DrawMeshContextMenu(mesh, false, resourcePath);

        if (touch && mesh)
            library.reloadMesh(mesh);
        else if (touch && !mesh)
            library.loadMesh(resourcePath);

        if (mesh && !ImUtil::SendDragDropPayload(DragDropTypes::Mesh, mesh, [&mesh]() { Hints::DrawMeshHint(mesh); }))
            Tooltips::DrawMeshTooltip(mesh);
        else if (!mesh)
            Tooltips::DrawUnloadedResourceTooltip(resourcePath, "mesh");
    }
}

static ImGuiTreeNodeFlags GetTreeNodeFlags(bool selected, bool leaf) {
    ImGuiTreeNodeFlags flags = 0;
    flags |= ImGuiTreeNodeFlags_SpanFullWidth;
    flags |= ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
    flags |= selected ? ImGuiTreeNodeFlags_Selected : 0;
    flags |= leaf ? ImGuiTreeNodeFlags_Leaf : 0;
    flags |= leaf ? ImGuiTreeNodeFlags_NoTreePushOnOpen : 0;
    return flags;
}

static bool DrawTreeNodeWithIcon(const char* label, ImGuiTreeNodeFlags flags, ImTextureID icon, ImVec2 iconSize) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext* context = ImGui::GetCurrentContext();
    const ImGuiStyle& style = context->Style;
    ImVec2 cursor = ImGui::GetCursorScreenPos();

    ImGui::PushID(label);
    bool open = ImGui::TreeNodeEx("", flags);
    ImGui::PopID();

    float spacing = style.ItemInnerSpacing.x;
    float arrowSize = context->FontSize;
    ImVec2 textSize = ImGui::CalcTextSize(label);

    ImVec2 imagePosition(cursor.x + arrowSize + spacing, cursor.y);
    ImVec2 imageEnd(imagePosition.x + iconSize.x, imagePosition.y + iconSize.y);
    ImVec2 textPosition(imageEnd.x + spacing, cursor.y + (iconSize.y - textSize.y) * 0.5f);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddImage(icon, imagePosition, imageEnd);
    drawList->AddText(textPosition, ImGui::GetColorU32(ImGuiCol_Text), label);

    return open;
}

static bool DrawFileTreeNode(const fs::path& path, fs::path& selectedPath, PathHistory& history, FileType fileType, bool shouldOpen = false) {
    if (shouldOpen)
        ImGui::SetNextItemOpen(true, ImGuiCond_Always);

    ImGui::BeginDisabled(!IsFileTypeLoadable(fileType));
    bool open = DrawTreeNodeWithIcon(
        path.filename().string().c_str(),
        GetTreeNodeFlags(path == selectedPath, fileType != FileType::Directory),
        GetFileTypeIcon(fileType, true),
        ImVec2(14, 14)
    );
    ImGui::EndDisabled();

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        SelectPath(path, selectedPath, history);
    DrawResourceContext(fileType, fs::relative(path, Engine::Library().rootDirectory()));
    return open;
}

static void IterateDirectory(const fs::path& path, fs::path& selectedPath, PathHistory& history, bool shouldOpen) {
    for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
        FileType fileType = GetFileType(entry);
        if (fileType == FileType::Directory) {
            shouldOpen = shouldOpen ? DoesPathContain(selectedPath, entry.path()) : false;
            if (DrawFileTreeNode(entry.path(), selectedPath, history, fileType, shouldOpen)) {
                IterateDirectory(entry.path(), selectedPath, history, shouldOpen);
                ImGui::TreePop();
            }
        }
    }

    for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
        FileType fileType = GetFileType(entry);
        if (fileType != FileType::Directory)
            DrawFileTreeNode(entry.path(), selectedPath, history, fileType, false);
    }
}

static bool DrawSelectableWithIcon(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size, ImTextureID icon) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext* context = ImGui::GetCurrentContext();
    const ImGuiStyle& style = context->Style;
    ImVec2 cursor = ImGui::GetCursorScreenPos();

    ImGui::PushID(label);
    selected = ImGui::Selectable("", selected, flags, size);
    ImGui::PopID();

    float spacing = style.ItemInnerSpacing.x;
    float fontSize = context->FontSize;
    std::string text = Ui::ImUtil::TruncateText(label, size.x);
    ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
    ImVec2 iconSize(size.x - fontSize, size.y - fontSize);

    ImVec2 imagePosition(cursor.x + fontSize * 0.5f, cursor.y);
    ImVec2 imageEnd(imagePosition.x + iconSize.x, imagePosition.y + iconSize.y);
    ImVec2 textPosition(cursor.x + (size.y - textSize.x) * 0.5f, imageEnd.y);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddImage(icon, imagePosition, imageEnd);
    drawList->AddText(textPosition, ImGui::GetColorU32(ImGuiCol_Text), text.c_str());

    return selected;
}

static bool DrawFileSelectable(const fs::path& path, fs::path& selectedPath, PathHistory& history, FileType fileType, ImVec2 size) {
    fs::path resourcePath = fs::relative(path, Engine::Library().rootDirectory());

    ImGui::BeginDisabled(!IsFileTypeLoadable(fileType));
    bool selected = DrawSelectableWithIcon(
        path.filename().string().c_str(),
        path == selectedPath,
        ImGuiSelectableFlags_None,
        size,
        GetFileTypePreviewIcon(fileType, resourcePath)
    );
    bool doubleClicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
    ImGui::EndDisabled();

    if (selected)
        SelectPath(path, selectedPath, history);
    DrawResourceContext(fileType, resourcePath, doubleClicked);
    return selected;
}

static void ShowNavigation(fs::path& directory, fs::path& selectedPath, PathHistory& history) {
    const rf::Resources::Library& library = rf::Engine::Library();
    const IconMap& iconMap = Engine::Overlay().iconMap();

    Styles::WithColorStyle(Styles::InvisibleButton, [&]() {
        ImGui::BeginDisabled(!history.canGoBack());
        if (ImGui::ImageButton("Left", iconMap.tinyLeft().handle(), ImVec2(14, 14))) {
            selectedPath = *history.back();
            directory = selectedPath;
        }
        ImGui::EndDisabled();
        ImGui::SameLine(0, 0);

        ImGui::BeginDisabled(!history.canGoForward());
        if (ImGui::ImageButton("Right", iconMap.tinyRight().handle(), ImVec2(14, 14))) {
            selectedPath = *history.forward();
            directory = selectedPath;
        }
        ImGui::EndDisabled();
        ImGui::SameLine();

        ImGui::BeginDisabled(directory == library.rootDirectory());
        if (ImGui::ImageButton("Up", iconMap.tinyUp().handle(), ImVec2(14, 14))) {
            selectedPath = directory.parent_path();
            directory = selectedPath;
            history.add(directory);
        }
        ImGui::EndDisabled();
        ImGui::SameLine();
    });

    fs::path accomulated;
    for (const fs::path& path : directory) {
        accomulated /= path;

        bool rootDirectory = accomulated == library.rootDirectory(), clicked = false;
        Styles::WithColorStyle(rootDirectory ? Styles::None : Styles::InvisibleButton, [&]() {
            clicked = ImGui::Button(fmt::format(
                "{}##{}", rootDirectory ? "Resources" : path.string().c_str(), accomulated.string()
            ).c_str());
        });

        if (clicked) {
            selectedPath = accomulated;
            directory = accomulated;
            history.add(directory);
            break;
        }

        ImGui::SameLine();
        ImGui::TextUnformatted("/");
        if (accomulated != directory)
            ImGui::SameLine();
    }
}

static void ShowDirectory(const fs::path& directory, fs::path& selectedPath, PathHistory& history, bool& shouldOpen) {
    if (fs::is_empty(directory)) {
        ImUtil::TextShifted("This directory is empty", 0.5f, 0.5f);
        return;
    }

    constexpr float ItemSize = 100.0f;
    constexpr float Spacing = 8.0f;
    ImVec2 startPosition = ImGui::GetCursorScreenPos();
    float availableWidth = ImGui::GetContentRegionAvail().x;

    std::vector<fs::directory_entry> items;
    for (const fs::directory_entry& entry : fs::directory_iterator(directory))
        if (GetFileType(entry) == FileType::Directory)
            items.push_back(entry);
    for (const fs::directory_entry& entry : fs::directory_iterator(directory))
        if (GetFileType(entry) != FileType::Directory)
            items.push_back(entry);

    int itemCount = static_cast<int>(items.size());
    int columnsCount = std::max(static_cast<int>(availableWidth / (ItemSize + Spacing)), 1);
    int rowsCount = static_cast<int>(std::ceil(static_cast<float>(itemCount) / columnsCount));
    for (int row = 0, itemIndex = 0; row < rowsCount && itemIndex < itemCount; ++row) {
        for (int column = 0; column < columnsCount && itemIndex < itemCount; ++column, ++itemIndex) {
            const fs::directory_entry& entry = items[itemIndex];
            FileType fileType = GetFileType(entry);

            ImGui::SetCursorScreenPos(ImVec2(startPosition.x + (ItemSize + Spacing) * column, startPosition.y + (ItemSize + Spacing) * row));
            if (DrawFileSelectable(entry.path(), selectedPath, history, fileType, ImVec2(ItemSize, ItemSize)))
                shouldOpen = fileType == FileType::Directory;
        }
    }
}

void Ui::Windows::ResourceBrowser::update() {
    const IconMap& iconMap = Engine::Overlay().iconMap();
    const rf::Resources::Library& library = rf::Engine::Library();

    static PathHistory s_history(library.rootDirectory());
    static fs::path s_selectedPath = library.rootDirectory();
    static bool s_shouldOpen = true;

    ImGui::BeginChild("##resourcebrowser_left_pane", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
    {
        if (s_shouldOpen)
            ImGui::SetNextItemOpen(true, ImGuiCond_Always);
        bool open = ImGui::CollapsingHeader("Resources", GetTreeNodeFlags(s_selectedPath == library.rootDirectory(), false));
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            SelectPath(library.rootDirectory(), s_selectedPath, s_history);

        if (open) {
            IterateDirectory(library.rootDirectory(), s_selectedPath, s_history, s_shouldOpen);
            s_shouldOpen = false;
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("##resourcebrowser_directory_view", ImVec2(0, 0));
    {
        fs::path directory = GetCurrentDirectory(s_selectedPath);
        ShowNavigation(directory, s_selectedPath, s_history);
        ImGui::Separator();
        ShowDirectory(directory, s_selectedPath, s_history, s_shouldOpen);
    }
    ImGui::EndChild();
}

} // namespace rf
