#include "ImNation.h"
// name == label
struct button_animation {
    bool clicked;
    bool reverse;
    float size;
    float mult;
    ImVec2 pos;
};
void ImNation::BoxProgressBar(const ImVec2& size_arg)
{
    using namespace ImGui;

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiStyle& style = g.Style;
    ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f);
    ImVec2 pos = window->DC.CursorPos;
    ImRect bb(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
    ItemSize(size);
    if (!ItemAdd(bb, 0))
        return;

    const float speed = g.FontSize * 0.05f;
    const float phase = ImFmod((float)g.Time * speed, 1.0f);
    const float width_normalized = 0.2f;
    float t0 = phase * (1.0f + width_normalized) - width_normalized;
    float t1 = t0 + width_normalized;

    RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
    bb.Expand(ImVec2(-style.FrameBorderSize, -style.FrameBorderSize));
    RenderRectFilledRangeH(window->DrawList, bb, GetColorU32(ImGuiCol_PlotHistogram), t0, t1, style.FrameRounding);
}
bool ImNation::LoadingBar(const char* label, float* value,  const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col) {
    ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;
        
        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = size_arg;
        size.x -= style.FramePadding.x * 2;
        
        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;
        
        // Render
        const float circleStart = size.x * 0.7f;
        const float circleEnd = size.x;
        const float circleWidth = circleEnd - circleStart;
        
        window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
        window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart*value, bb.Max.y), fg_col);
        
        const float t = g.Time;
        const float r = size.y / 2;
        const float speed = 1.5f;
        
        const float a = speed*0;
        const float b = speed*0.333f;
        const float c = speed*0.666f;
        
        const float o1 = (circleWidth+r) * (t+a - speed * (int)((t+a) / speed)) / speed;
        const float o2 = (circleWidth+r) * (t+b - speed * (int)((t+b) / speed)) / speed;
        const float o3 = (circleWidth+r) * (t+c - speed * (int)((t+c) / speed)) / speed;
        
        window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
        window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
        window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);
}

bool ImNation::LoadingCircle1(const char* label, float* radius, int* thickness, const ImU32& color) 
{
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;
        
        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        
        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size((radius )*2, (radius + style.FramePadding.y)*2);
        
        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;
        
        // Render
        window->DrawList->PathClear();
        
        int num_segments = 30;
        int start = abs(ImSin(g.Time*1.8f)*(num_segments-5));
        
        const float a_min = IM_PI*2.0f * ((float)start) / (float)num_segments;
        const float a_max = IM_PI*2.0f * ((float)num_segments-3) / (float)num_segments;

        const ImVec2 centre = ImVec2(pos.x+radius, pos.y+radius+style.FramePadding.y);
        
    for (int i = 0; i < num_segments; i++) 
	{
            const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a+g.Time*8) * radius,
                                                centre.y + ImSin(a+g.Time*8) * radius));
    }

        window->DrawList->PathStroke(color, false, thickness);
}
void ImNation::LoadingCircle(const ImVec4& fore_color, const ImVec4& back_color, float* arc_size, float* radius, float* thickness)
{
	auto front = ImGui::GetForegroundDrawList(); // also you can use GetWindowDrawList() or GetBackgroundDrawList()
    ImVec2 center = ImGui::GetIO().DisplaySize / 2.f;
    // Animation
    static float position = 0.f;
    position = ImLerp(position, IM_PI * 2.f, ImGui::GetIO().DeltaTime * 2.3f);

    // Background
    front->PathClear();
    front->PathArcTo(center, radius, 0.f, 2.f * IM_PI, 40.f);
    front->PathStroke(ImGui::GetColorU32(back_color.Value), 0, thickness);

    // Foreground
    front->PathClear();
    front->PathArcTo(center, radius, IM_PI * 1.5f + position, IM_PI * (1.5f + arc_size) + position, 40.f);
    front->PathStroke(ImGui::GetColorU32(fore_color.Value), 0, thickness);

    // Reset animation
    if (position >= IM_PI * 1.90f)
        position = 0.f;
}
void ImNation::ChildSizex(float* ChildSizex, float* ChildSizex2, int* tosize, float* Speed){
	if (ImGui::IsItemHovered())
	{
	    if (!(ChildSizex >= ChildSizex + tosize)){
			ChildSizex += Speed;
			ChildSizex2 -= Speed;
		}
	} else {
		if (ChildSizex != ChildSizex)
			ChildSizex -= Speed;
		    ChildSizex2 += Speed;
	}
}
void ImNation::ChildSizey(float* ChildSizey, float* ChildSizey2, int* tosize, float* Speed){
	if (ImGui::IsItemHovered())
	{
	    if (!(ChildSizey >= ChildSizey + tosize)){
			ChildSizey += Speed;
			ChildSizey2 -= Speed;
		}
	} else {
		if (ChildSizey != ChildSizey)
			ChildSizey -= Speed;
		    ChildSizey2 += Speed;
	}
}
void ImNation::ChildSizex(float* ChildSizex, int* tosize, float* Speed){
	if (ImGui::IsItemHovered())
	{
		if (!(ChildSizex >= ChildSizex + tosize))
			ChildSizex += Speed;
	} else {
		if (ChildSizex != ChildSizex)
			ChildSizex -= Speed;
	}
}
void ImNation::ChildSizey(float* ChildSizey, int* tosize, float* Speed){
	if (ImGui::IsItemHovered())
	{
		if (!(ChildSizey >= ChildSizey + tosize))
			ChildSizey += Speed;
	} else {
		if (ChildSizey != ChildSizey)
			ChildSizey -= Speed;
	}
}
void ImNation::TextColored(int* R, int* G, int* B, const char* label, float* Alpha, bool* tick, float* MaxValue, float* Speed, ...)
{
	if (tick || Alpha >= MaxValue)
	{
		tick = true;
		if(!(Alpha <= 0))
			Alpha -= Speed;
		else if (Alpha <= 0)
			tick = !tick;
	}
	else if (!tick || Alpha != MaxValue)
	{
		tick = false;
		if(!(Alpha >= MaxValue))
			Alpha += Speed;
		else if (Alpha >= MaxValue)
			tick = true;
	}
	
	const ImVec4& col = ImColor(R,G,B, (int)Alpha);
	va_list args;
    va_start(args, label);
    ImGui::TextColoredV(col, label, args);
    va_end(args);
}

