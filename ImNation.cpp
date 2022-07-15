#include "ImNation.h"
#include <map>
// name == label
struct checkbox_animation {
    bool clicked;
    bool reverse;
    float size;
    float mult;
};
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
bool ImNation::LoadingBar(const char* label, float value,  const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;
        
        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = size_arg;
        size.x -= style.FramePadding.x * 2;
        
        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ImGui::ItemSize(bb, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, id))
            return false;
        
        // Render
        const float circleStart = size.x * 0.7f;
        const float circleEnd = size.x;
        const float circleWidth = circleEnd - circleStart;
        
        window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
        window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart* value, bb.Max.y), fg_col);
        
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

bool ImNation::LoadingCircle1(const char* label, float radius, int thickness, const ImU32& color) 
{
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;
        
        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        
        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size((radius )*2, (radius + style.FramePadding.y)*2);
        
        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ImGui::ItemSize(bb, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, id))
            return false;
        
        // Render
        window->DrawList->PathClear();
        
        int num_segments = 30;
        int start = fabs(ImSin(g.Time*1.8f)*(num_segments-5));
        
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
void ImNation::LoadingCircle(const ImVec4& fore_color, const ImVec4& back_color, float arc_size, float radius, float thickness)
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
void ImNation::ChildSizex(int ChildSizex, int ChildSizex2, int tosize, int Speed){
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
void ImNation::ChildSizey(int ChildSizey, int ChildSizey2, int tosize, int Speed){
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
void ImNation::ChildSizex(int ChildSizex, int tosize, int Speed){
	if (ImGui::IsItemHovered())
	{
		if (!(ChildSizex >= ChildSizex + tosize))
			ChildSizex += Speed;
	} else {
		if (ChildSizex != ChildSizex)
			ChildSizex -= Speed;
	}
}
void ImNation::ChildSizey(int ChildSizey, int tosize, int Speed){
	if (ImGui::IsItemHovered())
	{
		if (!(ChildSizey >= ChildSizey + tosize))
			ChildSizey += Speed;
	} else {
		if (ChildSizey != ChildSizey)
			ChildSizey -= Speed;
	}
}
void ImNation::TextColored(int R, int G, int B, const char* label, float Alpha, bool tick, int MaxValue, int Speed, ...)
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
void ImNation::ToggleButton(const char* str_id, bool* v)
{
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.50f;

    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    if (ImGui::IsItemClicked())
        *v = !*v;

    float t = *v ? 1.0f : 0.0f;

    ImGuiContext& g = *GImGui;
    float ANIM_SPEED = 0.08f;
    if (g.LastActiveId == g.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
    {
        float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
        t = *v ? (t_anim) : (1.0f - t_anim);
    }

    ImU32 col_bg;
    if (ImGui::IsItemHovered())
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
    else
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));

    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
    draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}
bool ImNation::Checkbox(const char* name, bool* callback)
{
	static std::map<ImGuiID, checkbox_animation> circle_anim;

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = ImGui::CalcTextSize(name, NULL, true);

    const float square_sz = ImGui::GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *callback = !(*callback);
        ImGui::MarkItemEdited(id);
    }

    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));

    ImGui::RenderFrame(check_bb.Min, check_bb.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
    ImU32 check_col = ImGui::GetColorU32(ImGuiCol_CheckMark);

    float radius = square_sz * 0.82f;
    auto it_circle = circle_anim.find(id);
    if (it_circle == circle_anim.end())
    {
        circle_anim.insert({ id, {false, false, 0.f, 0.f} });
        it_circle = circle_anim.find(id);
    }

    if (pressed && *callback)
        it_circle->second.clicked = true;
    else if (pressed && !(*callback) && it_circle->second.clicked)
    {
        it_circle->second.mult = 0.f;
        it_circle->second.size = 0.f;

        it_circle->second.reverse = false;
        it_circle->second.clicked = false;
    }

    if (it_circle->second.clicked)
    {
        if (!it_circle->second.reverse)
        {
            it_circle->second.size = ImClamp(it_circle->second.size + 3.f * ImGui::GetIO().DeltaTime, 0.f, 1.f);
            it_circle->second.mult = ImClamp(it_circle->second.mult + 6.f * ImGui::GetIO().DeltaTime, 0.f, 1.f);

            if (it_circle->second.mult >= 0.99f)
                it_circle->second.reverse = true;
        }
        else
        {
            it_circle->second.size = ImClamp(it_circle->second.size + 3.f * ImGui::GetIO().DeltaTime, 0.f, 1.f);
            it_circle->second.mult = ImClamp(it_circle->second.mult - 6.f * ImGui::GetIO().DeltaTime, 0.f, 1.f);

            if (it_circle->second.mult <= 0.01f)
            {
                it_circle->second.mult = 0.f;
                it_circle->second.size = 0.f;

                it_circle->second.reverse = false;
                it_circle->second.clicked = false;
            }
               
        }
    }


    if (*callback)
    {
        ImGui::RenderFrame(check_bb.Min, check_bb.Max, ImGui::GetColorU32(g_menu.menu_color.Value), true, style.FrameRounding);

        const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 5.f));
        ImGui::RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);
    }

    ImU32 circle_col = ImGui::GetColorU32(ImGuiCol_CheckMark, it_circle->second.mult * 0.7f);

    if (it_circle->second.mult > 0.01f)
        window->DrawList->AddCircleFilled(ImVec2(check_bb.Min.x + (check_bb.Max.x - check_bb.Min.x) / 2, check_bb.Min.y + (check_bb.Max.y - check_bb.Min.y) / 2), radius * it_circle->second.size, circle_col, 30);

    // Better use clipped text (aligns and some else..)
    if (label_size.x > 0.0f)
        ImGui::RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + ((check_bb.Max.y - check_bb.Min.y) / 2) - label_size.y / 2 - 1), name);

    return pressed;
}
bool ImNation::Button(const char* name, const ImVec2& size_arg) {
    static std::map<ImGuiID, button_animation> button_circle;
    static std::map<ImGuiID, float> button_hover;

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = ImGui::CalcTextSize(name, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    // Render
    const ImU32 col = ImGui::GetColorU32(ImGuiCol_Button);
    ImGui::RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

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
        it_circle->second.pos = ImGui::GetIO().MousePos;
    }

    // If pressed and was active - restart animation
    else if (pressed && it_circle->second.clicked)
    {
        it_circle->second.mult = 0.f;
        it_circle->second.size = 0.f;

        it_circle->second.reverse = false;
        it_circle->second.clicked = true;

        it_circle->second.pos = ImGui::GetIO().MousePos;
    }

    // If we have active animation - procedure it
    if (it_circle->second.clicked)
    {
        // First stage - size(+) alpha(-)
        if (!it_circle->second.reverse)
        {
            it_circle->second.size = ImClamp(it_circle->second.size + 1.5f * ImGui::GetIO().DeltaTime, 0.f, 1.f);
            it_circle->second.mult = ImClamp(it_circle->second.mult + 2.f * ImGui::GetIO().DeltaTime, 0.f, 1.f);

            // Go for the second stage
            if (it_circle->second.mult >= 0.99f)
                it_circle->second.reverse = true;
        }

        // Second stage - size(+) alpha(-)
        else
        {
            it_circle->second.size = ImClamp(it_circle->second.size + 1.5f * ImGui::GetIO().DeltaTime, 0.f, 1.f);
            it_circle->second.mult = ImClamp(it_circle->second.mult - 2.f * ImGui::GetIO().DeltaTime, 0.f, 1.f);

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
    window->DrawList->AddCircleFilled(it_circle->second.pos, radius * it_circle->second.size, ImGui::GetColorU32(circle_color), 30);
    window->DrawList->PopClipRect();

    // hover animation
    auto it_hover = button_hover.find(id);
    if (it_hover == button_hover.end())
    {
        button_hover.insert({ id, {0.f} });
        it_hover = button_hover.find(id);
    }

    it_hover->second = ImClamp(it_hover->second + 2.5f * ImGui::GetIO().DeltaTime * (hovered ? 1.f : -1.f), 0.f, 1.f);

    ImVec4 hovering_color = ImVec4(0.98f, 0.98f, 0.98f, it_hover->second * 0.4f);
   
    window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(hovering_color), ImGui::GetStyle().FrameRounding);

    ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, name, NULL, &label_size, style.ButtonTextAlign, &bb);

    return pressed;
}