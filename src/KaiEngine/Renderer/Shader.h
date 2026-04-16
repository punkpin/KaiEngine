#pragma once
#include <string>
#include <glm/glm.hpp>
#include <cstdint>

namespace KaiEngine {
    class Shader {
    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void SetInt(const std::string& name, int value);
        void SetIntArray(const std::string& name, int* values, uint32_t count); // 新增
        void SetMat4(const std::string& name, const glm::mat4& mat);

    private:
        uint32_t m_RendererID;
    };
}