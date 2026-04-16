#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"

namespace KaiEngine {

    class Renderer2D {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const glm::mat4& viewProjection);
        static void EndScene();

        // 纹理绘制
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture);

        // 颜色绘制
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

    private:
        struct Renderer2DStorage;
        static Renderer2DStorage* s_Data;
    };
}