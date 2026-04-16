#include "Renderer2D.h"
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include "Logger.h"
#include "Buffer.h"

namespace KaiEngine {

    // 核心对齐：结构体顺序必须与 Shader 中的 layout (location = n) 严格一致
    struct QuadVertex {
        glm::vec3 Position;  // Location 0
        glm::vec4 Color;     // Location 1
        glm::vec2 TexCoord;  // Location 2
        float TexIndex;      // Location 3
    };

    struct Renderer2D::Renderer2DStorage {
        static const uint32_t MaxQuads = 10000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32;

        std::shared_ptr<VertexArray> QuadVAO;
        std::shared_ptr<VertexBuffer> QuadVBO;
        std::shared_ptr<Shader> TextureShader;
        std::shared_ptr<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0号留给白图
    };

    Renderer2D::Renderer2DStorage* Renderer2D::s_Data = nullptr;

    void Renderer2D::Init() {
        s_Data = new Renderer2DStorage();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        s_Data->QuadVAO = std::make_shared<VertexArray>();

        s_Data->QuadVBO = std::make_shared<VertexBuffer>(
            nullptr,
            static_cast<uint32_t>(s_Data->MaxVertices * sizeof(QuadVertex))
        );
        
        // 核心布局声明：顺序与结构体 QuadVertex 完全匹配
        s_Data->QuadVBO->SetLayout({
            { sizeof(glm::vec3), "a_Position" },
            { sizeof(glm::vec4), "a_Color" },
            { sizeof(glm::vec2), "a_TexCoord" },
            { sizeof(float),     "a_TexIndex" }
        });
        s_Data->QuadVAO->AddVertexBuffer(s_Data->QuadVBO);

        s_Data->QuadVertexBufferBase = new QuadVertex[s_Data->MaxVertices];

        uint32_t* quadIndices = new uint32_t[s_Data->MaxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data->MaxIndices; i += 6) {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;
            offset += 4;
        }
        auto ebo = std::make_shared<IndexBuffer>(quadIndices, s_Data->MaxIndices);
        s_Data->QuadVAO->SetIndexBuffer(ebo);
        delete[] quadIndices;

        // 创建 1x1 纯白纹理用于纯色绘制
        s_Data->WhiteTexture = std::make_shared<Texture2D>(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
        s_Data->TextureSlots[0] = s_Data->WhiteTexture;

        s_Data->TextureShader = std::make_shared<Shader>("shaders/default.vert", "shaders/default.frag");
        s_Data->TextureShader->Bind();

        int32_t samplers[s_Data->MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data->MaxTextureSlots; i++) {
            samplers[i] = i;
        }
        s_Data->TextureShader->SetIntArray("u_Textures", samplers, s_Data->MaxTextureSlots);

        KAI_CORE_INFO("Renderer2D: 批处理系统就绪 [Location 对齐已修正]");
    }

    void Renderer2D::Shutdown() {
        delete[] s_Data->QuadVertexBufferBase;
        delete s_Data;
    }

    void Renderer2D::BeginScene(const glm::mat4& viewProjection) {
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetMat4("u_ViewProjection", viewProjection);
        s_Data->QuadIndexCount = 0;
        s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
        s_Data->TextureSlotIndex = 1;
    }

    void Renderer2D::EndScene() {
        if (s_Data->QuadIndexCount == 0) return;

        uint32_t dataSize = (uint32_t)((uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase);
        s_Data->QuadVBO->SetData(s_Data->QuadVertexBufferBase, dataSize);

        for (uint32_t i = 0; i < s_Data->TextureSlotIndex; i++) {
            if (s_Data->TextureSlots[i]) s_Data->TextureSlots[i]->Bind(i);
        }

        s_Data->QuadVAO->Bind();
        glDrawElements(GL_TRIANGLES, s_Data->QuadIndexCount, GL_UNSIGNED_INT, nullptr);
    }

    // 绘制纯色
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
        if (s_Data->QuadIndexCount + 6 > Renderer2DStorage::MaxIndices) {
            EndScene();
            s_Data->QuadIndexCount = 0;
            s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
            s_Data->TextureSlotIndex = 1;
        }

        const float textureIndex = 0.0f; // 使用白图
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        static const glm::vec4 quadCoords[4] = {
            { -0.5f, -0.5f, 0.0f, 1.0f }, 
            {  0.5f, -0.5f, 0.0f, 1.0f },
            {  0.5f,  0.5f, 0.0f, 1.0f }, 
            { -0.5f,  0.5f, 0.0f, 1.0f }
        };

        // 统一 UV 映射
        static const glm::vec2 texCoords[4] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

        for (int i = 0; i < 4; i++) {
            s_Data->QuadVertexBufferPtr->Position = transform * quadCoords[i];
            s_Data->QuadVertexBufferPtr->Color = color;
            s_Data->QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data->QuadVertexBufferPtr++;
        }

        s_Data->QuadIndexCount += 6;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    // 绘制纹理
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture) {
        if (s_Data->QuadIndexCount + 6 > Renderer2DStorage::MaxIndices) {
            EndScene();
            s_Data->QuadIndexCount = 0;
            s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
            s_Data->TextureSlotIndex = 1;
        }

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data->TextureSlotIndex; i++) {
            if (s_Data->TextureSlots[i]->GetRendererID() == texture->GetRendererID()) {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f) {
            if (s_Data->TextureSlotIndex >= Renderer2DStorage::MaxTextureSlots) {
                EndScene();
                s_Data->QuadIndexCount = 0;
                s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;
                s_Data->TextureSlotIndex = 1;
            }

            textureIndex = (float)s_Data->TextureSlotIndex;
            s_Data->TextureSlots[s_Data->TextureSlotIndex] = texture;
            s_Data->TextureSlotIndex++;
        }

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        static const glm::vec4 quadCoords[4] = {
            { -0.5f, -0.5f, 0.0f, 1.0f }, 
            {  0.5f, -0.5f, 0.0f, 1.0f },
            {  0.5f,  0.5f, 0.0f, 1.0f }, 
            { -0.5f,  0.5f, 0.0f, 1.0f }
        };
        static const glm::vec2 texCoords[4] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

        for (int i = 0; i < 4; i++) {
            s_Data->QuadVertexBufferPtr->Position = transform * quadCoords[i];
            s_Data->QuadVertexBufferPtr->Color = color;
            s_Data->QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data->QuadVertexBufferPtr++;
        }

        s_Data->QuadIndexCount += 6;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture) {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture);
    }
}