#pragma once

namespace KaiEngine {

    class Input {
    public:
        // 检查某个按键按下
        static bool IsKeyPressed(int keycode);

        // 检查鼠标按键和位置
        static bool IsMouseButtonPressed(int button);
        static float GetMouseX();
        static float GetMouseY();
    };

}