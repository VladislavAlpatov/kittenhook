
#include "memory_manager/MemoryManager.h"
#include "apex_sdk/BaseEntity.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <apex_sdk/EntityList.h>
#include <apex_sdk/WorldToScreen.h>
#include <format>


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


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (auto const entity :apex_sdk::EntityList::GetAllEntities())
        {
            const auto projPos = apex_sdk::WorldToScreen(entity.GetOrigin(), 1920, 1080);

            if (!projPos)
                continue;

            ImGui::GetBackgroundDrawList()->AddText({projPos.value().x,
                                                     projPos.value().y},
                                                    ImColor(255,0,0),
                                                    "-=PLAYER=-");
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
