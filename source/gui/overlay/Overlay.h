//
// Created by vlad on 11/13/23.
//

#pragma once


struct GLFWwindow;


namespace ktth::overlay
{
    class Overlay
    {
    public:
        explicit Overlay() noexcept;
        void Run();
    private:
        GLFWwindow* m_windowHandle;
    };
} // overaly
