#pragma once
#include <memory>
#include <vector>
#include "Window.h"
#include "Layer.h"

namespace KaiEngine {
    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();
        void PushLayer(std::unique_ptr<Layer> layer);

        static Application& Get() { return *s_Instance; }
        Window& GetWindow() { return *m_Window; }

    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        std::vector<std::unique_ptr<Layer>> m_LayerStack;
        float m_LastFrameTime = 0.0f;

        static Application* s_Instance;
    };
}