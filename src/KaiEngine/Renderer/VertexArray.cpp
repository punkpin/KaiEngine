#include "VertexArray.h"
#include <glad/gl.h>

namespace KaiEngine {

    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_RendererID);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void VertexArray::Bind() const {
        glBindVertexArray(m_RendererID);
    }

    void VertexArray::Unbind() const {
        glBindVertexArray(0);
    }

    void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo) {
        this->Bind();
        vbo->Bind();

        const auto& layout = vbo->GetLayout();
        uint32_t index = 0;

        // 自动化工厂：根据 Layout 自动循环设置属性
        for (const auto& element : layout.GetElements()) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                element.Size / sizeof(float), // 元素的个数
                GL_FLOAT,
                GL_FALSE,
                layout.GetStride(),           // 步长
                (const void*)(uintptr_t)element.Offset // 偏移量
            );
            index++;
        }

        m_VertexBuffers.push_back(vbo);
    }

    void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ebo) {
        this->Bind();
        ebo->Bind();
        m_IndexBuffer = ebo;
    }
}