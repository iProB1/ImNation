#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
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
    void LoadingCircle(const ImVec4& fore_color, const ImVec4& back_color, float arc_size, float radius, float thickness);
	void ChildSizex(int ChildSizex, int ChildSizex2, int tosize = 20, int Speed = 1);
	void ChildSizey(int ChildSizey, int ChildSizey2, int tosize = 20, int Speed = 1);
	void ChildSizex(int ChildSizex, int tosize = 20, int Speed = 1);
	void ChildSizey(int ChildSizey, int tosize = 20, int Speed = 1);
	void TextColored(int R, int G, int B, const char* label, float Alpha = 255.0f, bool tick = false, int MaxValue = 255, int Speed = 1, ...);
    void ToggleButton(const char* str_id, bool* v);
	bool Checkbox(const char* name, bool* callback);
	bool Button(const char* name, const ImVec2& size = ImVec2(0, 0));
}