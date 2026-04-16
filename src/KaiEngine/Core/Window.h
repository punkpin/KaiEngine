#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Logger.h"

namespace KaiEngine {

    class Window {
    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }
        void OnUpdate(); 

        void Clear(float r = 0.15f, float g = 0.15f, float b = 0.15f);

        bool IsKeyPressed(int key) const;

        GLFWwindow* GetNativeWindow() const { return m_Window; }

    private:
        GLFWwindow* m_Window = nullptr;
        int m_Width, m_Height;
        std::string m_Title;
    };
}