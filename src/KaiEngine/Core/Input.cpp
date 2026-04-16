#include "Input.h"
#include <GLFW/glfw3.h> // 底层依赖只在这里出现
#include "Logger.h"

namespace {
    GLFWwindow* GetContextWindowOrWarn() {
        static bool s_MissingContextWarned = false;
        GLFWwindow* window = glfwGetCurrentContext();

        if (!window) {
            if (!s_MissingContextWarned) {
                KAI_CORE_WARN("Input: 当前没有活动的 GLFW 上下文，返回默认输入状态");
                s_MissingContextWarned = true;
            }
            return nullptr;
        }

        s_MissingContextWarned = false;
        return window;
    }
}

namespace KaiEngine {

    bool Input::IsKeyPressed(int keycode) {
        auto window = GetContextWindowOrWarn();
        if (!window) return false;
        auto state = glfwGetKey(window, keycode);
        
        // 只要状态是“按下”或“重复（长按）”，就返回真
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(int button) {
        auto window = GetContextWindowOrWarn();
        if (!window) return false;
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    float Input::GetMouseX() {
        auto window = GetContextWindowOrWarn();
        if (!window) return 0.0f;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return (float)xpos;
    }

    float Input::GetMouseY() {
        auto window = GetContextWindowOrWarn();
        if (!window) return 0.0f;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return (float)ypos;
    }
}