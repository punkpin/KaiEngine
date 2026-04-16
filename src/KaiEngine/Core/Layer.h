#pragma once
#include <string>

namespace KaiEngine {
    class Layer {
    public:
        Layer(const std::string& name = "Layer") : m_DebugName(name) {}
        virtual ~Layer() = default;

        virtual void OnAttach() {}           // 当层被推入层栈时调用
        virtual void OnDetach() {}           // 当层从层栈移除时调用
        virtual void OnUpdate(float dt) {}   // 每帧更新逻辑和渲染
        virtual void OnImGuiRender() {}      // 专门用于渲染 UI 面板

        inline const std::string& GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };
}