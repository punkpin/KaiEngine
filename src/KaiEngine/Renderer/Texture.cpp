#include "Texture.h"
#include "stb_image.h"
#include <glad/gl.h>
#include "Logger.h" 

namespace KaiEngine {

    Texture2D::Texture2D(const std::string& path)
        : m_RendererID(0), m_Path(path), m_Width(0), m_Height(0), m_Channels(0) 
    {
        // 1. 强制打印加载尝试
        KAI_CORE_TRACE("Texture2D: 尝试从路径加载纹理: {0}", path);

        // 3. 加载图片数据
        int width, height, channels;
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 4);

        if (data) {
            m_Width = width;
            m_Height = height;
            m_Channels = channels;

            // 4. 生成 OpenGL 纹理
            glGenTextures(1, &m_RendererID);
            glBindTexture(GL_TEXTURE_2D, m_RendererID);

            // 5. 设置采样和包裹参数（不设这些参数有时会导致黑屏）
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // 6. 上传数据到显存
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            
            // 7. 释放 CPU 内存
            stbi_image_free(data);

            KAI_CORE_INFO("Texture2D: 成功加载纹理 [ID: {0}] {1}x{2}", m_RendererID, m_Width, m_Height);
        }
        else {
            // 8. 失败报告
            KAI_CORE_ERROR("Texture2D: 纹理加载失败！无法打开文件: {0}", path);
            KAI_CORE_ERROR("stbi 原因: {0}", stbi_failure_reason()); 
        }
    }

    Texture2D::Texture2D(uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height), m_Channels(4) 
{
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

// 向显存写入像素数据
void Texture2D::SetData(void* data, uint32_t size) {
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

    Texture2D::~Texture2D() {
        if (m_RendererID) {
            glDeleteTextures(1, &m_RendererID);
        }
    }

    void Texture2D::Bind(uint32_t slot) const {
        // 激活对应的纹理槽位
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }
}