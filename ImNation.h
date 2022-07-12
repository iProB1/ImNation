#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
// LOADING CIRCLE
//fore_color(200, 20, 20, 255)
//back_color(200, 20, 20, 40)
//arc_size = 0.45f
//radius = 35.f
//float thickness = 4.f

//const ImU32 col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
//const ImU32 bg = ImGui::GetColorU32(ImGuiCol_Button);
//ImNation::LoadingCircle1("##spinner circle", 15, 6, col);
//ImNation::LoadingBar("##loadingbar", 0.7f, ImVec2(400, 6), bg, col);

//ImNation::BoxProgressBar(ImGui::GetTime() * -0.2f, ImVec2(0, 0), "Loading...");:

namespace ImNation {
	void BoxProgressBar(const ImVec2& size_arg);
	bool LoadingBar(const char* label, float value,  const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col);
	bool LoadingCircle1(const char* label, float radius, int thickness, const ImU32& color);
    void LoadingCircle(const ImVec4& fore_color, const ImVec4& back_color, float& arc_size, float& radius, float thickness);
	void ChildSizex(float& ChildSizex, float& ChildSizex2, int tosize = 20, float& Speed = 1);
	void ChildSizey(float& ChildSizey, float& ChildSizey2, int tosize = 20, float& Speed = 1);
	void ChildSizex(float& ChildSizex, int tosize = 20, float& Speed = 1);
	void ChildSizey(float& ChildSizey, int tosize = 20, float& Speed = 1);
	void TextColored(int R, int G, int B, const char* label, float& Alpha = 255, bool tick = false, float& MaxValue = 255, float& Speed = 1.0f, ...);
    bool Button(const char* label, const ImVec2& size = ImVec2(0, 0));
}
