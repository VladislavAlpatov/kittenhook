
#include "memory_manager/MemoryManager.h"
#include "apex_sdk/BaseEntity.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <apex_sdk/EntityList.h>
#include <apex_sdk/WorldToScreen.h>
#include <format>
#include "hacks/Aimbot.h"

int main()
{

    glfwInit();

    glfwWindowHint(GLFW_FLOATING,                true);
    glfwWindowHint(GLFW_RESIZABLE,               false);
    glfwWindowHint(GLFW_MAXIMIZED,               true);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH,       true);

    auto video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(video_mode->width, video_mode->height, "Overlay", nullptr, nullptr);

    glfwSetWindowAttrib(window, GLFW_DECORATED, false);

    glfwSetWindowMonitor(window, nullptr, 0, 0, video_mode->width, video_mode->height, 0);
    glfwMakeContextCurrent(window);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    auto theme = ImGui::GetStyle().Colors;

    theme[ImGuiCol_WindowBg] = ImColor(8, 15, 32, 255);
    theme[ImGuiCol_Border]   = ImColor(27, 117, 255, 255);
    theme[ImGuiCol_Button]   = ImColor(2, 91, 255, 255);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        hacks::Aimbot().Run();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto localPlayer = apex_sdk::EntityList::GetLocalPlayer();
        if (localPlayer.has_value())
        for (auto const entity :apex_sdk::EntityList::GetAllEntities())
        {
            if (!entity.IsAlive() or entity.IsKnockedout() or *localPlayer == entity or localPlayer->GetTeamNumber() == entity.GetTeamNumber())
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
        }
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
