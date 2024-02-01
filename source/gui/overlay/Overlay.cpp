//
// Created by vlad on 11/13/23.
//

#include "Overlay.h"

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_freetype.h>

#include <apex_sdk/BaseEntity.h>
#include <apex_sdk/EntityList.h>
#include <apex_sdk/WorldToScreen.h>
#include <format>
#include <unordered_map>

void TextCentered(const std::string& text, const ImVec2& pos) {
    auto textWidth   = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::GetBackgroundDrawList()->AddText({pos.x - textWidth / 2.f, pos.y}, ImColor(0,255,0), text.c_str());
}

namespace ktth::overlay
{

    Overlay::Overlay() noexcept
    {
        glfwInit();

        glfwWindowHint(GLFW_FLOATING,                true);
        glfwWindowHint(GLFW_RESIZABLE,               false);
        glfwWindowHint(GLFW_MAXIMIZED,               true);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
        glfwWindowHint(GLFW_MOUSE_PASSTHROUGH,       true);

        auto videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        m_windowHandle = glfwCreateWindow(videoMode->width, videoMode->height, "Overlay", nullptr, nullptr);

        glfwSetWindowAttrib(m_windowHandle, GLFW_DECORATED, false);

        glfwSetWindowMonitor(m_windowHandle, nullptr, 0, 0, videoMode->width, videoMode->height, 0);
        glfwMakeContextCurrent(m_windowHandle);

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(m_windowHandle, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        ImGuiIO& io     = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImFontConfig cfg;
        cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Monochrome | ImGuiFreeTypeBuilderFlags_MonoHinting;
        // static ImWchar ranges[] = { 0x1, 0xFFFD, 0 };


        io.Fonts->AddFontFromFileTTF(R"(/home/vlad/.fonts/Verdana/Verdana-Bold.ttf)", 11.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    }

    void Overlay::Run()
    {
        while (!glfwWindowShouldClose(m_windowHandle))
        {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            auto localPlayer = apex_sdk::EntityList::GetLocalPlayer();
            if (localPlayer.has_value())
                for (auto const entity : apex_sdk::EntityList::GetAllEntities())
                {
                    if (!entity.IsAlive() or *localPlayer == entity or entity.IsKnockedout() or localPlayer->GetTeamNumber() == entity.GetTeamNumber())
                        continue;

                    auto bottom = apex_sdk::WorldToScreen(entity.GetOrigin(), 1920, 1080);
                    auto top = apex_sdk::WorldToScreen(entity.GetBonePosition(8)+uml::Vector3{0.f,0.f,10.f},
                                                       1920,
                                                       1080);

                    if (!top or !bottom)
                        continue;

                    top->x = bottom->x;

                    const float delta = std::abs(top->y - bottom->y);

                    const auto topLeft = top.value() - uml::Vector3{delta / 3, 0, 0};
                    const auto topRight = top.value() + uml::Vector3{delta / 3, 0, 0};

                    const auto bottomLeft = bottom.value() - uml::Vector3{delta / 3, 0, 0};
                    const auto bottomRight = bottom.value() + uml::Vector3{delta / 3, 0, 0};

                    auto drawList = ImGui::GetBackgroundDrawList();

                    drawList->AddLine({topLeft.x, topLeft.y}, {topRight.x, topRight.y}, ImColor(255, 0,0));
                    drawList->AddLine({bottomLeft.x, bottomLeft.y}, {bottomRight.x, bottomRight.y}, ImColor(255, 0,0));
                    drawList->AddLine({topLeft.x, topLeft.y}, {bottomLeft.x, bottomLeft.y}, ImColor(255, 0,0));
                    drawList->AddLine({topRight.x, topRight.y}, {bottomRight.x, bottomRight.y}, ImColor(255, 0,0));

                    static std::unordered_map<int, ImColor> shieldColors =
                    {
                        {50, {156, 156, 156}},
                        {75, {0, 162, 255}},
                        {100, {105, 0, 186}},
                        {125, {255,0,0}},
                    };
                    const auto entShieldsMax = entity.GetMaxShieldValue();
                    if (entShieldsMax > 0)
                        drawList->AddText({topRight.x+5, topRight.y},
                                          shieldColors.at(entShieldsMax),
                                          std::format("Shield: {}/{}", entity.GetShieldValue(), entShieldsMax).c_str());
                    const auto distance = localPlayer->GetCameraPosition().DistTo(entity.GetOrigin());
                    TextCentered(std::format("[{}]", (int)(distance * 2.54f / 100.f)), {bottom->x, bottom->y});
                }
            ImGui::Render();
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(m_windowHandle);
        }
    }
} // overaly