bool ImNation::Button(const char* label, const ImVec2& size_arg) {
    static std::map<ImGuiID, button_animation> button_circle;
    static std::map<ImGuiID, float> button_hover;

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = CalcTextSize(name, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, 0);

    // Render
    const ImU32 col = GetColorU32(ImGuiCol_Button);
    RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

    // Press button animation (Drawing circle in clipped area)
    auto it_circle = button_circle.find(id);
    if (it_circle == button_circle.end())
    {
        button_circle.insert({ id, {false, false, 0.f, 0.f, ImVec2(0, 0)} });
        it_circle = button_circle.find(id);
    }

    // If pressed and wasn't active - start animation
    if (pressed && !it_circle->second.clicked)
    {
        it_circle->second.clicked = true;
        it_circle->second.pos = GetIO().MousePos;
    }

    // If pressed and was active - restart animation
    else if (pressed && it_circle->second.clicked)
    {
        it_circle->second.mult = 0.f;
        it_circle->second.size = 0.f;

        it_circle->second.reverse = false;
        it_circle->second.clicked = true;

        it_circle->second.pos = GetIO().MousePos;
    }

    // If we have active animation - procedure it
    if (it_circle->second.clicked)
    {
        // First stage - size(+) alpha(-)
        if (!it_circle->second.reverse)
        {
            it_circle->second.size = ImClamp(it_circle->second.size + 1.5f * GetIO().DeltaTime, 0.f, 1.f);
            it_circle->second.mult = ImClamp(it_circle->second.mult + 2.f * GetIO().DeltaTime, 0.f, 1.f);

            // Go for the second stage
            if (it_circle->second.mult >= 0.99f)
                it_circle->second.reverse = true;
        }

        // Second stage - size(+) alpha(-)
        else
        {
            it_circle->second.size = ImClamp(it_circle->second.size + 1.5f * GetIO().DeltaTime, 0.f, 1.f);
            it_circle->second.mult = ImClamp(it_circle->second.mult - 2.f * GetIO().DeltaTime, 0.f, 1.f);

            // Stop animation
            if (it_circle->second.mult <= 0.01f)
            {
                it_circle->second.mult = 0.f;
                it_circle->second.size = 0.f;

                it_circle->second.reverse = false;
                it_circle->second.clicked = false;
            }

        }
    }

    // Animated circle (on-press animation) color
    ImVec4 circle_color = ImVec4(0.98f, 0.98f, 0.98f, it_circle->second.mult * 0.6f);

    // Circle radius calcalution
    float radius = ImMax(fabs(it_circle->second.pos.x - bb.Max.x), fabs(it_circle->second.pos.x - bb.Min.x)) * 2.f;

    // Clip area to draw a cicrle inside frame
    window->DrawList->PushClipRect(bb.Min, bb.Max);
    window->DrawList->AddCircleFilled(it_circle->second.pos, radius * it_circle->second.size, GetColorU32(circle_color), 30);
    window->DrawList->PopClipRect();

    // hover animation
    auto it_hover = button_hover.find(id);
    if (it_hover == button_hover.end())
    {
        button_hover.insert({ id, {0.f} });
        it_hover = button_hover.find(id);
    }

    it_hover->second = ImClamp(it_hover->second + 2.5f * GetIO().DeltaTime * (hovered ? 1.f : -1.f), 0.f, 1.f);

    ImVec4 hovering_color = ImVec4(0.98f, 0.98f, 0.98f, it_hover->second * 0.4f);
   
    window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(hovering_color), GetStyle().FrameRounding);

    ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, name, NULL, &label_size, style.ButtonTextAlign, &bb);

    return pressed;
}
