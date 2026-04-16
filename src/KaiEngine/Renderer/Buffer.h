#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <initializer_list>

namespace KaiEngine {

    struct BufferElement {
        std::string Name;
        uint32_t Size;
        uint32_t Offset;

        BufferElement(uint32_t size, const std::string& name)
            : Name(name), Size(size), Offset(0) {}
    };

    class BufferLayout {
    public:
        BufferLayout() : m_Stride(0) {}
        BufferLayout(const std::initializer_list<BufferElement>& elements)
            : m_Elements(elements) {
            uint32_t offset = 0;
            m_Stride = 0;
            for (auto& element : m_Elements) {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

        inline uint32_t GetStride() const { return m_Stride; }
        inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride;
    };

    class VertexBuffer {
    public:
        VertexBuffer(float* vertices, uint32_t size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

        void SetData(const void* data, uint32_t size); 

        void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
        const BufferLayout& GetLayout() const { return m_Layout; }

    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };

    class IndexBuffer {
    public:
        IndexBuffer(uint32_t* indices, uint32_t count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;
        uint32_t GetCount() const { return m_Count; }

    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };
}