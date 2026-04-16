#include <Windows.h>
#include <memory>
#include "Core/Application.h"
#include "EditorLayer.h"
#include "Core/Logger.h"

int main() {
    SetConsoleOutputCP(65001);
    KaiEngine::Logger::Init();

    KaiEngine::Application app;
    app.PushLayer(std::make_unique<EditorLayer>());
    app.Run();

    return 0;
}