#pragma once
#include <string>
#include <glad/gl.h>

namespace KaiEngine {
    class Texture2D {
    public:

        Texture2D(uint32_t width, uint32_t height);
        void SetData(void* data, uint32_t size);
        
        Texture2D(const std::string& path);
        ~Texture2D();

        void Bind(uint32_t slot = 0) const;

        bool operator==(const Texture2D& other) const {
            return m_RendererID == other.m_RendererID;
        }

        uint32_t GetRendererID() const { return m_RendererID; }
        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }

    private:
        uint32_t m_RendererID; 
        std::string m_Path;
        int m_Width, m_Height, m_Channels;
    };
}