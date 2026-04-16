#pragma once
#include "Core/Layer.h"
#include "Renderer/Renderer2D.h"
#include "Camera/OrthographicCamera.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <memory>

/**
 * EditorLayer: 你的主要业务/开发层
 * 所有的物体移动、摄像机控制、DrawQuad 调用都发生在这里
 */
class EditorLayer : public KaiEngine::Layer {
public:
    //初始化层名称及摄像机视野 (0, 1280, 720, 0)
    EditorLayer() 
        : Layer("EditorLayer"), m_Camera(0.0f, 1280.0f, 720.0f, 0.0f) 
    {}

    // 当层被推入 Application 时调用，适合加载纹理等一次性资源
    virtual void OnAttach() override {
        m_LogoTex = std::make_shared<KaiEngine::Texture2D>("assets/logo.png");
        KAI_CORE_INFO("EditorLayer Attached: 资源加载完成");
    }

    // 每帧更新逻辑
    virtual void OnUpdate(float dt) override {
        //  摄像机控制逻辑 (I, J, K, L) 
        glm::vec3 camPos = m_Camera.GetPosition();
        float camSpeed = 500.0f;
        if (KaiEngine::Input::IsKeyPressed(GLFW_KEY_I)) camPos.y -= camSpeed * dt; 
        if (KaiEngine::Input::IsKeyPressed(GLFW_KEY_K)) camPos.y += camSpeed * dt; 
        if (KaiEngine::Input::IsKeyPressed(GLFW_KEY_J)) camPos.x -= camSpeed * dt; 
        if (KaiEngine::Input::IsKeyPressed(GLFW_KEY_L)) camPos.x += camSpeed * dt; 
        
        m_Camera.SetPosition(camPos);

        //  玩家移动逻辑 (W, A, S, D) 
        float playerSpeed = 600.0f;
        if (KaiEngine::Input::IsKeyPressed(GLFW_KEY_W)) m_PlayerPos.y -= playerSpeed * dt;
        if (KaiEngine::Input::IsKeyPressed(GLFW_KEY_S)) m_PlayerPos.y += playerSpeed * dt;
        if (KaiEngine::Input::IsKeyPressed(GLFW_KEY_A)) m_PlayerPos.x -= playerSpeed * dt;
        if (KaiEngine::Input::IsKeyPressed(GLFW_KEY_D)) m_PlayerPos.x += playerSpeed * dt;

        // --
        // 传入摄像机的 View-Projection 矩阵，决定渲染哪些区域
        KaiEngine::Renderer2D::BeginScene(m_Camera.GetViewProjectionMatrix());

        // 绘制背景参考 (红方块)
        KaiEngine::Renderer2D::DrawQuad({ 800.0f, 360.0f }, { 150.0f, 150.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });

        // 绘制贴图 (UE Logo)
        KaiEngine::Renderer2D::DrawQuad({ 400.0f, 200.0f }, { 200.0f, 200.0f }, m_LogoTex);

        // 绘制玩家 (绿方块)
        KaiEngine::Renderer2D::DrawQuad(m_PlayerPos, { 100.0f, 100.0f }, { 0.2f, 0.8f, 0.3f, 1.0f });

        KaiEngine::Renderer2D::EndScene();
    }

    virtual void OnDetach() override {
        KAI_CORE_INFO("EditorLayer Detached");
    }

private:
    // 引擎核心组件
    KaiEngine::OrthographicCamera m_Camera;
    std::shared_ptr<KaiEngine::Texture2D> m_LogoTex;

    // 业务数据
    glm::vec3 m_PlayerPos = { 640.0f, 360.0f, 0.0f };
};