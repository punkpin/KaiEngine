#include "Application.h"
#include <GLFW/glfw3.h>
#include "Renderer/Renderer2D.h"
#include "Logger.h"

namespace KaiEngine {
    Application* Application::s_Instance = nullptr;

    Application::Application() {
        s_Instance = this;
        m_Window = std::make_unique<Window>(1280, 720, "KaiEngine Editor");
        Renderer2D::Init();
    }

    Application::~Application() {
        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
            if (*it) {
                (*it)->OnDetach();
            }
        }
        m_LayerStack.clear();
        Renderer2D::Shutdown();
        s_Instance = nullptr;
    }

    void Application::PushLayer(std::unique_ptr<Layer> layer) {
        if (!layer) {
            KAI_CORE_WARN("PushLayer ignored: null layer pointer");
            return;
        }
        m_LayerStack.push_back(std::move(layer));
        m_LayerStack.back()->OnAttach();
    }

    void Application::Run() {
        while (m_Running && !m_Window->ShouldClose()) {
            float time = (float)glfwGetTime();
            float deltaTime = time - m_LastFrameTime;
            m_LastFrameTime = time;

            m_Window->Clear(0.15f, 0.15f, 0.15f);

            for (auto& layer : m_LayerStack)
                layer->OnUpdate(deltaTime);

            m_Window->OnUpdate();
        }
    }
}