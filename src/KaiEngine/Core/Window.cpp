#include "Window.h"
#include <iostream>

namespace KaiEngine {

    Window::Window(int width, int height, const std::string& title) 
        : m_Width(width), m_Height(height), m_Title(title) 
    {
        if (!glfwInit()) {
            KAI_CORE_ERROR("Could not initialize GLFW!");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
        
        if (!m_Window) {
            KAI_CORE_ERROR("Failed to create GLFW window!");
            glfwTerminate();
        }

        glfwMakeContextCurrent(m_Window);
        
        if (!gladLoaderLoadGL()) {
            KAI_CORE_ERROR("Failed to initialize Glad Loader!");
        }

        KAI_CORE_INFO("Window Created: {0}x{1}", m_Width, m_Height);
    }

    Window::~Window() {
        if (m_Window) {
            glfwDestroyWindow(m_Window);
            m_Window = nullptr;
        }
        glfwTerminate();
    }

    void Window::Clear(float r, float g, float b) {
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::OnUpdate() {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    bool Window::IsKeyPressed(int key) const {
        // 调用 GLFW 的接口检测按键状态
        auto state = glfwGetKey(m_Window, key);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }
}