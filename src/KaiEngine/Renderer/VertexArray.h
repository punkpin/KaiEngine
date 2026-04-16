#pragma once
#include <vector>
#include <memory>
#include "Buffer.h"

namespace KaiEngine {

    class VertexArray {
    public:
        VertexArray();
        virtual ~VertexArray();

        void Bind() const;
        void Unbind() const;

        // 添加顶点缓冲
        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo);
        // 设置索引缓冲
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ebo);

        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

    private:
        uint32_t m_RendererID; 

        
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };

}